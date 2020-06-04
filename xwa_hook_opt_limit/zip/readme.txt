xwa_hook_opt_limit

This hook removes the opt limit of 512 vertices per mesh.
It also generates the "spec.rci" file.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_opt_limit.dll and hook_opt_limit.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# 'triangling to infinity' patch
At offset 193C4A, replace 750C with 9090.
At offset 193C52, replace 0002 with 0080.

# 'disable briefing wireframe overlay' patch
At offset 07E09C, replace 7D with EB.
At offset 07E12E, replace 7D with EB.
At offset 07E1B9, replace 47 with 90.

# 'remove textures size limit' patch
At offset 19564A, replace 81790C000100007717 with 909090909090909090.
At offset 1956C2, replace 817A10000100007717 with 909090909090909090.

# 'opt textures count limit' patch
At offset 0CC432, replace 2003 with 0010.
At offset 0CC43A, replace 2803 with 0810.
At offset 0CC445, replace 2C03 with 0C10.
At offset 0CC44C, replace 2003 with 0010.
At offset 0CC466, replace C800 with 0004.
At offset 0CC497, replace 3003 with 1010.
At offset 0CC4C9, replace 3003 with 1010.
At offset 0CC4E6, replace 3003 with 1010.
At offset 0CC4EE, replace 2003 with 0010.

# To call the hook that removes the opt limit of 512 vertices per mesh
At offset 193BFE, replace 01 with 20.
At offset 0E7616, replace 68D37600 with hitDataArray8.
At offset 0E7888, replace 68D37600 with hitDataArray8.
At offset 0E7898, replace 68D37600 with hitDataArray8.
At offset 0E78A6, replace 68D37600 with hitDataArray8.
At offset 0E78B9, replace 68D37600 with hitDataArray8.
At offset 0E78DE, replace 60D37600 with hitDataArray0.
At offset 0E78F5, replace 60D37600 with hitDataArray0.
At offset 0E790C, replace 60D37600 with hitDataArray0.
At offset 0E792C, replace 60D37600 with hitDataArray0.
At offset 0E79DA, replace 60D37600 with hitDataArray0.
At offset 0E79E1, replace 64D37600 with hitDataArray4.
At offset 0E7A02, replace 60D37600 with hitDataArray0.
At offset 0E7A08, replace 64D37600 with hitDataArray4.

# 'GlowMarkUVArray vertices count' patch
At offset 0E758F, replace A148D37600 with A1066D9B00.
At offset 0E75B8, replace A148D37600 with A1066D9B00.

# To call the hook that generates spec.rci
At offset 127FCF, replace E87C0D0000 with E84CFF0700.

# 'fix depth buffer bug' patch
At offset 0415C0, replace 8B442404A3B4465B00C390 with C705B4465B0000008042C3.
At offset 0415D0, replace C705B4465B0000000045C3 with C705B4465B0000008042C3.


*** Usage ***

The original vertices count limit is 512 per mesh.
By default, the dll will use a value of 65536.
You can modify this value in hook_opt_limit.cfg (Size = value).

The size of the execute buffer is increased from 0x10000 to 0x200000.

To regenerate the "spec.rci" file, delete it. The file will be generated when you launch the game.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
