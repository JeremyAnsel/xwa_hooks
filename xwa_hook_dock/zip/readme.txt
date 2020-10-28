xwa_hook_dock

This hook fixes the docking position.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_dock.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that reads the dock position
At offset 03204C, replace 898EEAB65B00 with E8CF5E170090.

# To call the hook that fixes the ai dock position
At offset 0AFD71, replace E83A26FFFF with E8AA810F00.
At offset 0B0015, replace E89623FFFF with E8067F0F00.
At offset 0959DA, replace E8D1C90000 with E841251100.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
