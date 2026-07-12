using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;

namespace hook_patcher
{
    static class HookMain
    {
        [UnmanagedCallersOnly(CallConvs = [typeof(CallConvCdecl)], EntryPoint = nameof(GetHookFunctionsCount))]
        public static int GetHookFunctionsCount()
        {
            return 0;
        }

        [UnmanagedCallersOnly(CallConvs = [typeof(CallConvCdecl)], EntryPoint = nameof(GetHookFunction))]
        public static HookFunctionPtr GetHookFunction(int index)
        {
            return new HookFunctionPtr();
        }

        private static readonly Dictionary<string, HookPatchItem[]> _patcherPatches = ExePatcher.GetHookPatches();

        [UnmanagedCallersOnly(CallConvs = [typeof(CallConvCdecl)], EntryPoint = nameof(GetHookPatchesCount))]
        public static int GetHookPatchesCount()
        {
            return _patcherPatches.Count;
        }

        public static byte[] _patchData;

        private static IntPtr MarshalPatch(string name, HookPatchItem[] items)
        {
            byte[] nameData = Encoding.ASCII.GetBytes(name + "\0");

            int length = 0;
            length += 12;
            length += nameData.Length;

            foreach (HookPatchItem item in items)
            {
                length += 12;
                length += item.fromData.Length;
                length += item.toData.Length;
            }

            _patchData = new byte[length];

            IntPtr patchPtr = Marshal.UnsafeAddrOfPinnedArrayElement(_patchData, 0);
            int patchPtrInt = patchPtr.ToInt32();

            /*
            struct HookPatch
            {
	            const char* Name;
	            int Count;
	            const HookPatchItem* Items;
            };
            */

            BitConverter.GetBytes(patchPtrInt + 12).CopyTo(_patchData, 0);
            BitConverter.GetBytes(items.Length).CopyTo(_patchData, 4);
            BitConverter.GetBytes(patchPtrInt + 12 + nameData.Length).CopyTo(_patchData, 8);
            nameData.CopyTo(_patchData, 12);

            /*
            struct HookPatchItem
            {
	            int Offset;
	            const char* From;
	            const char* To;
            };
            */

            int itemsHeaderOffset = 12 + nameData.Length;
            int itemsPosition = itemsHeaderOffset + items.Length * 12;

            for (int itemIndex = 0; itemIndex < items.Length; itemIndex++)
            {
                HookPatchItem item = items[itemIndex];
                BitConverter.GetBytes(item.offset).CopyTo(_patchData, itemsHeaderOffset + itemIndex * 12 + 0);

                BitConverter.GetBytes(patchPtrInt + itemsPosition).CopyTo(_patchData, itemsHeaderOffset + itemIndex * 12 + 4);
                item.fromData.CopyTo(_patchData, itemsPosition);
                itemsPosition += item.fromData.Length;

                BitConverter.GetBytes(patchPtrInt + itemsPosition).CopyTo(_patchData, itemsHeaderOffset + itemIndex * 12 + 8);
                item.toData.CopyTo(_patchData, itemsPosition);
                itemsPosition += item.toData.Length;
            }

            BitConverter.ToString(_patchData);

            return patchPtr;
        }

        [UnmanagedCallersOnly(CallConvs = [typeof(CallConvCdecl)], EntryPoint = nameof(GetHookPatch))]
        public static IntPtr GetHookPatch(int index)
        {
            if (index < 0 || index >= _patcherPatches.Count)
            {
                return IntPtr.Zero;
            }

            KeyValuePair<string, HookPatchItem[]> patch = _patcherPatches.ElementAt(index);
            return MarshalPatch(patch.Key, patch.Value);
        }
    }
}
