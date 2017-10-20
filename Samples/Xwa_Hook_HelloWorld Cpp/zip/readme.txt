Xwa_Hook_HelloWorld

This hook shows "Hello, World!" at the bottom of the game menu, before the version string.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place Hook_HelloWorld.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# Hello World Patch
At offset 11C935, replace E846D10700 with E8E6B50800.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
