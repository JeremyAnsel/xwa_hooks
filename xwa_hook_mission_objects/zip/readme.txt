xwa_hook_mission_objects

This hook permits to replace mission objects and to enable crafts turrets.


*** Requirements ***

This dll requires:
- Windows 7 or superior
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

# To call the hook that filters turret weapons
At offset 0FB941, replace 0F85B8160000 with 909090909090.
;At offset 0903FE, replace 0F8591000000 with 909090909090.
At offset 090452, replace E869020000 with E8C97A1100.
At offset 090466, replace E855020000 with E8B57A1100.
At offset 09048B, replace E830020000 with E8907A1100.
At offset 0904EA, replace E8D1010000 with E8317A1100.
At offset 09057C, replace E83F010000 with E89F791100.
At offset 0905E9, replace E802070000 with E832791100.
At offset 0904D9, replace 7517 with 9090.
At offset 904FD, replace 75D7 with 9090.

# To call the hook that set turrets data
At offset 014EF6, replace E8F5870F00 with E835301900.
At offset 01C6B7, replace 33C933C0BA02000000 with E864B81800EB419090.
;At offset 01E22F, replace B9FE000000F3A5A48B0DC4337B00 with 5657E8EA9C180083C40890909090.
At offset 01E1FE, replace 8D44AD00C1E0032BC5 with E82D9D180090909090.
At offset 0FA0E6, replace 0FBF933B958B008B75EC8B45F48955E0 with 5355E833DE0A0083C408E9B20A000090.
At offset 080747, replace 0FBF8C46BF030000 with 56E8D37712005990.
At offset 080717, replace 0FBF8C46C3030000 with 56E8037812005990.
At offset 0831F3, replace 0FBF8456BF030000 with 56E8274D12005890.
At offset 0831C6, replace 0FBF8456C3030000 with 56E8544D12005890.
At offset 0915A0, replace 668B9C41C1030000 with 55E87A6911005B90.
At offset 0915C9, replace 668B8441C5030000 with 55E8516911005890.
At offset 091939, replace 0FBF8C01C1030000 with 55E8E16511005990.
At offset 0919CF, replace 0FBF9408C5030000 with 55E84B6511005A90.
At offset 096C5D, replace 0FBF8C4EBF030000 with 56E8BD1211005990.
At offset 096D1C, replace 0FBF8C56C3030000 with 56E8FE1111005990.
At offset 00CB4A, replace 0FBF8451C1030000 with 51E8D0B319005890.
At offset 00CBE1, replace 0FBF8C42C5030000 with 52E839B319005990.
At offset 03EE55, replace 668B8C47BF030000 with 56E8C59016005990.
At offset 03EE6F, replace 668BB8C3030000 with 56E8AB9016005F.

# To call the hook that read exe crafts turrets data
At offset 059F27, replace 668B8450D0B65B00 with B83D010000909090.
At offset 061C33, replace 668B8440D2B65B00 with B83D010000909090.
At offset 09178B, replace 668B8408D2B65B00 with B83D010000909090.
At offset 096B68, replace 668BB0D2B65B00 with BE3D0100009090.
At offset 096B2A, replace 8D0450 with 909090.
At offset 096B33, replace 0FBF88C6B65B00 with 50E8E713110059.
At offset 096B3C, replace 0FBF90BEB65B00 with 50E8DE1311005A.
At offset 096B53, replace 0FBF90C2B65B00 with 50E8C71311005A.
At offset 00C9DF, replace 668B9C42D2B65B00 with BB3D010000909090.
At offset 015D77, replace 668B8440D2B65B00 with B83D010000909090.
At offset 030B3C, replace 668B8440D2B65B00 with B83D010000909090.
At offset 03EE07, replace 668B88C8B65B00668B90CCB65B00 with 5056E812911600595A9090909090.
At offset 106FAA, replace 50E88052FDFF25FFFF000083C4048D14808D0CD0 with 50E8800F0A0083C40485C00F849E010000EB2390.
At offset 0C342F, replace 6683B8D2B65B00007424EB18 with E8FC4A0E009090907424EB28.
At offset 0C34A0, replace 3B404C00 with 21404C00.
At offset 0E8E65, replace 668B8448D0B65B00 with B83D010000909090.
At offset 0EF00E, replace 668B8448D0B65B00 with B83D010000909090.
At offset 0EFC0D, replace 668B8450D0B65B00 with B83D010000909090.

