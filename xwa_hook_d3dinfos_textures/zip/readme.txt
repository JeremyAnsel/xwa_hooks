xwa_hook_d3dinfos_textures

This hook increases the D3DTextures count limit.
When there are more than 1624 textures, the game outputs "Not enough D3DTextures" and uses the last created texture. This hook fixes that.

For more informations, please see the following thread at xwaupgrade.com:
http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=10&t=11274


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_d3dinfos_textures.dll and hook_d3dinfos_textures.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that removes D3DInfos count limit
At offset 040B50, replace 5657BAC3296600 with E8CB731600C390.

# To call the hook that tests the texture illumination
At offset 042D1C, replace 0F8478020000 with E8FF51160090.
At offset 081D6B, replace 8B8EA8000000 with E8B061120090.


*** Usage ***

The original D3DInfos count is 1624.
By default, the dll will use a value of 10000.
You can modify this value in "hook_d3dinfos_textures.cfg" or in the "hook_d3dinfos_textures" section of "hooks.ini".
The format is:
Size = value


*** Credits ***

- Jérémy Ansel (JeremyaFr)
