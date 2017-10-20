using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Hook_HelloWorld
{
    static class Hooks
    {
        private static HookFunction[] hookFunctions = new HookFunction[]
        {
            new HookFunction(0x51D53A, HelloWorld.HelloWorldHook)
        };

        public static HookFunctionPtr[] hookFunctionsPtr = Array.ConvertAll(hookFunctions, t => t.Struct);

        private static HookPatchItem[] helloWorldPatch = new HookPatchItem[]
        {
            new HookPatchItem(0x11C935, "E846D10700", "E8E6B50800")
        };

        public static HookPatchItemPtr[] helloWorldPatchPtr = Array.ConvertAll(helloWorldPatch, t => t.Struct);

        private static HookPatch[] patches = new HookPatch[]
        {
            new HookPatch("Hello World Patch", helloWorldPatchPtr)
        };

        public static HookPatchPtr[] patchesPtr = Array.ConvertAll(patches, t => t.Struct);
    }
}
