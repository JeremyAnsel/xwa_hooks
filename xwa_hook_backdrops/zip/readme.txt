xwa_hook_backdrops

This hook adds starfield backdrops to skirmish mission and permits to replace .dat files for a given mission.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_backdrops.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that adds starfield backdrops
At offset 17A3D9, replace 33DB85C08944241C0F8E4D0300008B74241469F63E0E0000 with 8D5C24145350E83CDB020083C408E9480300009090909090.

# To call the hook that loads mission
At offset 10F74A, replace E81154F0FF with E8E1870900.
At offset 11091F, replace E83C42F0FF with E80C760900.


*** Usage ***

Suppose that the mission is "[MissionDir]\[Mission].tie".
To replace dat images, create a file named "[MissionDir]\[Mission]_Resdata.txt" or create a section named "[Resdata]" in "[MissionDir]\[Mission].ini" and list your new dat files.
The format of this file is the same as "Resdata.txt".


*** Credits ***

- Jérémy Ansel (JeremyaFr)
