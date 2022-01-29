xwa_hook_textures_tag

This hook set a tag to the textures. The tag can then be read from ddraw.dll.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_textures_tag.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that set textures tag
At offset 031AEB, replace E8CE391600 with E840641700.
At offset 0CBD4C, replace E8AF010000 with E8DFC10D00.
At offset 0410DF, replace E8DA431500 with E84C6E1600.
At offset 04119B, replace E81E431500 with E8906D1600.
At offset 1959F0, replace E8FB710000 with E82B250100.


*** Usage ***

The tag can be read from the CreateSurface method of the IDirectDraw interface. The dwReserved variable of the DDSURFACEDESC structure is set to a pointer to the tag string (const char*).

The format of the tag is:
- for dat images: "dat,groupId,ImageId,mipmapLevel"
- for opt textures: "opt,optFileName,textureName,color,mipmapLevel" for color maps or "opt,optFileName,textureName,color-transparent,mipmapLevel" for transparent color maps or "opt,optFileName,textureName,light,mipmapLevel" for light maps

To help debugging, the hook can optionally write the tags to the debug ouput that can be read with DebugView.
See the "hook_textures_tag.cfg" file or the "hook_textures_tag" section of "hooks.ini".


*** Credits ***

- Jérémy Ansel (JeremyaFr)
