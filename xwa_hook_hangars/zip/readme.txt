xwa_hook_hangars

This hook permits to customize the hangars.
The XWA's engine uses two hangar models, Hangar.opt and FamilyBase.opt. If you wanted to use a custom hangar, you had to replace one of them. You can now choose which hangar to use, customize the objects and the camera positions, for a specific craft or for a specific mission.
You can also define which crafts appear in the craft selection menu.

For more informations, please see the following thread at xwaupgrade.com:
http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11197


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_hangars.dll and hook_hangars.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that selects the hangar type
At offset 17DF97, replace 390C858E2AAE000F92C2 with 50E8939F02008AD05890.

# To call the hook that selects the inside animation
At offset 89AF, replace 663D8700740A663D9400 with 50E87BF5190085C05890.

# To call the hook that selects the hangar model index
At offset 5737A, replace 663D8700740F663D9400 with 50E8B00B150085C05890.

# To call the hook that loads hangar opt
At offset 056DEE, replace E84D4F0700 with E83D111500.

# To call the hook that reloads hangar opt
At offset 054EA4, replace 750E50E8F4140000 with 909050E874301500.

# To call the hook that creates hangar object
At offset 055EF7, replace 66391C7DE0A67C007522 with 57E82320150083C40490.

# To call the hook that reloads the hangar
At offset 057021, replace A10C337700 with E80A0F1500.

# To remove the opt filter
At offset 563C3, replace 83FB0E0F8FCB0000000F84 with 33C089442410E9BF090000.

# To call the hook that sets the hangar camera
At offset 5F0A0, replace 81C76A04000081ED1009000081EE2C010000 with 565557E8788E14005F5D5E90909090909090.
At offset 5F7E8, replace B7FC4500 with 7EFC4500.
At offset 5F7EC, replace CFFE4500 with 7EFC4500.
At offset 5F7F0, replace F0FC4500 with 7EFC4500.
At offset 5F7F4, replace F7FC4500 with 7EFC4500.
At offset 5F7F8, replace 26FD4500 with 7EFC4500.
At offset 5F7FC, replace 5FFD4500 with 7EFC4500.
At offset 5F800, replace 08FF4500 with 7EFC4500.
At offset 5F804, replace 4DFF4500 with 7EFC4500.
At offset 5F86C, replace 08004600 with 7EFC4500.
At offset 5F870, replace 41004600 with 7EFC4500.
At offset 5F874, replace 7A004600 with 7EFC4500.
At offset 5F87C, replace E2004600 with 7EFC4500.
At offset 5F880, replace CD014600 with 7EFC4500.
At offset 5F884, replace 06024600 with 7EFC4500.
At offset 5F888, replace 3C024600 with 7EFC4500.
At offset 5F88C, replace 6F024600 with 7EFC4500.

# To call the hook that loads the shuttle
At offset 557D8, replace E803070000 with E853271500.

# To call the hook that updates the shuttle
At offset 5B5A5, replace E8C62A0000 with E876C91400.

# To call the hook that sets the shuttle re-enter position
At offset 57ADB, replace 8D1480C1E2032BD0A108BC6800 with E840041500E9B4000000909090.

# To call the hook that reads the shuttle opt infos
At offset 57A05, replace E8A68EFDFFE85193FDFF with E8160515009090909090.

# To call the hook that loads the droids
At offset 5570D, replace A110BC6800536870E50000 with E80E281500E98600000090.
At offset 557A3, replace 898680679C00 with 909090909090.
At offset 557AE, replace 899E84679C00 with 909090909090.
At offset 557B9, replace 899EA6679C00 with 909090909090.
At offset 557C4, replace 899EA2679C00 with 909090909090.
At offset 557CC, replace 899E88679C00891510BC6800 with 909090909090909090909090.

# To call the hook that loads the Hangar Roof Crane
At offset 55835, replace E8A6060000 with E8F6261500.

# To call the hook that sets the hangar map
At offset 55345, replace 68C896000068FFFFFF7F68 with 58E8D52B1500E9BD030000.

# To call the hook that sets the family hangar map
At offset 55873, replace 688877000068FFFFFF7F68 with 58E8A7261500E98E020000.

