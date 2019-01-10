xwa_hook_res1200

This hook replaces a static buffer with a dynamic one in initializing 2d buffer. It fixes a crash when resolution height > 1200 is used.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_res1200.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that fix res 1200 crash
At offset 0D31E0, replace 8B156C4B7D00 with E83B4D0D00C3.
At offset 0D32F0, replace C705A0D57400A0907F00 with E82B4C0D009090909090.
At offset 0D330D, replace 8B54241C33C985D27620 with E80E4C0D005F5E5D5BC3.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
