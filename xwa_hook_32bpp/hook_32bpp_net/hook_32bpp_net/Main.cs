using JeremyAnsel.Xwa.Opt;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace hook_32bpp_net
{
    public static class Main
    {
        private static IList<string> _getCustomFileLines_lines;
        private static string _getCustomFileLines_name;
        private static string _getCustomFileLines_mission;

        public static IList<string> GetCustomFileLines(string name)
        {
            string xwaMissionFileName = Marshal.PtrToStringAnsi(new IntPtr(0x06002E8));
            int currentGameState = Marshal.ReadInt32(new IntPtr(0x09F60E0 + 0x25FA9));
            int updateCallback = Marshal.ReadInt32(new IntPtr(0x09F60E0 + 0x25FB1 + currentGameState * 0x850 + 0x844));
            bool isTechLibraryGameStateUpdate = updateCallback == 0x00574D70;

            if (isTechLibraryGameStateUpdate)
            {
                _getCustomFileLines_name = name;
                _getCustomFileLines_mission = null;
                _getCustomFileLines_lines = XwaHooksConfig.GetFileLines("FlightModels\\" + name + ".txt");

                if (_getCustomFileLines_lines.Count == 0)
                {
                    _getCustomFileLines_lines = XwaHooksConfig.GetFileLines("FlightModels\\default.ini", name);
                }
            }
            else
            {
                if (_getCustomFileLines_name != name || _getCustomFileLines_mission != xwaMissionFileName)
                {
                    _getCustomFileLines_name = name;
                    _getCustomFileLines_mission = xwaMissionFileName;

                    string mission = XwaHooksConfig.GetStringWithoutExtension(xwaMissionFileName);
                    _getCustomFileLines_lines = XwaHooksConfig.GetFileLines(mission + "_" + name + ".txt");

                    if (_getCustomFileLines_lines.Count == 0)
                    {
                        _getCustomFileLines_lines = XwaHooksConfig.GetFileLines(mission + ".ini", name);
                    }

                    if (_getCustomFileLines_lines.Count == 0)
                    {
                        _getCustomFileLines_lines = XwaHooksConfig.GetFileLines("FlightModels\\" + name + ".txt");
                    }

                    if (_getCustomFileLines_lines.Count == 0)
                    {
                        _getCustomFileLines_lines = XwaHooksConfig.GetFileLines("FlightModels\\default.ini", name);
                    }
                }
            }

            return _getCustomFileLines_lines;
        }

        public static int GetFlightgroupsCount(IList<string> objectLines, string optName)
        {
            int count = 0;

            for (int index = 255; index >= 0; index--)
            {
                string key = optName + "_fg_" + index.ToString(CultureInfo.InvariantCulture);
                string value = XwaHooksConfig.GetFileKeyValue(objectLines, key);

                if (!string.IsNullOrEmpty(value))
                {
                    count = index + 1;
                    break;
                }
            }

            return count;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static int ReadOptFunction([MarshalAs(UnmanagedType.LPStr)] string optFilename)
        {
            if (!File.Exists(optFilename))
            {
                return 0;
            }

            string optName = Path.GetFileNameWithoutExtension(optFilename);
            IList<string> objectLines = GetCustomFileLines("Skins");
            IList<string> baseSkins = XwaHooksConfig.Tokennize(XwaHooksConfig.GetFileKeyValue(objectLines, optName));
            var baseDefaultSkinDirectory = new DirectoryInfo($"FlightModels\\Skins\\{optName}\\Default");
            bool baseDefaultSkinExists = baseDefaultSkinDirectory.Exists && baseDefaultSkinDirectory.EnumerateFiles().Any();
            int fgCount = GetFlightgroupsCount(objectLines, optName);
            bool hasSkins = baseDefaultSkinExists || baseSkins.Count != 0 || fgCount != 0;

            if (hasSkins)
            {
                var opt = OptFile.FromFile(optFilename);
                fgCount = Math.Max(fgCount, opt.MaxTextureVersion);
                UpdateOptFile(optName, opt, objectLines, baseSkins, fgCount);
                opt.Save("temp.opt");
            }

            return hasSkins ? 1 : 0;
        }

        private static void UpdateOptFile(string optName, OptFile opt, IList<string> objectLines, IList<string> baseSkins, int fgCount)
        {
            List<List<string>> fgSkins = ReadFgSkins(optName, objectLines, baseSkins, fgCount);
            List<string> distinctSkins = fgSkins.SelectMany(t => t).Distinct(StringComparer.InvariantCultureIgnoreCase).ToList();
            List<string> texturesExist = GetTexturesExist(optName, opt, distinctSkins);
            CreateSwitchTextures(opt, texturesExist, fgSkins);
            UpdateSkins(optName, opt, fgSkins);
        }

        private static List<List<string>> ReadFgSkins(string optName, IList<string> objectLines, IList<string> baseSkins, int fgCount)
        {
            var fgSkins = new List<List<string>>(fgCount);

            for (int i = 0; i < fgCount; i++)
            {
                var skins = new List<string>(baseSkins);
                string fgKey = optName + "_fg_" + i.ToString(CultureInfo.InvariantCulture);
                skins.AddRange(XwaHooksConfig.Tokennize(XwaHooksConfig.GetFileKeyValue(objectLines, fgKey)));

                if (skins.Count == 0)
                {
                    skins.Add("Default");
                }

                fgSkins.Add(skins);
            }

            return fgSkins;
        }

        private static List<string> GetTexturesExist(string optName, OptFile opt, List<string> distinctSkins)
        {
            var texturesExist = new List<string>();

            foreach (string textureName in opt.Textures.Keys)
            {
                foreach (string skin in distinctSkins)
                {
                    string baseFilename = $"FlightModels\\Skins\\{optName}\\{skin}\\{textureName}";

                    if (TextureExists(baseFilename, skin) != null)
                    {
                        texturesExist.Add(textureName);
                        break;
                    }
                }
            }

            return texturesExist;
        }

        private static void CreateSwitchTextures(OptFile opt, List<string> texturesExist, List<List<string>> fgSkins)
        {
            int fgCount = fgSkins.Count;

            if (fgCount == 0)
            {
                return;
            }

            var newTextures = new List<Texture>();

            foreach (var texture in opt.Textures)
            {
                if (!texturesExist.Contains(texture.Key))
                {
                    continue;
                }

                texture.Value.Convert8To32();

                for (int i = 0; i < fgCount; i++)
                {
                    Texture newTexture = texture.Value.Clone();
                    newTexture.Name += "_fg_" + i.ToString(CultureInfo.InvariantCulture) + "_" + string.Join(",", fgSkins[i]);
                    newTextures.Add(newTexture);
                }
            }

            foreach (var newTexture in newTextures)
            {
                opt.Textures.Add(newTexture.Name, newTexture);
            }

            foreach (var mesh in opt.Meshes)
            {
                foreach (var lod in mesh.Lods)
                {
                    foreach (var faceGroup in lod.FaceGroups)
                    {
                        if (faceGroup.Textures.Count == 0)
                        {
                            continue;
                        }

                        string name = faceGroup.Textures[0];

                        if (!texturesExist.Contains(name))
                        {
                            continue;
                        }

                        faceGroup.Textures.Clear();

                        for (int i = 0; i < fgCount; i++)
                        {
                            faceGroup.Textures.Add(name + "_fg_" + i.ToString(CultureInfo.InvariantCulture) + "_" + string.Join(",", fgSkins[i]));
                        }
                    }
                }
            }
        }

        private static void UpdateSkins(string optName, OptFile opt, List<List<string>> fgSkins)
        {
            foreach (var texture in opt.Textures)
            {
                int position = texture.Key.IndexOf("_fg_");

                if (position == -1)
                {
                    continue;
                }

                string textureName = texture.Key.Substring(0, position);
                int fgIndex = int.Parse(texture.Key.Substring(position + 4, texture.Key.IndexOf('_', position + 4) - position - 4), CultureInfo.InvariantCulture);

                foreach (string skin in fgSkins[fgIndex])
                {
                    string baseFilename = $"FlightModels\\Skins\\{optName}\\{skin}\\{textureName}";
                    string filename = TextureExists(baseFilename, skin);

                    if (filename == null)
                    {
                        continue;
                    }

                    CombineTextures(texture.Value, filename);
                }

                texture.Value.GenerateMipmaps();
            }
        }

        private static void CombineTextures(Texture baseTexture, string filename)
        {
            Texture newTexture = Texture.FromFile(filename);

            if (newTexture.Width != baseTexture.Width || newTexture.Height != baseTexture.Height)
            {
                return;
            }

            newTexture.Convert8To32();

            int size = baseTexture.Width * baseTexture.Height;
            byte[] src = newTexture.ImageData;
            byte[] dst = baseTexture.ImageData;

            for (int i = 0; i < size; i++)
            {
                int a = src[i * 4 + 3];

                dst[i * 4 + 0] = (byte)(dst[i * 4 + 0] * (255 - a) / 255 + src[i * 4 + 0] * a / 255);
                dst[i * 4 + 1] = (byte)(dst[i * 4 + 1] * (255 - a) / 255 + src[i * 4 + 1] * a / 255);
                dst[i * 4 + 2] = (byte)(dst[i * 4 + 2] * (255 - a) / 255 + src[i * 4 + 2] * a / 255);
            }
        }

        private static string TextureExists(string baseFilename, string skin)
        {
            var extensions = new string[] { ".bmp", ".png", ".jpg" };

            foreach (string ext in extensions)
            {
                string filename = baseFilename + "_" + skin + ext;

                if (File.Exists(filename))
                {
                    return filename;
                }
            }

            foreach (string ext in extensions)
            {
                string filename = baseFilename + ext;

                if (File.Exists(filename))
                {
                    return filename;
                }
            }

            return null;
        }
    }
}
