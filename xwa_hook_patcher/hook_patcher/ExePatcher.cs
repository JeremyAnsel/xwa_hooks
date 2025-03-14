using JeremyAnsel.Xwa.ExePatcher;
using JeremyAnsel.Xwa.HooksConfig;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;

namespace hook_patcher
{
    static class ExePatcher
    {
        public const string PatcherXmlFileName = "hook_patcher.xml";
        public const string PatcherTxtFileName = "hook_patcher.txt";

        public static Dictionary<string, HookPatchItem[]> GetHookPatches()
        {
            var hookPatches = new Dictionary<string, HookPatchItem[]>();

            try
            {
                if (File.Exists(PatcherXmlFileName))
                {
                    Patcher patcher = Patcher.Read(PatcherXmlFileName);
                    IList<string> lines = XwaHooksConfig.GetFileLines(PatcherTxtFileName);

                    foreach (Patch patch in patcher.Patches)
                    {
                        bool isPatchEnabled = XwaHooksConfig.GetFileKeyValueInt(lines, patch.Name) != 0;

                        Trace.WriteLine($"{patch.Name} = {isPatchEnabled}");

                        if (!isPatchEnabled)
                        {
                            continue;
                        }

                        var items = new List<HookPatchItem>();

                        foreach (PatchItem item in patch.Items)
                        {
                            items.Add(new HookPatchItem(item.Offset, item.OldValuesString, item.NewValuesString));
                        }

                        hookPatches["[hook_patcher] " + patch.Name] = items.ToArray();
                    }
                }
            }
            catch
            {
            }

            return hookPatches;
        }
    }
}
