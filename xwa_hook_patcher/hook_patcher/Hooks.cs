using System;
using System.Collections.Generic;
using System.Linq;

namespace hook_patcher
{
    static class Hooks
    {
        private static readonly Dictionary<string, HookPatchItem[]> patcherPatches = ExePatcher.GetHookPatches();

        private static readonly HookFunction[] hookFunctions = new HookFunction[]
        {
        };

        public static readonly HookFunctionPtr[] hookFunctionsPtr = Array.ConvertAll(hookFunctions, t => t.Struct);

        private static readonly HookPatchItem[][] hookPatcherPatch =
            patcherPatches
            .Select(patch => patch.Value)
            .ToArray();

        public static readonly HookPatchItemPtr[][] hookPatcherPatchPtr =
            hookPatcherPatch
            .Select(patch => Array.ConvertAll(patch, t => t.Struct))
            .ToArray();

        private static readonly HookPatch[] patches =
            Enumerable.Range(0, patcherPatches.Count)
            .Select(index => new HookPatch(patcherPatches.ElementAt(index).Key, hookPatcherPatchPtr[index]))
            .ToArray();

        public static readonly HookPatchPtr[] patchesPtr = Array.ConvertAll(patches, t => t.Struct);
    }
}
