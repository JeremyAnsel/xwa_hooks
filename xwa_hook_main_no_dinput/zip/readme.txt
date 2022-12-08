xwa_hook_main_no_dinput

This is the main dll hook that is used to call other hooks. A hook is a way to inject code into the exe.


*** Requirements ***

This dll requires:
- Windows 7 or superior


*** Setup ***

1) Place hooks_main.dll and dinput.cfg next to xwingalliance.exe

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
At offset 1A7F20, replace 00000000000000000000000000000000 with 9C6054FF15608B5A0083C404619DC390.
At offset 1A7F30, replace 00000000000000000000000000000000 with 9C6054FF15608B5A0083C404619DC390.

The first modification calls a hook and ignores the return value.
The second modification calls a hook and preserves the return value.


*** Usage ***

The hooks are downloadable from GitHub (https://github.com/JeremyAnsel/xwa_hooks) or via the XwaHooksSetup tool (https://github.com/JeremyAnsel/XwaHooksSetup/tree/master/XwaHooksSetup/zip).

Each hook is implemented in a separate dll. The name of the dll must begin with "hook_" or "Hook_".

When the game's exe ends, statistics about the hooks are written to OutputDebugString.
When the game crashes a dump file will be generated if the GenerateDumpFilesOnCrash setting is set to 1. The default value is 1.

See the "dinput.cfg" file or the "dinput" section of "hooks.ini".


*** Credits ***

- Jérémy Ansel (JeremyaFr)
