xwa_hook_opt_limit

This hook removes the opt limit of 512 vertices per mesh.


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


*** Usage ***

The original vertices count limit is 512 per mesh.
By default, the dll will use a value of 65536.
You can modify this value in hook_opt_limit.cfg (Size = value).

The size of the execute buffer is increased from 0x10000 to 0x200000.

*** Credits ***

- Jérémy Ansel (JeremyaFr)
