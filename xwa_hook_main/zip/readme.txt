xwa_hook_main

This is the main dll hook that is used to call other hooks. A hook is a way to inject code into the exe.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- Visual C++ 2013 Runtime (x86)

Visual C++ Redistributable Packages for Visual Studio 2013: vcredist_x86.exe
http://www.microsoft.com/en-us/download/details.aspx?id=40784


*** Setup ***

1) place dinput.dll and dinput_hooks.lst next to xwingalliance.exe

2) edit xwingalliance.exe:

You can manually apply the modifications with an hexadecimal editor, or you can use XwaExePatcher to have a GUI.

# To setup the dinput hook:
At offset 1A7F20, replace 00000000000000000000000000000000 with 506033C054505050E8EB42F7FF6158C3.
At offset 1A7F30, replace 00000000000000000000000000000000 with 536033C054505050E8DB42F7FF615BC3.

The first modification calls a hook and ignores the return value.
The second modification calls a hook and preserves the return value.


*** Usage ***

dinput_hooks.lst is a text file that lists the hooks.
Each hook is implemented in a separate dll.
See "dinput_hooks.lst" for an example.

*** Credits ***

- Jérémy Ansel (JeremyaFr)
