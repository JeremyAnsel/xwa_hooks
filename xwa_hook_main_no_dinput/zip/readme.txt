xwa_hook_main_no_dinput

This is the main dll hook that is used to call other hooks. A hook is a way to inject code into the exe.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior


*** Setup ***

1) Place hooks_main.dll next to xwingalliance.exe

2) edit xwingalliance.exe:

You can manually apply the modifications with an hexadecimal editor, or you can use XwaExePatcher to have a GUI.

# To setup the main hook
At offset 19C2AD, replace E8EED5F6FF with E8BEBC0000.
At offset 1A7F40, replace 000000000000000000000000000000 with 686F6F6B735F6D61696E2E646C6C00.
At offset 1A7F50, replace 000000000000000000 with 43616C6C486F6F6B00.
At offset 1A7F70, replace 0000000000000000000000000000000000000000000000000000000000000000000000000000 with 6068408B5A00FF15C0915A0068508B5A0050FF15FC905A00506A00FFD083C40861E90A19F6FF.


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the main hook
At offset 1A7F20, replace 00000000000000000000000000000000 with 506054FF15608B5A0083C4046158C390.
At offset 1A7F30, replace 00000000000000000000000000000000 with 536054FF15608B5A0083C404615BC390.

The first modification calls a hook and ignores the return value.
The second modification calls a hook and preserves the return value.


*** Usage ***

Each hook is implemented in a separate dll. The name of the dll must begin with "hook_" or "Hook_".


*** Credits ***

- Jérémy Ansel (JeremyaFr)
