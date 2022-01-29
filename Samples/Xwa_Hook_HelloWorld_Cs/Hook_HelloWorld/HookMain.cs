using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace Hook_HelloWorld
{
    static class HookMain
    {
        [DllExport(CallingConvention.Cdecl)]
        public static int GetHookFunctionsCount()
        {
            return Hooks.hookFunctionsPtr.Length;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static HookFunctionPtr GetHookFunction(int index)
        {
            if (index >= 0 && index < Hooks.hookFunctionsPtr.Length)
            {
                return Hooks.hookFunctionsPtr[index];
            }

            return new HookFunctionPtr();
        }

        [DllExport(CallingConvention.Cdecl)]
        public static int GetHookPatchesCount()
        {
            return Hooks.patchesPtr.Length;
        }

        [DllExport(CallingConvention.Cdecl)]
        public static IntPtr GetHookPatch(int index)
        {
            if (index < 0 || index >= Hooks.patchesPtr.Length)
            {
                return IntPtr.Zero;
            }

            return Marshal.UnsafeAddrOfPinnedArrayElement(Hooks.patchesPtr, index);
        }
    }
}
