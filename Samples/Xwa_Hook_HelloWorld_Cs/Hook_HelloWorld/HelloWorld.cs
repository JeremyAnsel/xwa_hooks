using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace Hook_HelloWorld
{
    static class HelloWorld
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        delegate string XwaGetFrontTxtStringDelegate(int index);

        static XwaGetFrontTxtStringDelegate XwaGetFrontTxtString = (XwaGetFrontTxtStringDelegate)Marshal.GetDelegateForFunctionPointer(
            new IntPtr(0x55CB50), typeof(XwaGetFrontTxtStringDelegate));

        public static int HelloWorldHook(IntPtr @params)
        {
            IntPtr bufferPtr = new IntPtr(0xABD680);
            const int FrontTxt_STR_CONFIG_VERSION = 1168;

            byte[] message = Encoding.ASCII.GetBytes("Hello, World! \0");

            Marshal.Copy(message, 0, bufferPtr, message.Length);

            byte[] message2 = Encoding.ASCII.GetBytes(XwaGetFrontTxtString(FrontTxt_STR_CONFIG_VERSION) + "2.02\0");
            Marshal.Copy(message2, 0, IntPtr.Add(bufferPtr, message.Length - 1), message2.Length);

            return 0;
        }
    }
}
