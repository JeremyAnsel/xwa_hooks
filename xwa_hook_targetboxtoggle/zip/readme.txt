xwa_hook_targetboxtoggle

This hook toggles targetbox on Ctrl+T key.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_targetboxtoggle.dll and hook_targetboxtoggle.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that toggles targetbox
At offset 0FAE9C, replace 8B86E0948B00 with E87FD00A0090.
At offset 102E30, replace 83EC08 with C39090.


*** Credits ***

- Blake Guthrie
- Jérémy Ansel (JeremyaFr)