# To call the hook that sets the craft elevation
At offset 61A49, replace 83F804740F50E8CC310200 with 50E8E164140083C404C390.
At offset 55183, replace 83F804741C50E892FA0200 with 50E8B7C8000083C404EB1B.
At offset 562F4, replace 83F804741050E821E90200 with 50E846B7000083C404EB0F.
At offset 5AF8C, replace 83F804741650E8899C0200 with 50E8AE6A000083C404EB15.
At offset 5B203, replace 83F804741650E8129A0200 with 50E83768000083C404EB15.
At offset 5B350, replace 83F804741650E8C5980200 with 50E8EA66000083C404EB15.
At offset 5BDA4, replace 83F804741050E8718E0200 with 50E8965C000083C404EB0F.

# To call the hook that sets the craft selection
At offset 54A69, replace 6683F93A0F84400100006683F941 with 5150E8B03415005859E9EA010000.
At offset 54B39, replace 80FA02BE02000000755A6683F901740B with 525150E8DF33150058595AE918010000.

# To call the hook that sets the hangar launch animation
At offset 05AC31, replace 03CB894C300F with E8EAD2140090.
At offset 05ACFA, replace 81C2F70000003BC20F8ED9070000 with E831D214009085C00F84D9070000.

# To call the hook that sets the hangar objects elevation
At offset 056314, replace 8B15C4337B0003C18944320F with 5055E8051C150083C4089090.
At offset 0551B8, replace 2BF8893D38BC6800 with 50E8622D15005890.
At offset 05BDC4, replace 8B15C4337B0003C18944320F with 50E856C11400589090909090.

# To call the hook that sets the hangar re-enter animation
At offset 0576EF, replace A138BC68008D8823020000 with E83C0815008BC890909090.
At offset 05AF69, replace 81C2930000003954350F7E7A with E8C2CF140090909085C0747A.
At offset 05AFAC, replace 8B1538BC68008B7C350F03C23BF87E33 with 50E87ECF14008B7C350F85C058907433.
At offset 05AFE3, replace 2BF8897C350F8B2DC4337B00 with 50E837CF1400589090909090.
At offset 05B223, replace 8B1538BC68008B4C370F with 50E8F7CC140058EB1590.
At offset 05B30C, replace 2BCA894C300F with E80FCC140090.
At offset 05B370, replace 8B1538BC680003C28B54310F3BD00F8F5F010000 with 50E8BACB140083C40490909085C00F845F010000.
At offset 05B40D, replace E82E660000 with E81ECB1400.

# To call the hook that sets the shuttle launch and re-enter animations
At offset 05E598, replace 0FAFC301443507 with 53E8829914005B.
At offset 05E76D, replace 0FAFC301443507 with 53E8AD9714005B.
At offset 05E8E9, replace D80DF8955A00 with D80DD0955A00.
At offset 05E4CF, replace A1C4337B008B4C010B with 51E85B9A1400599090.
At offset 05E13C, replace 8D14098B0803CA8908 with 51E8DE9D1400599090.
At offset 05E25F, replace 81C2610100003954300F0F8E870D0000 with E8CC9C14009085C090900F85870D0000.

At offset 05E923, replace 7E03 with EB0F.
At offset 05EAA6, replace 8B540A0B81C2BF030000 with 50E8749414005A909090.
At offset 05EF69, replace 8B540A0B81C2BF030000 with 50E8B18F14005A909090.
At offset 05EFB1, replace 66C74430170000 with E86A8F14009090.
At offset 05EB99, replace F7D9D1E103D18910 with 51E8819314005990.
At offset 05ECA1, replace 8B1538BC680081C28A000000 with 50E8899214008BD058909090.
At offset 05ECAD, replace 3954300F0F8F3F030000 with 85D290900F853F030000.

# To call the hook that gets the craft index
At offset 0590BA, replace E871310800 with E871EE1400.
At offset 0590C9, replace 7467 with EB67.

# To call the hook that disable shadow when inverted
At offset 05A319, replace E8D2870200 with E802DC1400.


*** Usage ***

# hook_hangars.cfg:
hook_hangars.cfg is a config file.

