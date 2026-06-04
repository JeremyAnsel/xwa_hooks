xwa_hook_weapon_lock

This hook defines the weapon lock settings.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_weapon_lock.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that sets warhead lock settings
At offset 08F20B, replace 8B0DC4337B00 with E8108D110090.
At offset 08F241, replace BB6CBA0300 with E8DA8C1100.
At offset 08F23A, replace BB487C0200 with E8E18C1100.
At offset 08F35D, replace 25FFFF0000 with E8BE8B1100.
At offset 06B240, replace 81E6FFFF0000 with E8DBCC130090.
At offset 03BDCC, replace 81E1FFFF0000 with E84FC1160090.
At offset 0910B6, replace B8BF3CB622 with E8656E1100.
At offset 0A6EFA, replace C7442428AD8D0100 with E821101000909090.
At offset 0A6F30, replace 056CBA0300 with E8EB0F1000.
At offset 0A74E4, replace 2BF0C1E603 with E8370A1000.

# To call the hook that sets attack break distances settings
At offset 0AC742, replace BF00140000 with E8D9B70F00.
At offset 0AC79E, replace 7507BF00060000 with E87DB70F009090.
At offset 0AC84F, replace 81FF00200000 with E8CCB60F0090.
At offset 0AC867, replace 6681FE8C00 with E8B4B60F00.
At offset 0A6A12, replace 8B2C85E86D5B00 with E8091510009090.


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".

To set the warhead lock settings, create a file named "FlightModels\[Model]WarheadLock.txt" or create a section named "[WarheadLock]" in "FlightModels\[Model].ini".
The format is:
LockDistance0 = value
LockDistance1 = value
LockDistance2 = value
LockTime = value
LockStrengthDivisor = value
AILockDistance0 = value
AILockDistance1 = value
AILockDistance2 = value
AILockTimeFactor = value

LockDistance0_#### = value
LockDistance1_#### = value
LockDistance2_#### = value
LockTime_#### = value
LockStrengthDivisor_#### = value
AILockDistance0_#### = value
AILockDistance1_#### = value
AILockDistance2_#### = value
AILockTimeFactor_#### = value

#### is the ObjectProfile name.

LockDistance0 is the default value.
LockDistance1 is for ShipCategory_Starship, ShipCategory_Platform, ShipCategory_Container, ShipCategory_Freighter.
LockDistance2 is for ShipCategory_Transport.

AILockDistance0 is the default value.
AILockDistance1 and AILockDistance2 are for ShipCategory_Starship, ShipCategory_Platform, ShipCategory_Freighter, ShipCategory_Transport, ShipCategory_Container, ShipCategory_Armament.
If the AI rank is Veteran then the AILockDistance2 setting is used else the AILockDistance1 setting is used.

The default values are:
LockDistance0 = 101805;
LockDistance1 = 244332;
LockDistance2 = 162888;
LockTime = 708;
LockStrengthDivisor = 236;
AILockDistance0 = 101805;
AILockDistance1 = 203610;
AILockDistance2 = 244332;
AILockTimeFactor = 472;

For the MissileBoat the default LockTime is 354.

To set default settings, create a file named "FlightModels\WarheadLock.txt" or create a section named "[WarheadLock]" in "FlightModels\default.ini".
The format is:
AILockDistance1_Starship = value
AILockDistance2_Starship = value
AILockDistance1_Platform = value
AILockDistance2_Platform = value
AILockDistance1_Freighter = value
AILockDistance2_Freighter = value
AILockDistance1_Transport = value
AILockDistance2_Transport = value
AILockDistance1_Container = value
AILockDistance2_Container = value
AILockDistance1_Armament = value
AILockDistance2_Armament = value

# Attack break distances

To set the attack break distances settings, create a file named "FlightModels\[Model]AttackBreakDistance.txt" or create a section named "[AttackBreakDistance]" in "FlightModels\[Model].ini".
Set default values in "FlightModels\AttackBreakDistance.txt" or in a section named "[AttackBreakDistance]" in "FlightModels\default.ini".

To define the distance for the warheads use the "Warhead" setting.
To define the distance for small targets use the "SmallObject_NoviceRank", "SmallObject_OfficerRank", "SmallObject_VeteranRank", "SmallObject_AceRank", "SmallObject_TopAceRank", "SmallObject_SuperAceRank" settings.
To define the max distance for large targets use the "LargeObject_MaxDistance" setting. To define a distance offset use "Model_[Target]" settings where [Target] is the name of the target craft.
To define the max range before shooting use the "MaxRange_Novice", "MaxRange_Officer", "MaxRange_Veteran", "MaxRange_Ace", "MaxRange_TopAce", "MaxRange_SuperAce" settings.

The default values are:
Warhead = 5120
SmallObject_NoviceRank = 5120
SmallObject_OfficerRank = 5120
SmallObject_VeteranRank = 4096
SmallObject_AceRank = 3072
SmallObject_TopAceRank = 2304
SmallObject_SuperAceRank = 1536
LargeObject_MaxDistance = 8192
Model_SuperStarDestroyer = 32768
Model_RepairYard = 32768
Model_ShipYard = 32768
MaxRange_Novice = 24576
MaxRange_Officer = 32768
MaxRange_Veteran = 40960
MaxRange_Ace = 40960
MaxRange_TopAce = 40960
MaxRange_SuperAce = 40960


*** Credits ***

- Jérémy Ansel (JeremyaFr)

