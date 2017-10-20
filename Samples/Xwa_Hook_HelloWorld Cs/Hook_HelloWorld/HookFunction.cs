using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace Hook_HelloWorld
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    delegate int HookFunctionDelegate(IntPtr @params);

    struct HookFunctionPtr
    {
        public int From;
        public IntPtr Function;
    }

    class HookFunction
    {
        private int from;
        private HookFunctionDelegate function;
        private IntPtr functionPtr;

        public HookFunction(int from, HookFunctionDelegate function)
        {
            this.from = from;
            this.function = function;
            this.functionPtr = Marshal.GetFunctionPointerForDelegate(this.function);
        }

        public HookFunctionPtr Struct
        {
            get
            {
                return new HookFunctionPtr
                {
                    From = this.from,
                    Function = this.functionPtr
                };
            }
        }
    };

    struct HookPatchItemPtr
    {
        public int Offset;
        public IntPtr From;
        public IntPtr To;
    }

    class HookPatchItem
    {
        private int offset;
        private byte[] fromData;
        private IntPtr fromPtr;
        private byte[] toData;
        private IntPtr toPtr;

        public HookPatchItem(int offset, string from, string to)
        {
            this.offset = offset;
            from = string.Concat(from, "\0");
            to = string.Concat(to, "\0");
            this.fromData = Encoding.ASCII.GetBytes(from);
            this.fromPtr = Marshal.UnsafeAddrOfPinnedArrayElement(this.fromData, 0);
            this.toData = Encoding.ASCII.GetBytes(to);
            this.toPtr = Marshal.UnsafeAddrOfPinnedArrayElement(this.toData, 0);
        }

        public HookPatchItemPtr Struct
        {
            get
            {
                return new HookPatchItemPtr
                {
                    Offset = this.offset,
                    From = this.fromPtr,
                    To = this.toPtr
                };
            }
        }
    }

    struct HookPatchPtr
    {
        public IntPtr Name;
        public int Count;
        public IntPtr Items;
    }

    class HookPatch
    {
        private byte[] nameData;
        private IntPtr namePtr;
        private int itemsCount;
        private IntPtr itemsPtr;

        public HookPatch(string name, HookPatchItemPtr[] items)
        {
            name = string.Concat(name, "\0");
            this.nameData = Encoding.ASCII.GetBytes(name);
            this.namePtr = Marshal.UnsafeAddrOfPinnedArrayElement(this.nameData, 0);
            this.itemsCount = items.Length;
            this.itemsPtr = Marshal.UnsafeAddrOfPinnedArrayElement(items, 0);
        }

        public HookPatchPtr Struct
        {
            get
            {
                return new HookPatchPtr
                {
                    Name = this.namePtr,
                    Count = this.itemsCount,
                    Items = this.itemsPtr
                };
            }
        }
    }
}