The SelectionMode option determines how a hangar is selected.
The possible values are:
- ForceHangar
- BasedOnModelIndex
- BasedOnModelIndexAzzameenCampaign

The default value is BasedOnModelIndexAzzameenCampaign

When SelectionMode = ForceHangar:
hangar = Hangar

When SelectionMode = BasedOnModelIndex:
if mission type is family
  hangar = FamilyBase
else
  hangar = Hangar
endif

When SelectionMode = BasedOnModelIndexAzzameenCampaign:
if mission type is family and mission id < 7
  hangar = FamilyBase
else
  hangar = Hangar
endif

The ProvingGroundHangarModel option set the proving ground hangar opt model.
The default value is "" or "FlightModels\".
Example:
ProvingGroundHangarModel = FlightModels\ProvingGround.opt
This will load "FlightModels\ProvingGroundHangar.opt" and "FlightModels\ProvingGround.ini" (or txt files). "FlightModels\ProvingGround.opt" may not exist.

# hangar files:

Suppose that the mission is "[MissionDir]\[Mission].tie".
Suppose that the arrival craft is "FlightModels\[Model].opt".
Suppose that the player craft is "FlightModels\[Player].opt".

The possible involved files are:
- "[MissionDir]\[Mission].ini"
- "[MissionDir]\[Mission]_Hangar.opt"
- "[MissionDir]\[Mission]_HangarObjects.txt"
- "[MissionDir]\[Mission]_HangarCamera.txt"
- "[MissionDir]\[Mission]_FamHangarCamera.txt"
- "[MissionDir]\[Mission]_HangarMap.txt"
- "[MissionDir]\[Mission]_FamHangarMap.txt"
- "FlightModels\[Model].ini"
- "FlightModels\[Model]Hangar.opt"
- "FlightModels\[Model]HangarObjects.txt"
- "FlightModels\[Model]HangarCamera.txt"
- "FlightModels\[Model]FamHangarCamera.txt"
- "FlightModels\[Model]HangarMap.txt"
- "FlightModels\[Model]FamHangarMap.txt"
- "FlightModels\default.ini"
- "FlightModels\Hangar.opt"
- "FlightModels\HangarObjects.txt"
- "FlightModels\HangarCamera.txt"
- "FlightModels\FamHangarCamera.txt"
- "FlightModels\HangarMap.txt"
- "FlightModels\FamHangarMap.txt"
- "FlightModels\[Player].ini"
- "FlightModels\[Player]Camera.txt"
- "FlightModels\[Player]Size.txt"


To use a custom hangar, name it "FlightModels\[Model]Hangar.opt" or "[MissionDir]\[Mission]_Hangar.opt".
The dll searches a "Hangar.opt" file in this order (from first to last):
- "[MissionDir]\[Mission]_Hangar.opt"
- "FlightModels\[Model]Hangar.opt"
- "FlightModels\Hangar.opt"

