xwa_hook_countermeasures

This hook fixes countermeasures count when reloading.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_countermeasures.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that fixes countermesures count
At offset 05FE6C, replace 8D04D08A8440B3B65B00 with 5753E8BD80140083C408.
At offset 0B0945, replace 8D04D08A8440B3B65B00 with 5051E8E4750F0083C408.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
