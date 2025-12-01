using System;
using System.Text;

namespace hook_patcher
{
    struct HookFunctionPtr
    {
        public int From;
        public IntPtr Function;
    }

    class HookPatchItem
    {
        public int offset;
        public byte[] fromData;
        public byte[] toData;

        private static void AjustOffset(ref int offset)
        {
            /*
            Number of Objects = 0004 (dec), Imagebase = 00400000h
            Object01: .text    RVA: 00001000 Offset: 00000400 Size: 001A7C00 Flags: 60000020
            Object02: .rdata   RVA: 001A9000 Offset: 001A8000 Size: 00004C00 Flags: 40000040
            Object03: .data    RVA: 001AE000 Offset: 001ACC00 Size: 00060600 Flags: C0000040
            Object04: .rsrc    RVA: 00710000 Offset: 0020D200 Size: 00000400 Flags: 40000040
            */

            if (offset >= 0x001A8000 && offset < 0x001A8000 + 0x00004C00)
            {
                // rdata segment
                offset = offset - 0x001A8000 + 0x5A9000 - 0x400C00;
            }
            else if (offset >= 0x001ACC00 && offset < 0x001ACC00 + 0x00060600)
            {
                // data segment
                offset = offset - 0x001ACC00 + 0x5AE000 - 0x400C00;
            }
        }

        public HookPatchItem(int offset, string from, string to)
        {
            //AjustOffset(ref offset);
            this.offset = offset;
            this.fromData = Encoding.ASCII.GetBytes(from + "\0");
            this.toData = Encoding.ASCII.GetBytes(to + "\0");
        }
    }
}
