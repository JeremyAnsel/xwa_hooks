xwa_hook_lightmaps

This hook replaces a static buffer with a dynamic one in lightmaps loading.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_lightmaps.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that creates lightmaps
At offset 040F60, replace 8D8BB0D164003BC2895424347E4B with 505356E8C86F160083C40C90EB4B.
At offset 040FCD, replace 8D83B0D16400 with 909090909090.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