To replace the objects, create a file named "FlightModels\[Model]HangarObjects.txt" or "[MissionDir]\[Mission]_HangarObjects.txt".
Or create a section named "[HangarObjects]" in "FlightModels\[Model].ini" or "[MissionDir]\[Mission].ini".
The dll searches a "HangarObjects.txt" file in this order (from first to last):
- "[MissionDir]\[Mission]_HangarObjects.txt"
- "[MissionDir]\[Mission].ini", section "[HangarObjects]"
- "FlightModels\[Model]HangarObjects.txt"
- "FlightModels\[Model].ini", section "[HangarObjects]"
- "FlightModels\HangarObjects.txt"
- "FlightModels\default.ini", section "[HangarObjects]"
If a "HangarObjects.txt" file doesn't exist, no model is replaced.
The format is a line per object: "FlightModels\ObjectA = FlightModels\NewObjA"
The replacement of a base opt with an exterior opt is ignored: lines like "FlightModels\ObjectA.opt = FlightModels\ObjectAExterior.opt" are ignored.
To remove an object, use an empty invisible opt.
To not load the shuttle, set "LoadShuttle = 0".
To set the shuttle model index, set "ShuttleModelIndex = value". value is an integer. The default value is 50 (Shuttle).
To set the shuttle markings, set "ShuttleMarkings = value". value is an integer. The default value is 0.
To set the shuttle x position, set "ShuttlePositionX = value". value is an integer. The default value is 1127.
To set the shuttle y position, set "ShuttlePositionY = value". value is an integer. The default value is 959.
To set the shuttle z position, set "ShuttlePositionZ = value". value is an integer. The default value is 0.
To invert the hangar floor for the shuttle, set "IsShuttleFloorInverted = 1". When set to 0, the floor is below the player craft. When set to 1, the floor is above the player craft.
To set the shuttle animation, set "ShuttleAnimation = value". value is "Right", "Top", or "Bottom". The default value is "Right".
To set the shuttle straight line animation length, set "ShuttleAnimationStraightLine = value". value is an integer. The default value is 0.
To not load the droids, set "LoadDroids = 0".
To set the droids z position, set "DroidsPositionZ = value". value is an integer. The default value is 0.
To invert the hangar floor for the droids, set "IsDroidsFloorInverted = 1". When set to 0, the floor is below the player craft. When set to 1, the floor is above the player craft.
To set the Hangar Roof Crane position, set "HangarRoofCranePositionY = value Y" and "HangarRoofCranePositionZ = value Z". The values are integers. The default value for HangarRoofCranePositionY is 786. The default value for HangarRoofCranePositionZ is -282.
To invert the hangar floor, set "IsHangarFloorInverted = 1". When set to 0, the floor is below the player craft. When set to 1, the floor is above the player craft.
To set the elevation delta of the player craft animation, set "PlayerAnimationElevation = value". value is an integer. The default value is 0.
See "HangarObjects.txt".

Apply to hangar:
To set the position of the camera, create a file named "FlightModels\[Model]HangarCamera.txt" or "[MissionDir]\[Mission]_HangarCamera.txt".
Or create a section named "[HangarCamera]" in "FlightModels\[Model].ini" or "[MissionDir]\[Mission].ini".
The dll searches a "HangarCamera.txt" file in this order (from first to last):
- "[MissionDir]\[Mission]_HangarCamera.txt"
- "[MissionDir]\[Mission].ini", section "[HangarCamera]"
- "FlightModels\[Model]HangarCamera.txt"
- "FlightModels\[Model].ini", section "[HangarCamera]"
- "FlightModels\HangarCamera.txt"
- "FlightModels\default.ini", section "[HangarCamera]"
If a "HangarCamera.txt" file doesn't exist, default values are used.
Only the position for keys 1,2,3,6,9 can be set with a "HangarCamera.txt" file.
The format is:
"Key1_X = value"
"Key1_Y = value"
"Key1_Z = value"
See "HangarCamera.txt".

Apply to family base:
To set the position of the camera, create a file named "FlightModels\[Model]FamHangarCamera.txt" or "[MissionDir]\[Mission]_FamHangarCamera.txt".
Or create a section named "[FamHangarCamera]" in "FlightModels\[Model].ini" or "[MissionDir]\[Mission].ini".
The dll searches a "FamHangarCamera.txt" file in this order (from first to last):
- "[MissionDir]\[Mission]_FamHangarCamera.txt"
- "[MissionDir]\[Mission].ini", section "[FamHangarCamera]"
- "FlightModels\[Model]FamHangarCamera.txt"
- "FlightModels\[Model].ini", section "[FamHangarCamera]"
- "FlightModels\FamHangarCamera.txt"
- "FlightModels\default.ini", section "[FamHangarCamera]"
If a "FamHangarCamera.txt" file doesn't exist, default values are used.
Only the position for keys 1,2,3,6,7,8,9 can be set with a "FamHangarCamera.txt" file.
The format is:
"FamKey1_X = value"
"FamKey1_Y = value"
"FamKey1_Z = value"
See "FamHangarCamera.txt".

To set the position of the camera for key 7 (hangar) or key 5 (family base), create a file named "FlightModels\[Player]Camera.txt".
Or create a section named "[Camera]" in "FlightModels\[Player].ini".
If a such file file doesn't exist for a model, default values are used.
The format is:
"X = value"
"Y = value"
"Z = value"
"FamX = value"
"FamY = value"
"FamZ = value"
See "PlayerCamera.txt".

