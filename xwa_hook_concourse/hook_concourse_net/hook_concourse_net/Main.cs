using JeremyAnsel.Xwa.Cbm;
using JeremyAnsel.Xwa.Dat;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace hook_concourse_net
{
    public static class Main
    {
        private static readonly Encoding _encoding = Encoding.GetEncoding("iso-8859-1");

        private static bool _resItemsDatLoaded = false;
        private static readonly List<ResItemDescription> _resItemsDat = new();
        private static readonly Dictionary<string, ResItem> _resItems = new();
        private static readonly Dictionary<string, DatFile> _datFiles = new();

        private static bool _backgroundsWideLoaded = false;
        private static IList<string> _backgroundsWideLines;

        private static bool _dcSet = false;
        private static SurfaceDC _dc = default;

        private static ResItemDescription GetResItemDescriptionDat(int groupId, int imageId)
        {
            return _resItemsDat
                .Where(t => t.GroupId == groupId && t.ImageId == imageId)
                .FirstOrDefault();
        }

        private static void SortDatFile(DatFile dat)
        {
            var orderedGroups = dat.Groups
                .OrderBy(t => t.GroupId)
                .ToList();

            dat.Groups.Clear();

            foreach (var group in orderedGroups)
            {
                dat.Groups.Add(group);

                var orderedImages = group.Images
                    .OrderBy(t => t.GroupId)
                    .ThenBy(t => t.ImageId)
                    .ToList();

                group.Images.Clear();

                foreach (var image in orderedImages)
                {
                    group.Images.Add(image);
                }
            }
        }

        private static void LoadDatFiles()
        {
            if (_resItemsDatLoaded)
            {
                return;
            }

            _resItemsDatLoaded = true;

            string[] resLines = File.ReadAllLines("Resdata.txt", _encoding);

            string[] resLinesFilter = new string[]
            {
                "Resdata\\LightingEffects.dat",
                "Resdata\\Backdrop.dat",
                "Resdata\\Galaxy.dat",
                "Resdata\\GalaxyTFTC.dat",
                "Resdata\\Nebula.dat",
                "Resdata\\NebulaTFTC.dat",
                "Resdata\\Planet.dat",
                "Resdata\\Planet2.dat",
                "Resdata\\PlanetTFTC.dat",
                "Resdata\\Explosions.dat",
                "Resdata\\Sparks.dat",
                "Resdata\\Chaff.dat",
                "Resdata\\Debris.dat",
                "Resdata\\Animations.dat",
                "Resdata\\HUD.dat",
                //"Resdata\\Map_Icons.dat",
                //"Resdata\\Map_Icons_Green.dat",
                //"Resdata\\Map_Icons_Red.dat",
                //"Resdata\\Map_Icons_Yellow.dat",
                //"Resdata\\Map_Icons_Blue.dat",
                //"Resdata\\Map_Icons_Purple.dat",
                "Resdata\\Hull_Icons.dat",
                "Resdata\\Hull_Icons_White.dat",
                "Resdata\\Hull_Icons_Green.dat",
                "Resdata\\Hull_Icons_Yellow.dat",
                "Resdata\\Hull_Icons_Red.dat",
                //"Resdata\\Craft_Icons.dat",
                //"Resdata\\Fonts.dat",
                "Resdata\\BlastMarks.dat",
                "Resdata\\DeathStarII.dat",
                "Resdata\\WrapBack.dat",
                "Resdata\\DSFire.dat",
                "Resdata\\Trails.dat",
                "Resdata\\Particles.dat",
                //"Resdata\\FRONTEND.dat",
                //"Resdata\\FRONTEND2.dat",
                //"Resdata\\shipstuff1.dat",
                //"Resdata\\shipstuff2.dat",
                //"Resdata\\shipstuff3.dat",
                //"Resdata\\shipstuff4.dat",
                //"Resdata\\frontplanet.dat",
                //"Resdata\\cursor.dat",
            };

            for (int i = 0; i < resLines.Length; i++)
            {
                if (resLinesFilter.Contains(resLines[i], StringComparer.OrdinalIgnoreCase))
                {
                    resLines[i] = null;
                }
            }

            foreach (string filename in resLines)
            {
                if (string.IsNullOrWhiteSpace(filename) || !File.Exists(filename))
                {
                    continue;
                }

                DatFile dat = DatFile.FromFile(filename, false);
                SortDatFile(dat);

                foreach (DatImage datImage in dat.Images)
                {
                    ResItemDescription description = new()
                    {
                        GroupId = datImage.GroupId,
                        ImageId = datImage.ImageId,
                        Width = datImage.Width,
                        Height = datImage.Height,
                        OffsetX = datImage.OffsetX,
                        OffsetY = datImage.OffsetY,
                        Filename = $"{filename}"
                    };

                    _resItemsDat.Add(description);
                }
            }
        }

        [DllExport(CallingConvention.Cdecl)]
        public static void LoadDatImage(
            [MarshalAs(UnmanagedType.LPStr)] string name,
            int groupId,
            int imageId,
            int count)
        {
            LoadDatFiles();

            _resItems.Remove(name);

            ResItemDescription descriptionSD = GetResItemDescriptionDat(groupId, imageId);

            if (descriptionSD is null)
            {
                return;
            }

            string filenameHD = descriptionSD.Filename.Substring(0, descriptionSD.Filename.Length - 4) + "_hd.dat";

            if (!File.Exists(filenameHD))
            {
                filenameHD = descriptionSD.Filename;
            }

            if (!_datFiles.TryGetValue(filenameHD, out DatFile datHD))
            {
                datHD = DatFile.FromFile(filenameHD, false);
                SortDatFile(datHD);
                _datFiles[filenameHD] = datHD;
            }

            DatGroup datGroupHD = datHD.Groups.FirstOrDefault(t => t.GroupId == groupId);

            if (datGroupHD is null)
            {
                return;
            }

            List<DatImage> datImagesHD = datGroupHD.Images
                .SkipWhile(t => t.ImageId != imageId)
                .Take(count)
                .ToList();

            if (datImagesHD.Count == 0)
            {
                return;
            }

            ResItem item = new()
            {
                DescriptionSD = descriptionSD
            };

            foreach (DatImage datImage in datImagesHD)
            {
                ResItemDescription description = GetResItemDescriptionDat(datImage.GroupId, datImage.ImageId);

                string source = $"{descriptionSD.Filename}-{datImage.GroupId}-{datImage.ImageId}";

                var image = new ResItemImage(source, description, datImage, name);
                item.ImagesHD.Add(image);
            }

            _resItems[name] = item;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static void LoadCbmImage(
            [MarshalAs(UnmanagedType.LPStr)] string name,
            [MarshalAs(UnmanagedType.LPStr)] string filename)
        {
            _resItems.Remove(name);

            if (!File.Exists(filename))
            {
                return;
            }

            CbmFile cbmSD = CbmFile.FromFile(filename);

            ResItemDescription descriptionSD = new()
            {
                GroupId = cbmSD.GroupId,
                ImageId = cbmSD.ImageId,
                Width = cbmSD.Width,
                Height = cbmSD.Height,
                OffsetX = cbmSD.AreaLeft,
                OffsetY = cbmSD.AreaTop,
                Filename = filename
            };

            string filenameHDDat = filename.Substring(0, filename.Length - 4) + "_hd.dat";
            string filenameHDCbm = filename.Substring(0, filename.Length - 4) + "_hd.cbm";

            ResItem item;

            if (File.Exists(filenameHDDat))
            {
                DatFile datHD = DatFile.FromFile(filenameHDDat, false);
                SortDatFile(datHD);
                List<DatImage> datImages = datHD.Images.ToList();

                if (datImages.Count == 0 || cbmSD.Images.Count == 0)
                {
                    return;
                }

                item = new()
                {
                    DescriptionSD = descriptionSD
                };

                for (int index = 0; index < datImages.Count; index++)
                {
                    DatImage datImage = datImages[index];
                    CbmImage cbmImage = index < cbmSD.Images.Count ? cbmSD.Images[index] : cbmSD.Images[0];

                    ResItemDescription description = new()
                    {
                        GroupId = 0,
                        ImageId = 0,
                        Width = cbmImage.Width,
                        Height = cbmImage.Height,
                        OffsetX = cbmImage.OffsetX,
                        OffsetY = cbmImage.OffsetY,
                        Filename = descriptionSD.Filename
                    };

                    var image = new ResItemImage(descriptionSD.Filename, description, datImage, name);
                    item.ImagesHD.Add(image);
                }
            }
            else
            {
                if (!File.Exists(filenameHDCbm))
                {
                    filenameHDCbm = filename;
                }

                CbmFile cbmHD = CbmFile.FromFile(filenameHDCbm);

                if (cbmHD.Images.Count == 0 || cbmSD.Images.Count == 0)
                {
                    return;
                }

                item = new()
                {
                    DescriptionSD = descriptionSD
                };

                for (int index = 0; index < cbmHD.Images.Count; index++)
                {
                    CbmImage cbmImage = cbmHD.Images[index];
                    CbmImage cbmImageSD = index < cbmSD.Images.Count ? cbmSD.Images[index] : cbmSD.Images[0];

                    ResItemDescription description = new()
                    {
                        GroupId = 0,
                        ImageId = 0,
                        Width = cbmImageSD.Width,
                        Height = cbmImageSD.Height,
                        OffsetX = cbmImageSD.OffsetX,
                        OffsetY = cbmImageSD.OffsetY,
                        Filename = descriptionSD.Filename
                    };

                    var image = new ResItemImage(descriptionSD.Filename, description, cbmImage, name);
                    item.ImagesHD.Add(image);
                }
            }

            _resItems[name] = item;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static void FreeImage(
            [MarshalAs(UnmanagedType.LPStr)] string name)
        {
            _resItems.Remove(name);
        }

        [DllExport(CallingConvention.Cdecl)]
        public static int GetItemImagesCount(
            [MarshalAs(UnmanagedType.LPStr)] string name)
        {
            if (_resItems.TryGetValue(name, out ResItem item))
            {
                return item.ImagesHD.Count;
            }

            return 0;
        }

        [DllExport(CallingConvention.Cdecl)]
        public unsafe static int GetItemImageDimensions(
            [MarshalAs(UnmanagedType.LPStr)] string name,
            int* widthSDPtr,
            int* heightSDPtr,
            int* widthHDPtr,
            int* heightHDPtr)
        {
            if (!_resItems.TryGetValue(name, out ResItem item))
            {
                return -1;
            }

            int widthSD = item.ImagesHD[0].DescriptionSD.Width;
            int heightSD = item.ImagesHD[0].DescriptionSD.Height;
            int widthHD = item.ImagesHD[0].DescriptionHD.Width;
            int heightHD = item.ImagesHD[0].DescriptionHD.Height;

            if (widthSDPtr != null && heightSDPtr != null)
            {
                *widthSDPtr = widthSD;
                *heightSDPtr = heightSD;
            }

            if (widthHDPtr != null && heightHDPtr != null)
            {
                *widthHDPtr = widthHD;
                *heightHDPtr = heightHD;
            }

            return 0;
        }

        [DllExport(CallingConvention.Cdecl)]
        public unsafe static int GetItemImageRawData(
            [MarshalAs(UnmanagedType.LPStr)] string name,
            int bufferPtr)
        {
            if (!_resItems.TryGetValue(name, out ResItem item))
            {
                return -1;
            }

            int widthSD = item.ImagesHD[0].DescriptionSD.Width;
            int heightSD = item.ImagesHD[0].DescriptionSD.Height;
            int widthHD = item.ImagesHD[0].DescriptionHD.Width;
            int heightHD = item.ImagesHD[0].DescriptionHD.Height;
            byte[] dataHD = item.ImagesHD[0].GetImageData32();

            GCHandle handle = GCHandle.Alloc(dataHD, GCHandleType.Pinned);

            try
            {
                using var bitmapHD = new Bitmap(widthHD, heightHD, widthHD * 4, PixelFormat.Format32bppArgb, handle.AddrOfPinnedObject());
                using var bitmapSD = new Bitmap(bitmapHD, new Size(widthSD, heightSD));
                var rect = new Rectangle(Point.Empty, bitmapSD.Size);

                BitmapData lockBits = bitmapSD.LockBits(rect, ImageLockMode.ReadOnly, bitmapSD.PixelFormat);

                try
                {
                    if (lockBits.Stride == lockBits.Width * 4)
                    {
                        Buffer.MemoryCopy(lockBits.Scan0.ToPointer(), new IntPtr(bufferPtr).ToPointer(), widthSD * heightSD * 4, widthSD * heightSD * 4);
                    }
                    else
                    {
                        IntPtr src = lockBits.Scan0;
                        IntPtr dst = new(bufferPtr);

                        for (int y = 0; y < heightSD; y++)
                        {
                            Buffer.MemoryCopy(src.ToPointer(), dst.ToPointer(), widthSD * 4, widthSD * 4);
                            src += lockBits.Stride;
                            dst += widthSD * 4;
                        }
                    }
                }
                finally
                {
                    bitmapSD.UnlockBits(lockBits);
                }
            }
            finally
            {
                handle.Free();
            }

            return 0;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static int FrontResDrawItemImage(
            [MarshalAs(UnmanagedType.LPStr)] string name,
            int positionX,
            int positionY,
            int currentImage,
            int dcPtr,
            int originReturnAddress)
        {
            var dc = new SurfaceDC(new IntPtr(dcPtr));

            if (_resItems.TryGetValue(name, out ResItem item))
            {
                DrawItem(name, item, positionX, positionY, currentImage, null, 0, 0, dc, originReturnAddress);
                return 1;
            }

            return 0;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static int FrontResDrawItemCurrentImage(
            [MarshalAs(UnmanagedType.LPStr)] string name,
            int positionX,
            int positionY,
            int currentImage,
            int dcPtr,
            int originReturnAddress)
        {
            var dc = new SurfaceDC(new IntPtr(dcPtr));

            if (_resItems.TryGetValue(name, out ResItem item))
            {
                DrawItem(name, item, positionX, positionY, currentImage, null, 0, 0, dc, originReturnAddress);
                return 1;
            }

            return 0;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static int FrontResDrawMapIcon(
            [MarshalAs(UnmanagedType.LPStr)] string name,
            int srcLeft,
            int srcTop,
            int srcRight,
            int srcBottom,
            int positionX,
            int positionY,
            int rotation,
            int dcPtr,
            int originReturnAddress)
        {
            var dc = new SurfaceDC(new IntPtr(dcPtr));

            if (_resItems.TryGetValue(name, out ResItem item))
            {
                DrawItem(name, item, positionX, positionY, 0, Rectangle.FromLTRB(srcLeft, srcTop, srcRight, srcBottom), rotation, 0, dc, originReturnAddress);
                return 1;
            }

            return 0;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static int FrontResDrawBlendColor(
            [MarshalAs(UnmanagedType.LPStr)] string name,
            int srcLeft,
            int srcTop,
            int srcRight,
            int srcBottom,
            int positionX,
            int positionY,
            int rotation,
            uint blendColor,
            int dcPtr,
            int originReturnAddress)
        {
            var dc = new SurfaceDC(new IntPtr(dcPtr));

            if (_resItems.TryGetValue(name, out ResItem item))
            {
                positionX += item.ImagesHD[0].DescriptionHD.OffsetX;
                positionY += item.ImagesHD[0].DescriptionHD.OffsetY;

                DrawItem(name, item, positionX, positionY, 0, Rectangle.FromLTRB(srcLeft, srcTop, srcRight, srcBottom), rotation, blendColor, dc, originReturnAddress);
                return 1;
            }

            return 0;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static int DrawBackgroundToScreen()
        {
            NativeMethods.DEVMODE mode = default;
            mode.dmSize = (short)Marshal.SizeOf(typeof(NativeMethods.DEVMODE));
            NativeMethods.EnumDisplaySettings(null, NativeMethods.ENUM_CURRENT_SETTINGS, ref mode);

            var dc = new SurfaceDC(mode.dmPelsWidth, mode.dmPelsHeight);
            string name = "background";

            if (_resItems.TryGetValue(name, out ResItem item))
            {
                DrawItem(name, item, 0, 0, 0, null, 0, 0, dc, 0);
            }

            return 0;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static int FrontResGetArea(
            [MarshalAs(UnmanagedType.LPStr)] string name,
            int pArea)
        {
            if (_resItems.TryGetValue(name, out ResItem item))
            {
                if (item.DescriptionSD.GroupId != 15900)
                {
                    return 0;
                }

                IntPtr area = new(pArea);

                ResItemImage image = item.ImagesHD[0];
                bool isBackgroundHD = IsBackgroundHD(name) != 0;
                int backgroundWidth = GetBackgroundWidth();

                bool isNewImage = image.DescriptionSD.Width != image.DescriptionHD.Width || image.DescriptionSD.Height != image.DescriptionHD.Height;
                bool isSameAspectRatio = Math.Abs(((float)image.DescriptionSD.Width / image.DescriptionSD.Height) - ((float)image.DescriptionHD.Width / image.DescriptionHD.Height)) < 0.1f;

                int destinationX = isBackgroundHD ? MultiplyDiv(image.DescriptionHD.OffsetX, backgroundWidth, _dc.DisplayWidth) : MultiplyDiv(image.DescriptionHD.OffsetX, _dc.Height, _dc.DisplayHeight);
                int destinationY = MultiplyDiv(image.DescriptionHD.OffsetY, _dc.Height, _dc.DisplayHeight);

                int destinationWidth;
                int destinationHeight;

                if (isSameAspectRatio)
                {
                    destinationWidth = isBackgroundHD ? MultiplyDiv(image.DescriptionSD.Width, backgroundWidth, _dc.DisplayWidth) : MultiplyDiv(image.DescriptionSD.Width, _dc.Height, _dc.DisplayHeight);
                    destinationHeight = MultiplyDiv(image.DescriptionSD.Height, _dc.Height, _dc.DisplayHeight);
                }
                else
                {
                    destinationHeight = MultiplyDiv(image.DescriptionSD.Height, _dc.Height, _dc.DisplayHeight);
                    destinationWidth = MultiplyDiv(image.DescriptionHD.Width, destinationHeight, image.DescriptionHD.Height);
                }

                int x;
                int y;
                int w;
                int h;

                if (isBackgroundHD)
                {
                    x = MultiplyDiv(destinationX, _dc.DisplayWidth, backgroundWidth);
                    y = MultiplyDiv(destinationY, _dc.DisplayHeight, _dc.Height);
                    w = MultiplyDiv(destinationWidth, _dc.DisplayWidth, backgroundWidth);
                    h = MultiplyDiv(destinationHeight, _dc.DisplayHeight, _dc.Height);
                }
                else
                {
                    x = MultiplyDiv(destinationX, _dc.DisplayHeight, _dc.Height);
                    y = MultiplyDiv(destinationY, _dc.DisplayHeight, _dc.Height);
                    w = MultiplyDiv(destinationWidth, _dc.DisplayHeight, _dc.Height);
                    h = MultiplyDiv(destinationHeight, _dc.DisplayHeight, _dc.Height);
                }

                Marshal.WriteInt32(area + 0, x);
                Marshal.WriteInt32(area + 4, y);
                Marshal.WriteInt32(area + 8, x + w);
                Marshal.WriteInt32(area + 12, y + h);

                return 1;
            }

            return 0;
        }

        private static readonly string[] _backgroundNames = new string[]
        {
            "backname",
            "backconfig",
            "concourse",
            "background",
            "briefstart",
            "globe",
            "techroom",
            "cerecase",
        };

        public static bool IsBackgroundName(string name)
        {
            return _backgroundNames.Contains(name);
        }

        public static ResItemImage GetBackgroundImage()
        {
            foreach (string name in _backgroundNames)
            {
                if (_resItems.TryGetValue(name, out ResItem item))
                {
                    if (item.ImagesHD.Count == 0)
                    {
                        break;
                    }

                    ResItemImage image = item.ImagesHD[0];

                    if (image.DescriptionSD == null || image.DescriptionHD == null)
                    {
                        break;
                    }

                    return image;
                }
            }

            return null;
        }

        public static int IsBackgroundWide()
        {
            if (!_backgroundsWideLoaded)
            {
                _backgroundsWideLoaded = true;
                _backgroundsWideLines = XwaHooksConfig.GetFileLines("ConcourseBackgroundsWide.txt");
                _backgroundsWideLines.Add(@"FRONTRES\CONFIG\DPMED.CBM = 0");
                _backgroundsWideLines.Add(@"FRONTRES\CONFIG\CONFIGB.CBM = 0");
            }

            ResItemImage image = GetBackgroundImage();

            if (image is null)
            {
                return 0;
            }

            if (image.IsSameAspectRatio())
            {
                return 0;
            }

            int isWide = XwaHooksConfig.GetFileKeyValueInt(_backgroundsWideLines, image.Source, 1);
            return isWide;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static int FrontResIsBackgroundWide()
        {
            return IsBackgroundWide();
        }

        public static int IsBackgroundHD(string imageName)
        {
            ResItemImage image = GetBackgroundImage();

            if (image is null)
            {
                return 0;
            }

            if (image.IsSameAspectRatio())
            {
                return 0;
            }

            if (image.IsNewImage())
            {
                return 1;
            }

            return 0;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static int FrontResIsBackgroundHD()
        {
            return IsBackgroundHD(string.Empty);
        }

        private enum Origin
        {
            Unknown,
            DrawImage,
            DrawCurrentImage,
            DrawCurrentImageAdd,
            DrawMapIcon,
            DrawGreyMapIcon,
            DrawIcon,
        }

        private static Origin GetOrigin(int originReturnAddress)
        {
            return originReturnAddress switch
            {
                0x534A6A => Origin.DrawImage,
                0x53547A => Origin.DrawCurrentImage,
                0x53235A => Origin.DrawCurrentImageAdd,
                0x532C9A => Origin.DrawMapIcon,
                0x533BFA => Origin.DrawGreyMapIcon,
                0x5338CA => Origin.DrawIcon,
                _ => Origin.Unknown
            };
        }

        private static Rectangle RestrictImage(Rectangle src, ResItemImage image)
        {
            return Rectangle.FromLTRB(
                Math.Min(Math.Max(src.Left, 0), image.DescriptionHD.Width - 1),
                Math.Min(Math.Max(src.Top, 0), image.DescriptionHD.Height - 1),
                Math.Min(Math.Max(src.Right, 0), image.DescriptionHD.Width - 1),
                Math.Min(Math.Max(src.Bottom, 0), image.DescriptionHD.Height - 1));
        }

        private static int MultiplyDivRound(int value)
        {
            return (value > 8000) ? ((value - 8000 + 5) / 10) : value;
        }

        private static int MultiplyDivOffset(int value, int offset)
        {
            return (value > 8000) ? (value + offset * 10) : (value + offset);
        }

        private static int MultiplyDiv(int v0, int a, int b)
        {
            float f = v0 > 8000 ? (v0 - 8000) / 10.0f : v0;

            return (int)((f * a + b / 2) / b);
        }

        private static int MultiplyDiv(int v0, int v1, int a, int b)
        {
            float f0 = v0 > 8000 ? (v0 - 8000) / 10.0f : v0;
            float f1 = v1 > 8000 ? (v1 - 8000) / 10.0f : v1;

            return (int)(((f0 + f1) * a + b / 2) / b);
        }

        private static int GetBackgroundWidth()
        {
            ResItemImage image = GetBackgroundImage();

            if (image is null)
            {
                return 0;
            }

            bool isNewImage = image.DescriptionSD.Width != image.DescriptionHD.Width || image.DescriptionSD.Height != image.DescriptionHD.Height;

            if (!isNewImage)
            {
                return 0;
            }

            int destinationWidth = MultiplyDiv(image.DescriptionHD.Width, _dc.Height, image.DescriptionHD.Height);
            return destinationWidth;
        }

        private static void DrawItem(string name, ResItem item, int positionX, int positionY, int currentImage, Rectangle? srcRect, int rotation, uint blendColor, in SurfaceDC dc, int originReturnAddress)
        {
            if (dc.ThisPtr != IntPtr.Zero && !_dcSet)
            {
                _dc = dc;
                _dcSet = true;
            }

            Origin origin = GetOrigin(originReturnAddress);
            bool isBackground = IsBackgroundName(name);
            bool isBackgroundHD = IsBackgroundHD(name) != 0;
            bool isBackgroundWide = IsBackgroundWide() != 0;
            int backgroundWidth = GetBackgroundWidth();
            int backgroundOffsetX = (dc.Width - backgroundWidth) / 2;

            if (currentImage < 0 || currentImage >= item.ImagesHD.Count)
            {
                return;
            }

            if (origin == Origin.DrawCurrentImageAdd && name == "topframe")
            {
                positionY = MultiplyDivOffset(positionY, 1);
            }

            if (origin == Origin.DrawCurrentImageAdd && name == "leftframe")
            {
                positionX = MultiplyDivOffset(positionX, -5);
            }

            if (origin == Origin.DrawImage && name == "sbarcenter")
            {
                positionY = MultiplyDivOffset(positionY, 2);
            }

            ResItemImage image = item.ImagesHD[currentImage];

            int x;
            int y;

            if (image.DescriptionSD.ImageId == 0)
            {
                x = MultiplyDivRound(positionX);
                y = MultiplyDivRound(positionY);
            }
            else
            {
                x = MultiplyDivRound(MultiplyDivOffset(positionX, image.DescriptionHD.OffsetX));
                y = MultiplyDivRound(MultiplyDivOffset(positionY, image.DescriptionHD.OffsetY));
            }

            int w = image.DescriptionSD.Width;
            int h = image.DescriptionSD.Height;

            bool isDrawScrollBar = originReturnAddress == 0x10;

            if (!isDrawScrollBar)
            {
                if (x >= dc.ScreenAreaRight
                    || y >= dc.ScreenAreaBottom
                    || x + w <= dc.ScreenAreaLeft
                    || y + h <= dc.ScreenAreaTop)
                {
                    return;
                }
            }

            if (name == "missmall")
            {
                if ((positionX < dc.ScreenAreaLeft) || (positionX + w > dc.ScreenAreaRight))
                {
                    return;
                }
            }

            bool isNewImage = image.DescriptionSD.Width != image.DescriptionHD.Width || image.DescriptionSD.Height != image.DescriptionHD.Height;
            bool isSameAspectRatio = Math.Abs(((float)image.DescriptionSD.Width / image.DescriptionSD.Height) - ((float)image.DescriptionHD.Width / image.DescriptionHD.Height)) < 0.1f;

            int destinationX;
            int destinationY;

            if (image.DescriptionSD.ImageId == 0)
            {
                destinationX = isBackgroundWide ? backgroundOffsetX + MultiplyDiv(positionX, backgroundWidth, dc.DisplayWidth) : MultiplyDiv(positionX, dc.Height, dc.DisplayHeight);
                destinationY = MultiplyDiv(positionY, dc.Height, dc.DisplayHeight);
            }
            else
            {
                destinationX = isBackgroundWide ? backgroundOffsetX + MultiplyDiv(positionX + image.DescriptionHD.OffsetX, backgroundWidth, dc.DisplayWidth) : MultiplyDiv(positionX, image.DescriptionHD.OffsetX, dc.Height, dc.DisplayHeight);
                destinationY = MultiplyDiv(positionY, image.DescriptionHD.OffsetY, dc.Height, dc.DisplayHeight);
            }

            int destinationWidth;
            int destinationHeight;

            if (IsBackgroundName(name))
            {
                if (isNewImage && !isSameAspectRatio)
                {
                    destinationX = (dc.Width - backgroundWidth) / 2;
                    destinationY = 0;
                    destinationWidth = backgroundWidth;
                    destinationHeight = dc.Height;
                }
                else
                {
                    destinationX = MultiplyDiv(positionX, dc.Height, dc.DisplayHeight);
                    destinationY = MultiplyDiv(positionY, dc.Height, dc.DisplayHeight);
                    destinationWidth = MultiplyDiv(image.DescriptionSD.Width, dc.Height, dc.DisplayHeight);
                    destinationHeight = MultiplyDiv(image.DescriptionSD.Height, dc.Height, dc.DisplayHeight);
                }

                if (isNewImage && !isSameAspectRatio)
                {
                    srcRect = null;
                }
                else
                {
                    if (image.DescriptionSD.ImageId == 0)
                    {
                        srcRect = Rectangle.FromLTRB(
                            0,
                            0,
                            image.DescriptionSD.Width,
                            image.DescriptionSD.Height);
                    }
                    else
                    {
                        srcRect = Rectangle.FromLTRB(
                            image.DescriptionSD.OffsetX,
                            image.DescriptionSD.OffsetY,
                            image.DescriptionSD.Width,
                            image.DescriptionSD.Height);
                    }
                }

                if (dc.ThisPtr == IntPtr.Zero)
                {
                    IntPtr hdc = NativeMethods.GetDC(IntPtr.Zero);

                    try
                    {
                        using var g = Graphics.FromHdc(hdc);
                        g.CompositingMode = System.Drawing.Drawing2D.CompositingMode.SourceCopy;
                        g.CompositingQuality = System.Drawing.Drawing2D.CompositingQuality.HighSpeed;
                        g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor;
                        g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighSpeed;

                        g.FillRectangle(Brushes.Black, Rectangle.FromLTRB(0, 0, dc.Width, dc.Height));

                        int imageWidth = image.Image.Width;
                        int imageHeight = image.Image.Height;
                        byte[] imageData = image.Image.GetImageData();

                        Bitmap bitmap = null;
                        GCHandle pinnedArray = GCHandle.Alloc(imageData, GCHandleType.Pinned);

                        try
                        {
                            bitmap = new Bitmap(imageWidth, imageHeight, imageWidth * 4, PixelFormat.Format32bppArgb, pinnedArray.AddrOfPinnedObject());
                        }
                        finally
                        {
                            pinnedArray.Free();
                        }

                        try
                        {
                            g.DrawImage(bitmap, destinationX, destinationY, destinationWidth, destinationHeight);
                        }
                        finally
                        {
                            bitmap.Dispose();
                            bitmap = null;
                        }
                    }
                    finally
                    {
                        NativeMethods.ReleaseDC(hdc);
                    }

                    return;
                }
            }
            else if (origin == Origin.DrawIcon)
            {
                if (isSameAspectRatio)
                {
                    destinationWidth = isBackgroundHD && isBackgroundWide ? MultiplyDiv(image.DescriptionSD.Width, backgroundWidth, dc.DisplayWidth) : MultiplyDiv(image.DescriptionSD.Width, dc.Height, dc.DisplayHeight);
                    destinationHeight = MultiplyDiv(image.DescriptionSD.Height, dc.Height, dc.DisplayHeight);
                }
                else
                {
                    destinationX = isBackgroundHD && isBackgroundWide ? backgroundOffsetX + MultiplyDiv(positionX, backgroundWidth, dc.DisplayWidth) : MultiplyDiv(positionX, dc.Height, dc.DisplayHeight);
                    destinationY = MultiplyDiv(positionY, -1, dc.Height, dc.DisplayHeight);
                    destinationWidth = isBackgroundHD && isBackgroundWide ? MultiplyDiv(image.DescriptionSD.Width, backgroundWidth, dc.DisplayWidth) : MultiplyDiv(image.DescriptionSD.Width, dc.Height, dc.DisplayHeight);
                    destinationHeight = MultiplyDiv(image.DescriptionHD.Height, destinationWidth, image.DescriptionHD.Width);
                    srcRect = null;
                }

                bool isHudColor = name == "hud";
                uint s_V0x0ABD224 = 0xff000000 | (0x60 >> 3 << 19) | (0x80 >> 2 << 10) | (0xFF >> 3 << 3);
                uint s_V0x0AE2A48 = 0xff000000 | (0xFF >> 3 << 19) | (0xFF >> 2 << 10) | (0x00 >> 3 << 3);
                uint s_V0x0AE2A30 = 0xff000000 | (0x00 >> 3 << 19) | (0xFF >> 2 << 10) | (0x00 >> 3 << 3);
                bool isHightlight = blendColor == s_V0x0ABD224 || blendColor == s_V0x0AE2A48 || blendColor == s_V0x0AE2A30;

                if (!isHudColor && !isHightlight && isNewImage)
                {
                    blendColor = 0;
                }
            }
            else if (origin == Origin.DrawCurrentImageAdd && name == "leftframe")
            {
                destinationWidth = isBackgroundHD && isBackgroundWide ? MultiplyDiv(image.DescriptionSD.Width, 7, backgroundWidth, dc.DisplayWidth) : MultiplyDiv(image.DescriptionSD.Width, 7, dc.Height, dc.DisplayHeight);
                destinationHeight = MultiplyDiv(image.DescriptionSD.Height, dc.Height, dc.DisplayHeight);
            }
            else if (image.DescriptionSD.ImageId == 0 || isSameAspectRatio)
            {
                destinationWidth = isBackgroundHD && isBackgroundWide ? MultiplyDiv(image.DescriptionSD.Width, backgroundWidth, dc.DisplayWidth) : MultiplyDiv(image.DescriptionSD.Width, dc.Height, dc.DisplayHeight);
                destinationHeight = MultiplyDiv(image.DescriptionSD.Height, dc.Height, dc.DisplayHeight);
            }
            else
            {
                destinationHeight = MultiplyDiv(image.DescriptionSD.Height, dc.Height, dc.DisplayHeight);
                destinationWidth = MultiplyDiv(image.DescriptionHD.Width, destinationHeight, image.DescriptionHD.Height);
            }

            Rectangle src = Rectangle.FromLTRB(0, 0, image.DescriptionHD.Width, image.DescriptionHD.Height);

            if (srcRect.HasValue)
            {
                src = srcRect.Value;

                if (IsBackgroundName(name) && isNewImage && !isSameAspectRatio)
                {
                    destinationX = (dc.Width - backgroundWidth) / 2;
                    destinationY = 0;
                    destinationWidth = backgroundWidth;
                    destinationHeight = dc.Height;
                }
                else
                {
                    destinationX = isBackgroundHD && isBackgroundWide ? backgroundOffsetX + MultiplyDiv(positionX, backgroundWidth, dc.DisplayWidth) : MultiplyDiv(positionX, dc.Height, dc.DisplayHeight);
                    destinationY = MultiplyDiv(positionY, dc.Height, dc.DisplayHeight);
                    destinationWidth = isBackgroundHD && isBackgroundWide ? MultiplyDiv(src.Width, backgroundWidth, dc.DisplayWidth) : MultiplyDiv(src.Width, dc.Height, dc.DisplayHeight);
                    destinationHeight = MultiplyDiv(src.Height, dc.Height, dc.DisplayHeight);
                }

                src.X = MultiplyDiv(src.X, image.DescriptionHD.Width, image.DescriptionSD.Width);
                src.Y = MultiplyDiv(src.Y, image.DescriptionHD.Height, image.DescriptionSD.Height);
                src.Width = MultiplyDiv(src.Width, image.DescriptionHD.Width, image.DescriptionSD.Width);
                src.Height = MultiplyDiv(src.Height, image.DescriptionHD.Height, image.DescriptionSD.Height);
            }

            src = RestrictImage(src, image);

            if ((!isBackground || !isBackgroundHD) && !isBackgroundWide)
            {
                destinationX += dc.Width / 2 - MultiplyDiv(dc.DisplayWidth, dc.Height, dc.DisplayHeight) / 2;
            }

            bool isBlackTransparent = origin == Origin.DrawMapIcon || origin == Origin.DrawGreyMapIcon;

            byte[] imageBitmapData = image.GetImageData();
            int imageFormat = (int)image.RawDataFormat;
            uint blendColorParameter = (blendColor & 0xffffff) | (isBlackTransparent ? 0xff000000 : 0);

            if (srcRect == null)
            {
                dc.DrawSurface(
                    dc.ThisPtr,
                    Marshal.UnsafeAddrOfPinnedArrayElement(imageBitmapData, 0),
                    image.DescriptionHD.Width,
                    image.DescriptionHD.Height,
                    destinationX,
                    destinationY,
                    destinationWidth,
                    destinationHeight,
                    0,
                    0,
                    image.DescriptionHD.Width,
                    image.DescriptionHD.Height,
                    isBackground ? -1 : 0,
                    blendColorParameter,
                    imageFormat);
            }
            else
            {
                dc.DrawSurface(
                    dc.ThisPtr,
                    Marshal.UnsafeAddrOfPinnedArrayElement(imageBitmapData, 0),
                    image.DescriptionHD.Width,
                    image.DescriptionHD.Height,
                    destinationX,
                    destinationY,
                    destinationWidth,
                    destinationHeight,
                    src.X,
                    src.Y,
                    src.Width,
                    src.Height,
                    rotation,
                    blendColorParameter,
                    imageFormat);
            }
        }
    }
}
