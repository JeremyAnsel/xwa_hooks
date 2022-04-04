using JeremyAnsel.IO.Locator;
using JeremyAnsel.Xwa.Opt;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
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

        private static int GetFlightgroupsCount(IList<string> objectLines, string optName)
        {
            int count = 0;

            for (int index = 255; index >= 0; index--)
            {
                string key = optName + "_fgc_" + index.ToString(CultureInfo.InvariantCulture);
                string value = XwaHooksConfig.GetFileKeyValue(objectLines, key);

                if (!string.IsNullOrEmpty(value))
                {
                    count = index + 1;
                    break;
                }
            }

            return count;
        }

        private static List<int> GetFlightgroupsColors(IList<string> objectLines, string optName, int fgCount, bool hasDefaultSkin)
        {
            bool hasBaseSkins = hasDefaultSkin || !string.IsNullOrEmpty(XwaHooksConfig.GetFileKeyValue(objectLines, optName));

            var colors = new List<int>();

            for (int index = 0; index < 256; index++)
            {
                string key = optName + "_fgc_" + index.ToString(CultureInfo.InvariantCulture);
                string value = XwaHooksConfig.GetFileKeyValue(objectLines, key);

                if (!string.IsNullOrEmpty(value) || (hasBaseSkins && index < fgCount))
                {
                    colors.Add(index);
                }
            }

            return colors;
        }

        private static string GetSkinDirectoryLocatorPath(string optName, string skinName)
        {
            string path = $"FlightModels\\Skins\\{optName}\\{skinName}";

            var baseDirectoryInfo = new DirectoryInfo(path);
            bool baseDirectoryExists = baseDirectoryInfo.Exists && baseDirectoryInfo.EnumerateFiles().Any();

            if (baseDirectoryExists)
            {
                return path;
            }

            if (File.Exists(path + ".zip"))
            {
                return path + ".zip";
            }

            if (File.Exists(path + ".7z"))
            {
                return path + ".7z";
            }

            return null;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static unsafe int ReadOptFunction([MarshalAs(UnmanagedType.LPStr)] string optFilename)
        {
            if (!File.Exists(optFilename))
            {
                return 0;
            }

            string optName = Path.GetFileNameWithoutExtension(optFilename);
            IList<string> objectLines = GetCustomFileLines("Skins");
            IList<string> baseSkins = XwaHooksConfig.Tokennize(XwaHooksConfig.GetFileKeyValue(objectLines, optName));
            bool hasDefaultSkin = GetSkinDirectoryLocatorPath(optName, "Default") != null;
            int fgCount = GetFlightgroupsCount(objectLines, optName);
            bool hasSkins = hasDefaultSkin || baseSkins.Count != 0 || fgCount != 0;

            int filePtr = 0;

            if (hasSkins)
            {
                var opt = OptFile.FromFile(optFilename);
                fgCount = Math.Max(fgCount, opt.MaxTextureVersion);
                UpdateOptFile(optName, opt, objectLines, baseSkins, fgCount, hasDefaultSkin);
                //opt.Save("temp.opt", false);

                int requiredSize = opt.GetSaveRequiredFileSize();
                IntPtr ptr = Marshal.AllocHGlobal(requiredSize);

                using (var stream = new UnmanagedMemoryStream((byte*)ptr, requiredSize, requiredSize, FileAccess.Write))
                {
                    opt.Save(stream, false);
                }

                filePtr = ptr.ToInt32();
            }

            //return hasSkins ? 1 : 0;
            return filePtr;
        }

        private static void UpdateOptFile(string optName, OptFile opt, IList<string> objectLines, IList<string> baseSkins, int fgCount, bool hasDefaultSkin)
        {
            List<List<string>> fgSkins = ReadFgSkins(optName, objectLines, baseSkins, fgCount);
            List<string> distinctSkins = fgSkins.SelectMany(t => t).Distinct(StringComparer.OrdinalIgnoreCase).ToList();
            ICollection<string> texturesExist = GetTexturesExist(optName, opt, distinctSkins);
            List<int> fgColors = GetFlightgroupsColors(objectLines, optName, fgCount, hasDefaultSkin);
            CreateSwitchTextures(opt, texturesExist, fgSkins, fgColors);
            UpdateSkins(optName, opt, distinctSkins, fgSkins);
        }

        private static List<List<string>> ReadFgSkins(string optName, IList<string> objectLines, IList<string> baseSkins, int fgCount)
        {
            var fgSkins = new List<List<string>>(fgCount);

            for (int i = 0; i < fgCount; i++)
            {
                var skins = new List<string>(baseSkins);
                string fgKey = optName + "_fgc_" + i.ToString(CultureInfo.InvariantCulture);
                skins.AddRange(XwaHooksConfig.Tokennize(XwaHooksConfig.GetFileKeyValue(objectLines, fgKey)));

                if (skins.Count == 0)
                {
                    skins.Add("Default");
                }

                fgSkins.Add(skins);
            }

            return fgSkins;
        }

        private static ICollection<string> GetTexturesExist(string optName, OptFile opt, List<string> distinctSkins)
        {
            var texturesExist = new SortedSet<string>();

            foreach (string skin in distinctSkins)
            {
                string path = GetSkinDirectoryLocatorPath(optName, skin);

                if (path == null)
                {
                    continue;
                }

                SortedSet<string> filesSet;

                using (IFileLocator locator = FileLocatorFactory.Create(path))
                {
                    if (locator == null)
                    {
                        continue;
                    }

                    var filesEnum = locator.EnumerateFiles()
                        .Select(t => Path.GetFileName(t));

                    filesSet = new SortedSet<string>(filesEnum, StringComparer.OrdinalIgnoreCase);
                }

                foreach (string textureName in opt.Textures.Keys)
                {
                    if (TextureExists(filesSet, textureName, skin) != null)
                    {
                        texturesExist.Add(textureName);
                    }
                }
            }

            return texturesExist;
        }

        private static void CreateSwitchTextures(OptFile opt, ICollection<string> texturesExist, List<List<string>> fgSkins, List<int> fgColors)
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

                foreach (int i in fgColors)
                {
                    texture.Value.Convert8To32();
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
                            if (fgColors.Contains(i))
                            {
                                faceGroup.Textures.Add(name + "_fg_" + i.ToString(CultureInfo.InvariantCulture) + "_" + string.Join(",", fgSkins[i]));
                            }
                            else
                            {
                                faceGroup.Textures.Add(name);
                            }
                        }
                    }
                }
            }
        }

        private static void UpdateSkins(string optName, OptFile opt, List<string> distinctSkins, List<List<string>> fgSkins)
        {
            var locatorsPath = new Dictionary<string, string>(distinctSkins.Count, StringComparer.OrdinalIgnoreCase);
            var filesSets = new Dictionary<string, SortedSet<string>>(distinctSkins.Count, StringComparer.OrdinalIgnoreCase);

            foreach (string skin in distinctSkins)
            {
                string path = GetSkinDirectoryLocatorPath(optName, skin);
                locatorsPath.Add(skin, path);

                SortedSet<string> filesSet = null;

                if (path != null)
                {
                    using (IFileLocator locator = FileLocatorFactory.Create(path))
                    {
                        if (locator != null)
                        {
                            var filesEnum = locator.EnumerateFiles()
                                .Select(t => Path.GetFileName(t));

                            filesSet = new SortedSet<string>(filesEnum, StringComparer.OrdinalIgnoreCase);
                        }
                    }
                }

                filesSets.Add(skin, filesSet ?? new SortedSet<string>());
            }

            opt.Textures.AsParallel().ForAll(texture =>
            {
                int position = texture.Key.IndexOf("_fg_");

                if (position == -1)
                {
                    return;
                }

                string textureName = texture.Key.Substring(0, position);
                int fgIndex = int.Parse(texture.Key.Substring(position + 4, texture.Key.IndexOf('_', position + 4) - position - 4), CultureInfo.InvariantCulture);

                foreach (string skin in fgSkins[fgIndex])
                {
                    string path = locatorsPath[skin];

                    if (path == null)
                    {
                        continue;
                    }

                    string filename = TextureExists(filesSets[skin], textureName, skin);

                    if (filename == null)
                    {
                        continue;
                    }

                    using (IFileLocator locator = FileLocatorFactory.Create(path))
                    {
                        if (locator == null)
                        {
                            continue;
                        }

                        CombineTextures(texture.Value, locator, filename);
                    }
                }

                texture.Value.GenerateMipmaps();
            });
        }

        private static void CombineTextures(Texture baseTexture, IFileLocator locator, string filename)
        {
            Texture newTexture;

            using (Stream file = locator.Open(filename))
            {
                newTexture = Texture.FromStream(file);
                newTexture.Name = Path.GetFileNameWithoutExtension(filename);
            }

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

        private static readonly string[] _textureExtensions = new string[] { ".bmp", ".png", ".jpg" };

        private static string TextureExists(ICollection<string> files, string baseFilename, string skin)
        {
            foreach (string ext in _textureExtensions)
            {
                string filename = baseFilename + "_" + skin + ext;

                if (files.Contains(filename))
                {
                    return filename;
                }
            }

            foreach (string ext in _textureExtensions)
            {
                string filename = baseFilename + ext;

                if (files.Contains(filename))
                {
                    return filename;
                }
            }

            return null;
        }

        [DllExport(CallingConvention.Cdecl)]
        unsafe public static void ReadCompressedDatImageFunction(byte* destination, int destinationLength, byte* source, int sourceLength)
        {
            byte[] coderProperties = new byte[5];
            Marshal.Copy(new IntPtr(source), coderProperties, 0, 5);

            using (var imageDecompressedStream = new UnmanagedMemoryStream(destination, destinationLength, destinationLength, FileAccess.Write))
            using (var imageStream = new UnmanagedMemoryStream(source + 5, sourceLength - 5, sourceLength - 5, FileAccess.Read))
            {
                var decoder = new SevenZip.Compression.LZMA.Decoder();
                decoder.SetDecoderProperties(coderProperties);
                decoder.Code(imageStream, imageDecompressedStream, sourceLength - 5, destinationLength, null);
            }
        }
    }
}
