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
At offset 17E0CD, replace E8AE94FDFF with E85E9E0200.
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
At offset 094161, replace 7606 with 76D0.
At offset 094133, replace 909090909090909090909090 with 33D20FA4C210C1E010F7F1C3.

# To call the hook that checks mission ids
At offset 05B6E4, replace 668B01663D8700 with E837C814009090.
At offset 05B759, replace 83FE147523 with E8C2C71400.
At offset 09812A, replace 83F9318B4C24107522 with 8B4C2410E8EDFD1000.
At offset 09807A, replace E8C1F0FFFF with E8A1FE1000.
At offset 0980CB, replace E870F0FFFF with E850FE1000.
At offset 097DA4, replace 83F831722683F834 with E877011100EB2490.
At offset 10086D, replace 0F8411040000 with E8AE760A0090.
At offset 0F88D5, replace 390C858E2AAE007539 with E846F60A0090909090.
At offset 0F88E5, replace 7530 with 9090.
At offset 0F88EE, replace 7527 with 9090.
At offset 0F88F7, replace 751E with 9090.
At offset 0902FF, replace 0F849C030000 with E81C7C110090.
At offset 090356, replace 0F8558020000 with E8C57B110090.
At offset 065405, replace E8E6470000 with E8162B1400.
At offset 00E818, replace 0F8C1B010000 with E80397190090.
At offset 00EFD4, replace A00A548000 with E8478F1900.

# To call the hook that override order of battle strings
At offset 178064, replace E8171A0200 with E8B7FE0200.
At offset 178159, replace E822190200 with E8C2FD0200.

# To call the hook that fixes xwa.tab
At offset 16C599, replace 8D144033DBC1E209 with 8BD09033DBC1E20A.


*** Usage ***

Suppose that the mission is "[MissionDir]\[Mission].tie".

To override the mission data, create a file named "[MissionDir]\[Mission].txt" or create a section named "[mission_tie]" in "[MissionDir]\[Mission].ini".
The format is a line per replacement.

To replace the flight group markings, the format is: "fg", fg number, "markings", value.
To replace the flight group wingman markings, the format is: "fg", fg number, "index", wingman index, "markings", value.
To replace the flight group iff, the format is: "fg", fg number, "iff", value.
To replace the flight group pilot voice, the format is: "fg", fg number, "pilotvoice", value.

To replace the craft name in shiplist.txt, the format is: "craft", craft index, "name", value.
To replace the craft spec name in strings.txt, the format is: "craft", craft index, "specname", value.
To replace the craft plural name in strings.txt, the format is: "craft", craft index, "pluralname", value.
To replace the craft short name in strings.txt, the format is: "craft", craft index, "shortname", value.

To define the red alert in hangar, set "IsRedAlertEnabled = 1". The default setting enables IsRedAlertEnabled for mission id 20.
To skip hyperspaced messages, set "SkipHyperspacedMessages = 1". The default setting enables SkipHyperspacedMessages for mission id 49.
To skip objects messages based on iff, set "SkipObjectsMessagesIff = value". When value is -1 no message is skip. When value is 255 all messages are skiped. In other cases the value is the IFF. The default value is 1 for mission id 49, 50, 51, 52.
To force the player in turret, set "ForcePlayerInTurret = 1". The default setting enables ForcePlayerInTurret for mission id 1.
To set the force turret time, set the ForcePlayerInTurretHours, ForcePlayerInTurretMinutes and ForcePlayerInTurretSeconds settings. The default values are 0, 0, 8.
To disable player laser shoot, set "DisablePlayerLaserShoot = 1". The default value is 0.
To disable player warhead shoot, set "DisablePlayerWarheadShoot = 1". The default value is 0.
To disable warhead collision damages, set "IsWarheadCollisionDamagesEnabled = 0". The default value is 1.
To define if the ships can shoot throught the shield on hard difficulty, set "CanShootThroughtShieldOnHardDifficulty = 0". The default value is 0. It is 1 for the mission 50 (Death Star Phase 2).

See "mission.txt".


# Stats Profiles

Suppose that the craft is "FlightModels\[Model].opt".
To create a stats profile named "Profile1", create a file named "FlightModels\[Model]StatsProfile_Profile1.txt" or create a section named "StatsProfile_Profile1" in "FlightModels\[Model].ini".
To define a profile for the player, append _Player after the profile name. For example: StatsProfile_Profile1_Player.
To define a profile based on the difficulty, append _Easy, _Medium, or _Hard after the profile name. For example: StatsProfile_Profile1_Easy.
To define a profile for the player based on the difficulty, append the player key and the difficulty key. For example: StatsProfile_Profile1_Player_Easy.
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
