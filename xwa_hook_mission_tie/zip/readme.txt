xwa_hook_mission_tie

This hook permits to override the data of mission files.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_mission_tie.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that override tie mission
At offset 1521A, replace E8E1A40000 with E8112D1900.


*** Usage ***

Suppose that the mission is "[MissionDir]\[Mission].tie".

To override the mission data, create a file named "[MissionDir]\[Mission].txt".
The format is a line per replacement.
To replace the flight group markings, the format is: "fg", fg number, "markings", value.
See "mission.txt".


*** Credits ***

- Jérémy Ansel (JeremyaFr)
