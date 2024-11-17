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


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".

To set the warhead losk settings, create a file named "FlightModels\[Model]WarheadLock.txt" or create a section named "[WarheadLock]" in "FlightModels\[Model].ini".
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


*** Credits ***

- Jérémy Ansel (JeremyaFr)

