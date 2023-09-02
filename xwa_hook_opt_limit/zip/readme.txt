xwa_hook_opt_limit

This hook removes the opt limit of 512 vertices per mesh.
It also generates the "spec.rci" file.
The meshes count per opt is now 254.
The engines count per opt is now 255.


*** Requirements ***

This dll requires:
- Windows 7 or superior
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

# disable hitzone bounds check on render
At offset 081A38, replace 7562 with EB62.

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

# To call the hook that sets the craft size
See "exe_edit_meshes.txt"

# To call the hook that sets the craft offsets
See "exe_edit_meshes.txt"

# To call the hook that compares the meshes count
See "exe_edit_meshes.txt"

# To call the hook that sets the meshes info
See "exe_edit_meshes.txt"

# To call the hook that sets the cockpit opt meshes info
See "exe_edit_meshes.txt"

# To call the hook that sets the exterior opt meshes info
See "exe_edit_meshes.txt"

# To call the hook that inits the escape pod craft
See "exe_edit_meshes.txt"

# To call the hook that inits the current craft
See "exe_edit_meshes.txt"

# To call the hook that filters craft meshes
See "exe_edit_meshes.txt"

# To call the hook that zero inits the craft memory
See "exe_edit_meshes.txt"

# To call the hook that sets the craft engines count
See "exe_edit_engines.txt"

# To call the hook that gets the craft engine mesh
See "exe_edit_engines.txt"

# To call the hook that selects the lod version
At offset 0819B9, replace A1C4277800 with E862651200

# To call the hook that computes the checksum
At offset 0F5010, replace 83EC14A178AE910053C1F8045589442410A148099100568B3570AE9100575033DB68BCFA5F006800000200895C241C with 33EDBA100000008D3CAD201277002BD533C0C1E2028BCA8D34ADD80E7700C1E902F3AB8BCA8BFEC1E902F3AB33C0C3.


*** Usage ***

The original vertices count limit is 512 per mesh.
By default, the dll will use a value of 65536.
You can modify this value in the "hook_opt_limit.cfg" file or the "hook_opt_limit" section of "hooks.ini".
The format is:
HitDataArraySize = value

The size of the execute buffer is increased from 0x10000 to 0x200000.

To regenerate the "spec.rci" file, delete it. The file will be generated when you launch the game.

To use default values for the meshes count limit, set "MeshesCount = 0".

Suppose that the craft is "FlightModels\[Model].opt".
To define which engine glows are rendered based on the craft speed, create a file named "FlightModels\[Model]EngineGlows.txt" or create a section named "[EngineGlows]" in "FlightModels\[Model].ini".
The format is a line per engine:
engine index, percent, mglt.
If the percent value is -1 then it is not used.
If the mglt value is -1 then it is not used.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
