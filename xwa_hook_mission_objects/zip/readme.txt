xwa_hook_mission_objects

This hook permits to replace mission objects and to enable crafts turrets.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_mission_objects.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that loads mission objects
At offset 3117C, replace E8BFAB0900 with E8AF6D1700.
At offset 10DBDD, replace E85EE1FBFF with E84EA30900.
At offset 30A1A, replace E821B30900 with E811751700.
At offset 30C59, replace E8E2B00900 with E8D2721700.
At offset 30D05, replace E836B00900 with E826721700.
At offset 30EA8, replace E893AE0900 with E883701700.

# To call the hook that set crafts turret
At offset 101190, replace 535556578B7C2414 with E88B6D0A00C39090.


*** Usage ***

# mission objects
Suppose that the mission is "[MissionDir]\[Mission].tie".

The possible involved files are:
- "[MissionDir]\[Mission].ini"
- "[MissionDir]\[Mission]_Objects.txt"
- "FlightModels\Objects.txt"

To replace the objects, create a file named "[MissionDir]\[Mission]_Objects.txt" or create a section named "[Objects]" in "[MissionDir]\[Mission].ini".
The dll searches a "Objects.txt" file in this order (from first to last):
- "[MissionDir]\[Mission]_Objects.txt"
- "[MissionDir]\[Mission].ini", section "[Objects]"
- "FlightModels\Objects.txt"
If a "Objects.txt" file doesn't exist, no model is replaced.
The format is a line per object: "FlightModels\ObjectA = FlightModels\NewObjA"
See "Objects.txt".

# crafts turrets
The craft turrets are no longer limited to CorellianTransport2, MilleniumFalcon2, and FamilyTransport. You can now enable turrets for any craft.

Suppose that the player craft is "FlightModels\[Player].opt".

The possible involved files are:
- "FlightModels\[Player]Gunner.opt"
- "FlightModels\[Player]Gunner2.opt"
- "FlightModels\CorellianTransportGunner.opt"

To define a custom gunner opt, create a "[Player]Gunner.opt" file.
To define a second custom gunner opt, create a "[Player]Gunner2.opt" file.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
