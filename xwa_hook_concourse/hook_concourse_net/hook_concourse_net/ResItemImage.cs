using JeremyAnsel.Xwa.Cbm;
using JeremyAnsel.Xwa.Dat;
using System;
using System.IO;

namespace hook_concourse_net
{
    public sealed class ResItemImage
    {
        public ResItemImage()
        {
        }

        public ResItemImage(string source, ResItemDescription descriptionSD, DatImage datImage, string name)
        {
            Source = source;

            DescriptionSD = descriptionSD;

            DescriptionHD.GroupId = datImage.GroupId;
            DescriptionHD.ImageId = datImage.ImageId;
            DescriptionHD.Width = datImage.Width;
            DescriptionHD.Height = datImage.Height;
            DescriptionHD.OffsetX = datImage.OffsetX;
            DescriptionHD.OffsetY = datImage.OffsetY;

            Image = datImage;
        }

        public ResItemImage(string source, ResItemDescription descriptionSD, CbmImage cbmImage, string name)
        {
            Source = source;

            DescriptionSD = descriptionSD;

            DescriptionHD.GroupId = 0;
            DescriptionHD.ImageId = 0;
            DescriptionHD.Width = cbmImage.Width;
            DescriptionHD.Height = cbmImage.Height;
            DescriptionHD.OffsetX = cbmImage.OffsetX;
            DescriptionHD.OffsetY = cbmImage.OffsetY;

            bool addBackground = (cbmImage.GetPalette32()[0] & 0xffffff) != 0;

            if (name == "dialogbox")
            {
                cbmImage.GetPalette32()[0] = 0;
            }
            else if (name == "single" || name == "multi")
            {
                addBackground = true;
            }

            byte[] data = cbmImage.GetImageData(addBackground);

            DatImage datImage = DatImage.FromMemory(
                0,
                0,
                DatImageFormat.Format25,
                (short)cbmImage.Width,
                (short)cbmImage.Height,
                0,
                data);

            datImage.ConvertToFormatBc3();

            Image = datImage;
        }

        public string Source { get; }

        public ResItemDescription DescriptionSD { get; } = new ResItemDescription();

        public ResItemDescription DescriptionHD { get; } = new ResItemDescription();

        public DatImage Image { get; private set; }

        public DatImageFormat RawDataFormat { get; private set; }

        public byte[] RawData { get; private set; }

        public byte[] RawData32 { get; private set; }

        public byte[] GetImageData()
        {
            if (RawData is null && Image is not null)
            {
                if (Image.GetRawData() == null)
                {
                    string filenameHD = DescriptionSD.Filename.Substring(0, DescriptionSD.Filename.Length - 4) + "_hd.dat";

                    if (!File.Exists(filenameHD))
                    {
                        filenameHD = DescriptionSD.Filename;
                    }

                    Image = DatFile.GetImageDataById(filenameHD, Image.GroupId, Image.ImageId);
                }

                if (Image.Format == DatImageFormat.FormatBc3 || Image.Format == DatImageFormat.Format25)
                {
                    RawDataFormat = Image.Format;
                    RawData = Image.GetRawData();
                }
                //else if (Image.Width % 4 == 0 && Image.Height % 4 == 0)
                //{
                //    Image.ConvertToFormatBc3();
                //    RawDataFormat = Image.Format;
                //    RawData = Image.GetRawData();
                //}
                else
                {
                    RawDataFormat = DatImageFormat.Format25;
                    RawData = Image.GetImageData();
                }
            }

            return RawData;
        }

        public byte[] GetImageData32()
        {
            if (RawData32 is null && Image is not null)
            {
                if (Image.GetRawData() != null)
                {
                    RawData32 = Image.GetImageData();
                }
                else
                {
                    string filenameHD = DescriptionSD.Filename.Substring(0, DescriptionSD.Filename.Length - 4) + "_hd.dat";

                    if (!File.Exists(filenameHD))
                    {
                        filenameHD = DescriptionSD.Filename;
                    }

                    DatImage image = DatFile.GetImageDataById(filenameHD, Image.GroupId, Image.ImageId);
                    return image.GetImageData();
                }
            }

            return RawData32;
        }

        public bool IsNewImage()
        {
            if (DescriptionSD is null || DescriptionHD is null)
            {
                return false;
            }

            bool isNewImage = DescriptionSD.Width != DescriptionHD.Width || DescriptionSD.Height != DescriptionHD.Height;
            return isNewImage;
        }

        public bool IsSameAspectRatio()
        {
            if (DescriptionSD is null || DescriptionHD is null)
            {
                return true;
            }

            bool isSameAspectRatio = Math.Abs(((float)DescriptionSD.Width / DescriptionSD.Height) - ((float)DescriptionHD.Width / DescriptionHD.Height)) < 0.1f;
            return isSameAspectRatio;
        }
    }
}
