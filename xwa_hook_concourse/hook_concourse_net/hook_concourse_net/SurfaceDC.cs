using System;
using System.Runtime.InteropServices;

namespace hook_concourse_net
{
    public readonly struct SurfaceDC
    {
        public delegate void DrawSurfaceDelegate(
            IntPtr dc,
            IntPtr data,
            int dataWidth,
            int dataHeight,
            int destinationX,
            int destinationY,
            int destinationWidth,
            int destinationHeight,
            int srcLeft,
            int srcTop,
            int srcWidth,
            int srcHeight,
            int rotation,
            uint blendColorParameter,
            int imageFormat);

        public SurfaceDC(IntPtr ptr)
        {
            ThisPtr = ptr;
            Buffer32 = new IntPtr(Marshal.ReadInt32(ptr + 0));
            Width = Marshal.ReadInt32(ptr + 4);
            Height = Marshal.ReadInt32(ptr + 8);
            DisplayWidth = Marshal.ReadInt32(ptr + 12);
            DisplayHeight = Marshal.ReadInt32(ptr + 16);
            AspectRatioPreserved = Marshal.ReadByte(ptr + 20) != 0;
            Callback = new IntPtr(Marshal.ReadInt32(ptr + 24));
            DrawSurface = Marshal.GetDelegateForFunctionPointer<DrawSurfaceDelegate>(new IntPtr(Marshal.ReadInt32(Callback)));

            ScreenAreaLeft = Marshal.ReadInt32(new IntPtr(0x09F60E0 + 0x0FAA));
            ScreenAreaRight = Marshal.ReadInt32(new IntPtr(0x09F60E0 + 0x0FAE));
            ScreenAreaTop = Marshal.ReadInt32(new IntPtr(0x09F60E0 + 0x0FB2));
            ScreenAreaBottom = Marshal.ReadInt32(new IntPtr(0x09F60E0 + 0x0FB6));
        }

        public SurfaceDC(int width, int height)
        {
            ThisPtr = IntPtr.Zero;
            Buffer32 = IntPtr.Zero;
            Width = width;
            Height = height;
            DisplayWidth = 640;
            DisplayHeight = 480;
            AspectRatioPreserved = true;
            Callback = IntPtr.Zero;
            DrawSurface = null;

            ScreenAreaLeft = Marshal.ReadInt32(new IntPtr(0x09F60E0 + 0x0FAA));
            ScreenAreaRight = Marshal.ReadInt32(new IntPtr(0x09F60E0 + 0x0FAE));
            ScreenAreaTop = Marshal.ReadInt32(new IntPtr(0x09F60E0 + 0x0FB2));
            ScreenAreaBottom = Marshal.ReadInt32(new IntPtr(0x09F60E0 + 0x0FB6));
        }

        public IntPtr ThisPtr { get; }

        public IntPtr Buffer32 { get; }

        public int Width { get; }

        public int Height { get; }

        public int DisplayWidth { get; }

        public int DisplayHeight { get; }

        public bool AspectRatioPreserved { get; }

        public IntPtr Callback { get; }

        public DrawSurfaceDelegate DrawSurface { get; }

        public int ScreenAreaLeft { get; }

        public int ScreenAreaRight { get; }

        public int ScreenAreaTop { get; }

        public int ScreenAreaBottom { get; }
    }
}
