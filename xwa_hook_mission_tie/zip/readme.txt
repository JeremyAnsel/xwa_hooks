xwa_hook_mission_tie

This hook permits to override the data of mission files.
It fixes a bug with music state at mission start.


*** Requirements ***

This dll requires:
- Windows 7 or superior
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

# To call the hook that sets the stats profiles
At offset 01F343, replace E838250800 with E8D88B1800.
At offset 0B80DA, replace 668BB440A0B45B00 with 668BB2C400000090.
At offset 0B810E, replace 8D0CD08D04C8668BBC40A0B45B00 with 909090909090668BB9C400000090.
At offset 02C3E2, replace 668B8440A2B45B00 with E849BB1700909090.
At offset 02C41B, replace 668B9440A2B45B00 with E810BB17008BD090.
At offset 02C518, replace 668B9440A4B45B00 with E813BA17008BD090.
At offset 02C558, replace 668B8440A4B45B00 with E8D3B91700909090.
At offset 02C4B6, replace 66FF8285000000 with E865BA17009090.
At offset 02C5E3, replace 66FF8885000000 with E838B917009090.
At offset 00AC89, replace 8B844095B45B00 with E8A2D219009090.
At offset 00FA35, replace 8D04D0668B84409DB45B00 with 8B450850E8F28419005A90.
At offset 00FBBA, replace 8D04D0668B84409DB45B00 with 8B450850E86D8319005A90.
At offset 072F4A, replace 668B84409DB45B00 with 55E8E04F13005A90.
At offset 0A70EE, replace 8D3CF8896C24308D04F8668B84409DB45B00 with 896C2430A1A0A17C0050E8330E10005F9090.
At offset 0F2F57, replace 8D04D0663B8C409DB45B00 with 57E8D34F0B005A663BC890.
At offset 01E76E, replace 8B8C408FB45B00 with 53E8AC97180059.
At offset 05FAD8, replace 8B8C408FB45B00898BA20100008D84408FB45B008B008983A6010000 with A108BC680050E83D84140059898BA2010000898BA601000090909090.
At offset 072EA5, replace 8B84408FB45B00 with 55E87550130058.
At offset 08F892, replace 8B84408FB45B00 with 53E88886110058.
At offset 08FB01, replace 8B84408FB45B00 with 55E81984110058.
At offset 08FB65, replace 8B84408FB45B00 with 55E8B583110058.
At offset 08FBC4, replace 8BB4408FB45B00 with 55E8568311005E.
At offset 08FC25, replace 8B84408FB45B00 with 55E8F582110058.
At offset 090274, replace 8B15C4337B0025FFFF0000 with 50E8A67C110058D1E0C390.
At offset 0D53AD, replace 8B868FB45B00 with E87E2B0D0090.
At offset 0FC400, replace 8D14C88D04D08B84408FB45B00 with 8B86E0948B0050E814BB0A0058.
At offset 0FC68A, replace 8D0C808D14C88B8BA60100008D04D08B93A20100008B84408FB45B00 with 8B86E0948B008B8BA60100009090908B93A201000050E87BB80A0058.
At offset 01B861, replace 7529 with EB29.
At offset 0B9CAA,, replace 389C408CB45B00 with E881E20E003AC3.
At offset 0BBE6A, replace 8A94408CB45B00 with E8C1C00E008AD0.
At offset 0BD20C, replace 8A8C408CB45B00 with E81FAD0E008AC8.
At offset 01BA3D, replace 754F with EB4F.


*** Usage ***

Suppose that the mission is "[MissionDir]\[Mission].tie".

To override the mission data, create a file named "[MissionDir]\[Mission].txt" or create a section named "[mission_tie]" in "[MissionDir]\[Mission].ini".
The format is a line per replacement.

To replace the flight group markings, the format is: "fg", fg number, "markings", value.
To replace the flight group wingman markings, the format is: "fg", fg number, "index", wingman index, "markings", value.
To replace the flight group iff, the format is: "fg", fg number, "iff", value.
To replace the flight group pilot voice, the format is: "fg", fg number, "pilotvoice", value.

See "mission.txt".

# Stats Profiles

Suppose that the craft is "FlightModels\[Model].opt".
To create a stats profile named "Profile1", create a file named "FlightModels\[Model]StatsProfile_Profile1.txt" or create a section named "StatsProfile_Profile1" in "FlightModels\[Model].ini".
The format is
Speed = integer
Acceleration = integer
SpeedIncrement = integer
Deceleration = integer
SpeedDecrement = integer
Pitch = integer
Roll = integer
Yaw = integer
ExplosionStrength = integer
HullStrength = integer
SystemStrength = integer
ShieldStrength = integer
HasShieldGenerator = integer
HasHyperdrive = integer

See "XWing.ini".

Suppose that the mission is "[MissionDir]\[Mission].tie".
To define a stats profile for a craft, create a file named "[MissionDir]\[Mission]_StatsProfiles.txt" or create a section named "[StatsProfiles]" in "[MissionDir]\[Mission].ini".
Or create a file named "FlightModels\StatsProfiles.txt" or create a section named "[StatsProfiles]" in "FlightModels\default.ini".
The format is
CraftOptName_fgc_# = ProfileName
PlayerSpeedPercent = integer
PlayerAccelerationPercent = integer
PlayerDecelerationPercent = integer
PlayerPitchPercent = integer
PlayerRollPercent = integer
PlayerYawPercent = integer
PlayerExplosionStrengthPercent = integer
PlayerHullStrengthPercent = integer
PlayerSystemStrengthPercent = integer
PlayerShieldStrengthPercent = integer
SpeedPercent = integer
AccelerationPercent = integer
DecelerationPercent = integer
PitchPercent = integer
RollPercent = integer
YawPercent = integer
ExplosionStrengthPercent = integer
HullStrengthPercent = integer
SystemStrengthPercent = integer
ShieldStrengthPercent = integer
# in CraftOptName_fgc_# is an integer for the opt color marking index, starting at 0.
To define a profile for the player craft, set "CraftOptName_fg_player = ProfileName".
The default ProfileName is "Default".
Player*Percent applies to the player craft.
*Percent applies to all crafts.
See "StatsProfiles.txt"

The raw values are calculated as follow:

stats.Speed = (int)(stats.Speed * 2.25f + 0.5f);
stats.Acceleration = (int)(stats.Acceleration * 2.25f + 0.5f);
stats.Deceleration = (int)(stats.Deceleration * 2.25f + 0.5f);
stats.Pitch = stats.Pitch * 256;
stats.Roll = stats.Roll * 256;
stats.Yaw = stats.Yaw * 256;
stats.ExplosionStrength = stats.ExplosionStrength * 105;
stats.HullStrength = stats.HullStrength * 105;
stats.ShieldStrength = stats.ShieldStrength * 50;

if( ShipCategory == ShipCategory_Starship || ShipCategory == ShipCategory_Platform )
{
    HullStrength /= 16;
    ShieldStrength /= 16;
    ExplosionStrength /= 16
}

if( ShipCategory == ShipCategory_Freighter || ShipCategory == ShipCategory_Container )
{
    HullStrength /= 4;
    ShieldStrength /= 4;
    ExplosionStrength /= 4
}


*** Credits ***

- Jérémy Ansel (JeremyaFr)
