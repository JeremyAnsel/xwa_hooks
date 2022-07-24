xwa_hook_main

This is the main dll hook that is used to call other hooks. A hook is a way to inject code into the exe.


*** Requirements ***

This dll requires:
- Windows 7 or superior


*** Setup ***

Place dinput.dll and dinput.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To setup the dinput hook:
At offset 1A7F20, replace 00000000000000000000000000000000 with 9C6033C054505050E8EB42F7FF619DC3.
At offset 1A7F30, replace 00000000000000000000000000000000 with 9C6033C054505050E8DB42F7FF619DC3.

The first modification calls a hook and ignores the return value.
The second modification calls a hook and preserves the return value.


*** Usage ***

The hooks are downloadable from GitHub (https://github.com/JeremyAnsel/xwa_hooks) or via the XwaHooksSetup tool (https://github.com/JeremyAnsel/XwaHooksSetup/tree/master/XwaHooksSetup/zip).

Each hook is implemented in a separate dll. The name of the dll must begin with "hook_" or "Hook_".

When the game's exe ends, statistics about the hooks are written to OutputDebugString.
See the "dinput.cfg" file or the "dinput" section of "hooks.ini".

If you use a custom Dinput.dll, it will conflict with this hook. In this case, use xwa_hook_main_no_dinput instead of xwa_hook_main.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
