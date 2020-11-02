xwa_hook_mission_tie

This hook permits to override the data of mission files.
It fixes a bug with music state at mission start.


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
At offset 1C6FF, replace 668B442428 with E82CB81800.

# 'crafts count per region' patch
At offset 014C28, replace C705944B7D0060000000C1E305 with C705944B7D00C0000000C1E306.
At offset 014BF2, replace 8D1CF633D2C1E302 with 8D1C7633D2C1E306.


*** Usage ***

Suppose that the mission is "[MissionDir]\[Mission].tie".

To override the mission data, create a file named "[MissionDir]\[Mission].txt" or create a section named "[mission_tie]" in "[MissionDir]\[Mission].ini".
The format is a line per replacement.

To replace the flight group markings, the format is: "fg", fg number, "markings", value.
To replace the flight group wingman markings, the format is: "fg", fg number, "index", wingman index, "markings", value.
To replace the flight group iff, the format is: "fg", fg number, "iff", value.
To replace the flight group pilot voice, the format is: "fg", fg number, "pilotvoice", value.

See "mission.txt".


*** Credits ***

- Jérémy Ansel (JeremyaFr)
