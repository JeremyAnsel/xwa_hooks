using JeremyAnsel.Xwa.Opt;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace hook_32bpp_net
{
    public static class Main
    {
        private static bool _isD3dRendererHookEnabled = IsD3dRendererHookEnabled();

        private static bool IsD3dRendererHookEnabled()
        {
            IList<string> lines = XwaHooksConfig.GetFileLines("ddraw.cfg");
            bool isD3dRendererHookEnabled = XwaHooksConfig.GetFileKeyValueInt(lines, "D3dRendererHookEnabled", 1) != 0;
            return isD3dRendererHookEnabled;
        }

        private static IList<string> _getCustomFileLines_lines;
        private static string _getCustomFileLines_name;
        private static string _getCustomFileLines_mission;
        private static int _getCustomFileLines_missionIndex;
        private static string _getCustomFileLines_hangar;
        private static byte _getCustomFileLines_hangarIff;

        private static SevenZip.Compression.LZMA.Decoder _decoder;
        private static byte[] _decoderProperties;

        public static string GetMissionFileName()
        {
            int currentGameState = Marshal.ReadInt32(new IntPtr(0x09F60E0 + 0x25FA9));
            int updateCallback = Marshal.ReadInt32(new IntPtr(0x09F60E0 + 0x25FB1 + currentGameState * 0x850 + 0x844));
            bool isTechLibraryGameStateUpdate = updateCallback == 0x00574D70;

            if (isTechLibraryGameStateUpdate)
            {
                return string.Empty;
            }

            int missionDirectoryId = Marshal.ReadInt32(new IntPtr(0x00AE2A60 + 0x002A));
            string missionDirectory = Marshal.PtrToStringAnsi(new IntPtr(Marshal.ReadInt32(new IntPtr(0x00603168 + missionDirectoryId * 4))));
            int s_V0x09F5E74 = Marshal.ReadInt32(new IntPtr(0x009F5E74));
            int s_XwaMissionDescriptions = Marshal.ReadInt32(new IntPtr(0x009F4B98));

            string missionFilename;

            if (s_XwaMissionDescriptions != 0)
            {
                missionFilename = missionDirectory + "\\" + Marshal.PtrToStringAnsi(new IntPtr(s_XwaMissionDescriptions + s_V0x09F5E74 * 0x148 + 0x0000));
            }
            else
            {
                string xwaMissionFileName = Marshal.PtrToStringAnsi(new IntPtr(0x06002E8));
                missionFilename = xwaMissionFileName;
            }

            return missionFilename;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static int RetrieveMissionFileName()
        {
            string filename = GetMissionFileName();
            return Marshal.StringToHGlobalAnsi(filename).ToInt32();
        }

        public static IList<string> GetCustomFileLines(string name)
        {
            string xwaMissionFileName = GetMissionFileName();
            int xwaMissionFileNameIndex = Marshal.ReadInt32(new IntPtr(0x06002E4));
            int currentGameState = Marshal.ReadInt32(new IntPtr(0x09F60E0 + 0x25FA9));
            int updateCallback = Marshal.ReadInt32(new IntPtr(0x09F60E0 + 0x25FB1 + currentGameState * 0x850 + 0x844));
            bool isTechLibraryGameStateUpdate = updateCallback == 0x00574D70;
            string hangar = Marshal.PtrToStringAnsi(new IntPtr(0x00ABD680));
            byte hangarIff = Marshal.ReadByte(new IntPtr(0x00ABD680) + 255);

            if (isTechLibraryGameStateUpdate)
            {
                _getCustomFileLines_name = name;
                _getCustomFileLines_mission = null;
                _getCustomFileLines_missionIndex = 0;
                _getCustomFileLines_lines = XwaHooksConfig.GetFileLines("FlightModels\\" + name + ".txt");
                _getCustomFileLines_hangar = null;
                _getCustomFileLines_hangarIff = 0;

                if (_getCustomFileLines_lines.Count == 0)
                {
                    _getCustomFileLines_lines = XwaHooksConfig.GetFileLines("FlightModels\\default.ini", name);
                }
            }
            else
            {
                if (_getCustomFileLines_name != name
                    || _getCustomFileLines_mission != xwaMissionFileName
                    || _getCustomFileLines_missionIndex != xwaMissionFileNameIndex
                    || _getCustomFileLines_hangar != hangar
                    || _getCustomFileLines_hangarIff != hangarIff)
                {
                    _getCustomFileLines_name = name;
                    _getCustomFileLines_mission = xwaMissionFileName;
                    _getCustomFileLines_missionIndex = xwaMissionFileNameIndex;
                    _getCustomFileLines_hangar = hangar;
                    _getCustomFileLines_hangarIff = hangarIff;

                    string mission = XwaHooksConfig.GetStringWithoutExtension(xwaMissionFileName);
                    _getCustomFileLines_lines = XwaHooksConfig.GetFileLines(mission + "_" + name + ".txt");

                    if (_getCustomFileLines_lines.Count == 0)
                    {
                        _getCustomFileLines_lines = XwaHooksConfig.GetFileLines(mission + ".ini", name);
                    }

                    if (_getCustomFileLines_hangar != null && !_getCustomFileLines_hangar.EndsWith("\\"))
                    {
                        IList<string> hangarLines = new List<string>();

                        if (hangarLines.Count == 0)
                        {
                            hangarLines = XwaHooksConfig.GetFileLines(_getCustomFileLines_hangar + name + _getCustomFileLines_hangarIff + ".txt");
                        }

                        if (hangarLines.Count == 0)
                        {
                            hangarLines = XwaHooksConfig.GetFileLines(_getCustomFileLines_hangar + ".ini", name + _getCustomFileLines_hangarIff);
                        }

                        if (hangarLines.Count == 0)
                        {
                            hangarLines = XwaHooksConfig.GetFileLines(_getCustomFileLines_hangar + name + ".txt");
                        }

                        if (hangarLines.Count == 0)
                        {
                            hangarLines = XwaHooksConfig.GetFileLines(_getCustomFileLines_hangar + ".ini", name);
                        }

                        foreach (string line in hangarLines)
                        {
                            _getCustomFileLines_lines.Add(line);
                        }
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

        private static int GetFlightgroupsDefaultCount(string optName)
        {
            int count = 0;

            //for (int index = 255; index >= 0; index--)
            //{
            //    string skinName = "Default_" + index.ToString(CultureInfo.InvariantCulture);

            //    if (GetSkinDirectoryLocatorPath(optName, skinName) != null)
            //    {
            //        count = index + 1;
            //        break;
            //    }
            //}

            var locker = new object();
            var partition = Partitioner.Create(0, 256);

            Parallel.ForEach(
                partition,
                () => 0,
                (range, _, localValue) =>
                {
                    int localCount = 0;

                    for (int index = range.Item2 - 1; index >= range.Item1; index--)
                    {
                        string skinName = "Default_" + index.ToString(CultureInfo.InvariantCulture);

                        if (GetSkinDirectoryLocatorPath(optName, skinName) != null)
                        {
                            localCount = index + 1;
                            break;
                        }
                    }

                    return Math.Max(localCount, localValue);
                },
                localCount =>
                {
                    lock (locker)
                    {
                        if (localCount > count)
                        {
                            count = localCount;
                        }
                    }
                });

            return count;
        }

        private static int GetFlightgroupsCount(IList<string> objectLines, string optName)
        {
            int count = 0;

            //for (int index = 255; index >= 0; index--)
            //{
            //    string key = optName + "_fgc_" + index.ToString(CultureInfo.InvariantCulture);
            //    string value = XwaHooksConfig.GetFileKeyValue(objectLines, key);

            //    if (!string.IsNullOrEmpty(value))
            //    {
            //        count = index + 1;
            //        break;
            //    }
            //}

            var locker = new object();
            var partition = Partitioner.Create(0, 256);

            Parallel.ForEach(
                partition,
                () => 0,
                (range, _, localValue) =>
                {
                    int localCount = 0;

                    for (int index = range.Item2 - 1; index >= range.Item1; index--)
                    {
                        string key = optName + "_fgc_" + index.ToString(CultureInfo.InvariantCulture);
                        string value = XwaHooksConfig.GetFileKeyValue(objectLines, key);

                        if (!string.IsNullOrEmpty(value))
                        {
                            localCount = index + 1;
                            break;
                        }
                    }

                    return Math.Max(localCount, localValue);
                },
                localCount =>
                {
                    lock (locker)
                    {
                        if (localCount > count)
                        {
                            count = localCount;
                        }
                    }
                });

            return count;
        }

        private static List<int> GetFlightgroupsColors(IList<string> objectLines, string optName, int fgCount, bool hasDefaultSkin)
        {
            bool hasBaseSkins = hasDefaultSkin || !string.IsNullOrEmpty(XwaHooksConfig.GetFileKeyValue(objectLines, optName));

            var colors = new List<int>();

            //for (int index = 0; index < 256; index++)
            //{
            //    string key = optName + "_fgc_" + index.ToString(CultureInfo.InvariantCulture);
            //    string value = XwaHooksConfig.GetFileKeyValue(objectLines, key);

            //    if (!string.IsNullOrEmpty(value) || (hasBaseSkins && index < fgCount))
            //    {
            //        colors.Add(index);
            //    }
            //}

            var locker = new object();
            var partition = Partitioner.Create(0, 256);

            Parallel.ForEach(
                partition,
                () => new List<int>(),
                (range, _, localValue) =>
                {
                    for (int index = range.Item1; index < range.Item2; index++)
                    {
                        string key = optName + "_fgc_" + index.ToString(CultureInfo.InvariantCulture);
                        string value = XwaHooksConfig.GetFileKeyValue(objectLines, key);

                        if (!string.IsNullOrEmpty(value) || (hasBaseSkins && index < fgCount))
                        {
                            localValue.Add(index);
                        }
                    }

                    return localValue;
                },
                localCount =>
                {
                    lock (locker)
                    {
                        colors.AddRange(localCount);
                    }
                });

            return colors;
        }

        private static string GetSkinDirectoryLocatorPath(string optName, string skinName)
        {
            string[] skinNameParts = skinName.Split('-');
            skinName = skinNameParts[0];
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

            return null;
        }

        private static OptFile _tempOptFile;
        private static int _tempOptFileSize;

        [DllExport(CallingConvention.Cdecl)]
        public static int ReadOptFunction([MarshalAs(UnmanagedType.LPStr)] string optFilename)
        {
            _tempOptFile = null;
            _tempOptFileSize = 0;

            if (!File.Exists(optFilename))
            {
                return 0;
            }

            string optName = Path.GetFileNameWithoutExtension(optFilename);

            var opt = OptFile.FromFile(optFilename, false);

            if (Directory.Exists($"FlightModels\\Skins\\{optName}"))
            {
                IList<string> objectLines = GetCustomFileLines("Skins");
                IList<string> baseSkins = XwaHooksConfig.Tokennize(XwaHooksConfig.GetFileKeyValue(objectLines, optName));
                bool hasDefaultSkin = GetSkinDirectoryLocatorPath(optName, "Default") != null || GetFlightgroupsDefaultCount(optName) != 0;
                int fgCount = GetFlightgroupsCount(objectLines, optName);
                bool hasSkins = hasDefaultSkin || baseSkins.Count != 0 || fgCount != 0;

                if (hasSkins)
                {
                    fgCount = Math.Max(fgCount, opt.MaxTextureVersion);
                    fgCount = Math.Max(fgCount, GetFlightgroupsDefaultCount(optName));
                    UpdateOptFile(optName, opt, objectLines, baseSkins, fgCount, hasDefaultSkin);
                }
            }

            if (_isD3dRendererHookEnabled)
            {
                opt.GroupFaceGroups();
            }

            _tempOptFile = opt;
            _tempOptFileSize = opt.GetSaveRequiredFileSize(false);

            return _tempOptFileSize;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static int GetOptVersionFunction()
        {
            if (_tempOptFile == null)
            {
                return 0;
            }

            return _tempOptFile.Version;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static unsafe void WriteOptFunction(IntPtr ptr)
        {
            if (ptr == IntPtr.Zero || _tempOptFile == null || _tempOptFileSize == 0)
            {
                _tempOptFile = null;
                _tempOptFileSize = 0;
                return;
            }

            using (var stream = new UnmanagedMemoryStream((byte*)ptr, _tempOptFileSize, _tempOptFileSize, FileAccess.Write))
            {
                _tempOptFile.Save(stream, false, false, false);
            }

            _tempOptFile = null;
            _tempOptFileSize = 0;
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
                    string skinName = "Default_" + i.ToString(CultureInfo.InvariantCulture);

                    if (GetSkinDirectoryLocatorPath(optName, skinName) != null)
                    {
                        skins.Add(skinName);
                    }
                    else
                    {
                        skins.Add("Default");
                    }
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

                string[] filenames;

                if (path.EndsWith(".zip", StringComparison.OrdinalIgnoreCase))
                {
                    using ZipArchive zip = ZipFile.OpenRead(path);
                    ZipArchiveEntry[] files = zip.Entries.ToArray();
                    filenames = Array.ConvertAll(files, t => t.Name);
                }
                else
                {
                    string[] files = Directory.GetFiles(path);
                    filenames = Array.ConvertAll(files, t => Path.GetFileName(t));
                }

                SortedSet<string> filesSet = new(filenames, StringComparer.OrdinalIgnoreCase);

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

            var newTextures = new ConcurrentBag<Texture>();

            foreach (var texture in opt.Textures.Where(texture => texturesExist.Contains(texture.Key)))
            {
                //texture.Value.Convert8To32(false);

                foreach (int i in fgColors)
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

            opt.Meshes
                .SelectMany(t => t.Lods)
                .SelectMany(t => t.FaceGroups)
                .AsParallel()
                .ForAll(faceGroup =>
                {
                    if (faceGroup.Textures.Count == 0)
                    {
                        return;
                    }

                    string name = faceGroup.Textures[0];

                    if (!texturesExist.Contains(name))
                    {
                        return;
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
                });
        }

        private static void UpdateSkins(string optName, OptFile opt, List<string> distinctSkins, List<List<string>> fgSkins)
        {
            var locatorsPath = new ConcurrentDictionary<string, string>(StringComparer.OrdinalIgnoreCase);
            var filesSets = new ConcurrentDictionary<string, SortedSet<string>>(StringComparer.OrdinalIgnoreCase);

            distinctSkins.AsParallel().ForAll(skin =>
            {
                string path = GetSkinDirectoryLocatorPath(optName, skin);
                locatorsPath[skin] = path;

                SortedSet<string> filesSet = null;

                if (path != null)
                {
                    string[] filenames;

                    if (path.EndsWith(".zip", StringComparison.OrdinalIgnoreCase))
                    {
                        using ZipArchive zip = ZipFile.OpenRead(path);
                        ZipArchiveEntry[] files = zip.Entries.ToArray();
                        filenames = Array.ConvertAll(files, t => t.Name);
                    }
                    else
                    {
                        string[] files = Directory.GetFiles(path);
                        filenames = Array.ConvertAll(files, t => Path.GetFileName(t));
                    }

                    filesSet = new(filenames, StringComparer.OrdinalIgnoreCase);
                }

                filesSets[skin] = filesSet ?? new SortedSet<string>();
            });

            foreach (var texture in opt.Textures.Where(texture => texture.Key.IndexOf("_fg_") != -1))
            {
                int position = texture.Key.IndexOf("_fg_");

                if (position == -1)
                {
                    continue;
                }

                texture.Value.Convert8To32(false);

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

                    Stream file = null;
                    ZipArchive zip = null;

                    try
                    {
                        if (path.EndsWith(".zip", StringComparison.OrdinalIgnoreCase))
                        {
                            zip = ZipFile.OpenRead(path);
                            //file = zip.GetEntry(filename)!.Open();

                            foreach (ZipArchiveEntry entry in zip.Entries)
                            {
                                if (string.Equals(entry.Name, filename, StringComparison.OrdinalIgnoreCase))
                                {
                                    file = entry.Open();
                                    break;
                                }
                            }
                        }
                        else
                        {
                            file = File.OpenRead(Path.Combine(path, filename));
                        }

                        CombineTextures(texture.Value, file, filename, skin);
                    }
                    finally
                    {
                        file?.Dispose();
                        zip?.Dispose();
                    }
                }

                texture.Value.GenerateMipmaps();
            }
        }

        private static void CombineTextures(Texture baseTexture, Stream file, string filename, string skin)
        {
            string[] skinParts = skin.Split('-');
            skin = skinParts[0];
            int skinOpacity = 100;
            if (skinParts.Length > 1 && int.TryParse(skinParts[1], NumberStyles.Integer, CultureInfo.InvariantCulture, out int opacity))
            {
                opacity = Math.Max(0, Math.Min(100, opacity));
                skinOpacity = opacity;
            }

            Texture newTexture;

            newTexture = Texture.FromStream(file);
            newTexture.Name = Path.GetFileNameWithoutExtension(filename);

            if (newTexture.Width != baseTexture.Width || newTexture.Height != baseTexture.Height)
            {
                return;
            }

            newTexture.Convert8To32(false);
            FlipPixels(newTexture.ImageData, newTexture.Width, newTexture.Height, 32);

            int size = baseTexture.Width * baseTexture.Height;
            byte[] src = newTexture.ImageData;
            byte[] dst = baseTexture.ImageData;

            //for (int i = 0; i < size; i++)
            //{
            //    int a = src[i * 4 + 3];

            //    dst[i * 4 + 0] = (byte)(dst[i * 4 + 0] * (255 - a) / 255 + src[i * 4 + 0] * a / 255);
            //    dst[i * 4 + 1] = (byte)(dst[i * 4 + 1] * (255 - a) / 255 + src[i * 4 + 1] * a / 255);
            //    dst[i * 4 + 2] = (byte)(dst[i * 4 + 2] * (255 - a) / 255 + src[i * 4 + 2] * a / 255);
            //}

            var partition = Partitioner.Create(0, size);

            Parallel.ForEach(partition, range =>
            {
                for (int i = range.Item1; i < range.Item2; i++)
                {
                    int a = src[i * 4 + 3];

                    a = (a * skinOpacity + 50) / 100;

                    dst[i * 4 + 0] = (byte)(dst[i * 4 + 0] * (255 - a) / 255 + src[i * 4 + 0] * a / 255);
                    dst[i * 4 + 1] = (byte)(dst[i * 4 + 1] * (255 - a) / 255 + src[i * 4 + 1] * a / 255);
                    dst[i * 4 + 2] = (byte)(dst[i * 4 + 2] * (255 - a) / 255 + src[i * 4 + 2] * a / 255);
                }
            });
        }

        private static void FlipPixels(byte[] pixels, int width, int height, int bpp)
        {
            int length = pixels.Length;
            int offset = 0;
            int w = width;
            int h = height;

            while (offset < length)
            {
                int stride = w * bpp / 8;

                for (int i = 0; i < h / 2; i++)
                {
                    for (int j = 0; j < stride; j++)
                    {
                        byte v = pixels[offset + i * stride + j];
                        pixels[offset + i * stride + j] = pixels[offset + (h - 1 - i) * stride + j];
                        pixels[offset + (h - 1 - i) * stride + j] = v;
                    }
                }

                offset += h * stride;

                w = w > 1 ? w / 2 : 1;
                h = h > 1 ? h / 2 : 1;
            }
        }

        private static readonly string[] _textureExtensions = new string[] { ".bmp", ".png", ".jpg" };

        private static string TextureExists(ICollection<string> files, string baseFilename, string skin)
        {
            string[] skinParts = skin.Split('-');
            skin = skinParts[0];

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
            if (_decoder == null)
            {
                _decoder = new SevenZip.Compression.LZMA.Decoder();
            }

            byte[] coderProperties = new byte[5];
            Marshal.Copy(new IntPtr(source), coderProperties, 0, 5);

            if (_decoderProperties == null
                || _decoderProperties[0] != coderProperties[0]
                || _decoderProperties[1] != coderProperties[1]
                || _decoderProperties[2] != coderProperties[2]
                || _decoderProperties[3] != coderProperties[3]
                || _decoderProperties[4] != coderProperties[4])
            {
                _decoderProperties = coderProperties;
                _decoder.SetDecoderProperties(coderProperties);
            }

            using var imageDecompressedStream = new UnmanagedMemoryStream(destination, destinationLength, destinationLength, FileAccess.Write);
            using var imageStream = new UnmanagedMemoryStream(source + 5, sourceLength - 5, sourceLength - 5, FileAccess.Read);
            _decoder.Code(imageStream, imageDecompressedStream, sourceLength - 5, destinationLength, null);
        }
    }
}
