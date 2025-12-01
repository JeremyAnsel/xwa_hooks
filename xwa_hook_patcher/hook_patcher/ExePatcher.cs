using JeremyAnsel.Xwa.ExePatcher;
using JeremyAnsel.Xwa.HooksConfig;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;

namespace hook_patcher
{
    static class ExePatcher
    {
        public const string PatcherXmlFileName = "hook_patcher.xml";
        public const string PatcherTxtFileName = "hook_patcher.txt";
        public const string PatcherXmlFilesDirectory = "patcher-xml";

        private static Patcher GetPatcher()
        {
            Patcher patcher = new();

            MergePatcher(patcher, PatcherXmlFileName);

            if (Directory.Exists(PatcherXmlFilesDirectory))
            {
                foreach (string file in Directory.EnumerateFiles(PatcherXmlFilesDirectory, "*.xml"))
                {
                    if (!file.EndsWith(".xml", StringComparison.OrdinalIgnoreCase))
                    {
                        continue;
                    }

                    MergePatcher(patcher, file);
                }
            }

            return patcher;
        }

        private static Patcher ReadPatcherXml(string filename)
        {
            if (!File.Exists(filename))
            {
                return new();
            }

            return Patcher.Read(filename);
        }

        private static void MergePatcher(Patcher patcher, string filename)
        {
            try
            {
                Patcher patcherToAdd = ReadPatcherXml(filename);

                foreach (Patch patch in patcherToAdd.Patches)
                {
                    patcher.Patches.Add(patch);
                }
            }
            catch
            {
            }
        }

        public static Dictionary<string, HookPatchItem[]> GetHookPatches()
        {
            var hookPatches = new Dictionary<string, HookPatchItem[]>();

            try
            {
                Patcher patcher = GetPatcher();
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
            catch
            {
            }

            return hookPatches;
        }
    }
}