# To call the hook that set turret index
At offset 080124, replace 6683B8F9968B0002 with E8077E120085C090.
At offset 09038D, replace 7546 with EB46.
At offset 0902F3, replace 8BAEE0948B0081FDFFFF0000 with 53E8377C11005D85C0909090.

# To call the hook that defines object profile
At offset 01BD27, replace 803D04979E00050F85A7000000 with E8F4C11800909090E9A7000000.
At offset 056351, replace E85AA7FAFF with E8CA1B1500.
At offset 10DE08, replace 66891D0F267800 with E813A109009090.


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

To define the cockpit POV for a craft, you can either directly define the coordinates for the craft int the "CockpitPov" section, either define profiles and use them on a per-mission basis.

Suppose that the new craft is "FlightModels\[Model].opt".
To define the cockpit POV, set CockpitPovX, CockpitPovY and CockpitPovZ in a file named "FlightModels\[Model]CockpitPov.txt" or create a section named "[CockpitPov]" in "FlightModels\[Model].ini".
See "CorellianTransport2CockpitPov.txt"

To create a POV profile named "Profile1", create a file named "FlightModels\[Model]CockpitPov_Profile1.txt" or create a section named "CockpitPov_Profile1" in "FlightModels\[Model].ini".
The format is
CockpitPovX = integer
CockpitPovY = integer
CockpitPovZ = integer
See "CorellianTransport2CockpitPov_Default.txt"

Suppose that the new craft is "FlightModels\[Model].opt".
Suppose that the mission is "[MissionDir]\[Mission].tie".
To replace the cockpit POV profile, create a file named "[MissionDir]\[Mission]_Objects.txt" or create a section named "[Objects]" in "[MissionDir]\[Mission].ini".
The format is
FlightModels\[Model]_CockpitPovProfile = ProfileName
CockpitPovProfile = ProfileName
When "FlightModels\[Model]_CockpitPovProfile" doesn't exist, "CockpitPovProfile" is used. If "CockpitPovProfile" doesn't exist, "Default is used".
See "Objects.txt".

Cockpit POV position is read in this order:
- [Model]CockpitPov
- [Model]_CockpitPovProfile
- CockpitPovProfile
- Default profile

# crafts turrets
The craft turrets are no longer limited to CorellianTransport2, MilleniumFalcon2, and FamilyTransport. You can now enable turrets for any craft.

Suppose that the player craft is "FlightModels\[Player].opt".

The possible involved files are:
- "FlightModels\[Player]Turrets.txt"
- "FlightModels\[Player].ini", section "[Turrets]"
- "FlightModels\[Player]Gunner.opt"
- "FlightModels\[Player]Gunner2.opt"
- "FlightModels\CorellianTransportGunner.opt"

To define the turrets, create a file named "FlightModels\[Player]Turrets.txt" or create a section named "[Turrets]" in "FlightModels\[Player].ini".
If the file does not exist, default values are read from XWingAlliance.exe.
The format is a line per turret:
PositionX, PositionY, PositionZ, OrientationX, OrientationY, ArcX, ArcY, OptModel, IsInverted

If a turrets section is not used, there is a limit of 2 turrets per craft.
To define a custom gunner opt, create a "[Player]Gunner.opt" file.
To define a second custom gunner opt, create a "[Player]Gunner2.opt" file.

See "FamilyTransportTurrets.txt".

To disable the turret forward fire, set "TurretForwardFire = 0".
The default value is "1".

See "EscortShuttleTurrets.txt".

By default, the turrets lasers are unlimited. They can be made limited by setting "UnlimitedTurretLaser = 0".
See the "hook_mission_objects.cfg" file or the "hook_mission_objects" section of "hooks.ini".

# object profiles
Suppose that the new craft is "FlightModels\[Model].opt".
To hide meshes for a craft, create a file named "FlightModels\[Model]ObjectProfiles.txt" or create a section named "[ObjectProfiles]" in "FlightModels\[Model].ini".
The format is:
ProfileName = indices
indices are a comma separated list.
See "ModelObjectProfiles.txt"

Suppose that the mission is "[MissionDir]\[Mission].tie".
To define an object profile for a flightgroup, create a file named "[MissionDir]\[Mission]_Objects.txt" or create a section named "[Objects]" in "[MissionDir]\[Mission].ini".
The format is
ObjectProfile_fg_# = ProfileName
Replace # with the flightgroup index.
The default ProfileName is "Default".
See "Objects.txt"


*** Credits ***

- Jérémy Ansel (JeremyaFr)
