xwa_hook_lightmaps

This hook replaces a static buffer by a dynamic one in lightmaps loading.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- Visual C++ 2015 Runtime (x86)
- xwa_hook_main

Visual C++ Redistributable for Visual Studio 2015: vc_redist.x86.exe
https://www.microsoft.com/en-us/download/details.aspx?id=48145


*** Setup ***

1) add hook_lightmaps.dll to dinput_hooks.lst

2) place hook_lightmaps.dll next to xwingalliance.exe

3) edit xwingalliance.exe:

You can manually apply the modifications with an hexadecimal editor, or you can use XwaExePatcher to have a GUI.

# To call the hook that creates lightmaps
At offset 040F60, replace 8D8BB0D164003BC2895424347E4B with 505356E8C86F160083C40C90EB4B.
At offset 040FCD, replace 8D83B0D16400 with 909090909090.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
