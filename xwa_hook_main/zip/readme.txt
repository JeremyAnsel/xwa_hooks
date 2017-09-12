xwa_hook_main

This is the main dll hook that is used to call other hooks. A hook is a way to inject code into the exe.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior


*** Setup ***

Place dinput.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To setup the dinput hook:
At offset 1A7F20, replace 00000000000000000000000000000000 with 506033C054505050E8EB42F7FF6158C3.
At offset 1A7F30, replace 00000000000000000000000000000000 with 536033C054505050E8DB42F7FF615BC3.

The first modification calls a hook and ignores the return value.
The second modification calls a hook and preserves the return value.


*** Usage ***

Each hook is implemented in a separate dll. The name of the dll must begin with "hook_" or "Hook_".


*** Credits ***

- Jérémy Ansel (JeremyaFr)