Apply to hangar:
To set the hangar map, create a file named "FlightModels\[Model]HangarMap.txt" or "[MissionDir]\[Mission]_HangarMap.txt".
Or create a section named "[HangarMap]" in "FlightModels\[Model].ini" or "[MissionDir]\[Mission].ini".
The dll searches a "HangarMap.txt" file in this order (from first to last):
- "[MissionDir]\[Mission]_HangarMap.txt"
- "[MissionDir]\[Mission].ini", section "[HangarMap]"
- "FlightModels\[Model]HangarMap.txt"
- "FlightModels\[Model].ini", section "[HangarMap]"
- "FlightModels\HangarMap.txt"
- "FlightModels\default.ini", section "[HangarMap]"
If a "HangarMap.txt" file doesn't exist, default values are used.
The file must contain at least 4 object line.
The format is : model index, position X, position Y, position Z, heading XY, heading Z.
or : model index, markings, position X, position Y, position Z, heading XY, heading Z.
The numbers can be  written in decimal or hexadecimal (0x) notation.
When position Z is set to 0x7FFFFFFF, this means that the object stands at the ground.
See "HangarMap.txt".

Apply to family base:
To set the family hangar map, create a file named "FlightModels\[Model]FamHangarMap.txt" or "[MissionDir]\[Mission]_FamHangarMap.txt".
Or create a section named "[FamHangarMap]" in "FlightModels\[Model].ini" or "[MissionDir]\[Mission].ini".
The dll searches a "FamHangarMap.txt" file in this order (from first to last):
- "[MissionDir]\[Mission]_FamHangarMap.txt"
- "[MissionDir]\[Mission].ini", section "[FamHangarMap]"
- "FlightModels\[Model]FamHangarMap.txt"
- "FlightModels\[Model].ini", section "[FamHangarMap]"
- "FlightModels\FamHangarMap.txt"
- "FlightModels\default.ini", section "[FamHangarMap]"
If a "FamHangarMap.txt" file doesn't exist, default values are used.
The file must contain at least 4 object line.
The format is : model index, position X, position Y, position Z, heading XY, heading Z.
or : model index, markings, position X, position Y, position Z, heading XY, heading Z.
The numbers can be  written in decimal or hexadecimal (0x) notation.
When position Z is set to 0x7FFFFFFF, this means that the object stands at the ground.
See "FamHangarMap.txt".

To set the elevation of the craft with closed S-Foils, create a file named "FlightModels\[Player]Size.txt".
Or create a section named "[Size]" in "FlightModels\[Player].ini".
If the hangar floor is inverted, the ClosedSFoilsElevationInverted setting is used, else the ClosedSFoilsElevation setting is used.
If a such file file doesn't exist for a model, default values are used:
if the craft is BWing, elevation = 50, else elevation = model height / 2.
The format is:
"ClosedSFoilsElevation = value"
"ClosedSFoilsElevationInverted = value"
See "BWingSize.txt".

# craft selection:

To define the list of crafts that appear in the hangar crafts selection menu, create the following files (next to XWingAlliance.exe):
- "CraftSelectionTransports.txt"
- "CraftSelectionFighters.txt"
- "CraftSelectionCrafts.txt"
If the files don't exist, default values are used.
CraftSelectionTransports define the crafts that appear when the mission player craft is a transport.
CraftSelectionFighters define the crafts that appear when the mission player craft is a fighter.
CraftSelectionCrafts define the crafts that appear in the Proving Ground.
See "CraftSelectionTransports.txt", "CraftSelectionFighters.txt", "CraftSelectionCrafts.txt"


*** Notes ***

# Hangar views
For Hangar:
Key 0 is the cockpit view.
Keys 1,2,3,6,9 are fixed views.
Keys 4 and 5 are related to the hangar droids.
Key 7 is related to the player model.
Key 8 is related to the hangar roof crane.

For FamilyBase:
Key 0 is the cockpit view.
Keys 1,2,3,6,7,8,9 are fixed views.
Key 4 is related to the work droid 1.
Key 5 is related to the player model.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
