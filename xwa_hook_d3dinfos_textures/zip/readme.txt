xwa_hook_d3dinfos_textures

This hook increases the D3DTextures count limit.
When there are more than 1624 textures, the game outputs "Not enough D3DTextures" and uses the last created texture. This hook fixes that.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

1) add hook_d3dinfos_textures.dll to dinput_hooks.lst

2) place hook_d3dinfos_textures.dll and hook_d3dinfos_textures.cfg next to xwingalliance.exe

3) edit xwingalliance.exe:

You can manually apply the modifications with an hexadecimal editor, or you can use XwaExePatcher to have a GUI.

# To call the hook that removes D3DInfos count limit
At offset 040B50, replace 5657BAC3296600 with E8CB731600C390.


*** Usage ***

The original D3DInfos count is 1624.
By default, the dll will use a value of 10000.
You can modify this value in hook_d3dinfos_textures.cfg (Size = value).


*** Credits ***

- Jérémy Ansel (JeremyaFr)
