xwa_hook_weapon_rate

This hook permits to define weapon decharge and recharge rates for any craft.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_weapon_rate.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that defines weapon decharge and recharge rates
At offset 090A51, replace A1C4337B008A54080484D2 with E8CA741100E98000000090.
At offset 08FCCE, replace 6683FA0574096683FA078D1436 with 52E84C8211005A0FAFD6EB0690.
At offset 090C66, replace A1FC0D910083FBFF with E8B5721100EB2990.
At offset 0FB100, replace 66F7D81BC0 with E81BCE0A00.
At offset 0FC1F9, replace 66F7D81BC0 with E822BD0A00.
At offset 0FC6FF, replace 751283E21F with E81CB80A00.
At offset 0FC20F, replace 7F038B45FC with E80CBD0A00.
At offset 0FC2BB, replace 66837DF464734B with E860BC0A009090.
At offset 0FC6C0, replace 7E07C745F820030000 with E85BB80A0090909090.
At offset 0FC73B, replace 66837DF4640F83ED000000 with E8E0B70A00909090909090.
At offset 0A6F23, replace C744243810000000 with E8F80F1000909090.
At offset 00DCB9, replace A015548000 with E872A21900.
At offset 00B725, replace 897C2410663D00807202F7D8 with E8F6C71900897C241090EB0F.

# To call the hook that sets the weapons profiles
At offset 0E1B45, replace 8BB08D00000083C63B89B08D000000 with E8D6630C0090909090909090909090.
At offset 0E0DC3, replace 668B9C51D0B55B00 with E858710C00909090.
At offset 0A813D, replace 8B8C91C4B55B00 with E8DEFD0F009090.
At offset 0A83B1, replace 8B8C90C4B55B00 with E86AFB0F009090.
At offset 0A85AD, replace 8B8488C4B55B00 with E86EF90F009090.
At offset 0A8F10, replace 8B8490C4B55B00 with E80BF00F009090.
At offset 0E1514, replace 8BB48AC4B55B00 with E8076A0C009090.
At offset 0E181A, replace 837C081FFF747C with E801670C009090.

At offset 0E4186, replace E825C1FAFF with E8953D0C00.
At offset 090EAA, replace 8B15C4337B00 with E87170110090.
At offset 00C947, replace 668B8160655B00 with E8D4B519009090.
At offset 00C96B, replace 668B8198655B00 with E8B0B519009090.
At offset 0916D6, replace 668B1C4530635B00 with E845681100909090.
At offset 0916DE, replace 668B044568635B00 with E83D681100909090.
At offset 092529, replace 668B3C4530635B00 with E8F2591100909090.
At offset 092531, replace 668B044568635B00 with E8EA591100909090.
At offset 092A11, replace 668B347530635B00 with E80A551100909090.
At offset 092A23, replace 668B045568635B00 with E8F8541100909090.
At offset 09684F, replace 668B144DF8635B00 with E8CC161100909090.
At offset 0D8F11, replace 668B1C7DF8635B00 with E80AF00C00909090.
At offset 0D8F49, replace 668B0C55F8635B00 with E8D2EF0C00909090.
At offset 00C94E, replace 668B9928655B00 with E8CDB519009090.
At offset 091615, replace 668B1C45F8625B00 with E806691100909090.
At offset 091630, replace 668B1C45F8625B00 with E8EB681100909090.
At offset 0922E0, replace 668B1542655B00 with E83B5C11009090.
At offset 0924EE, replace 668B1445F8625B00 with E82D5A1100909090.
At offset 092995, replace 668B1475F8625B00 with E886551100909090.
At offset 0929BE, replace 66031475F8625B00 with E85D551100909090.
At offset 093649, replace 66A15C655B00 with E8D248110090.
At offset 0936CA, replace 668B0D5C655B00 with E8514811009090.
At offset 095CFE, replace 668B0D42655B00 with E81D2211009090.
At offset 0B5371, replace 668B344DF8625B00 with E8AA2B0F00909090.
At offset 0E16F2, replace 668B044DF8625B00 with E829680C00909090.
At offset 0E1A73, replace 668B047D28655B00 with E8A8640C00909090.
At offset 0E1A9D, replace 668B047D28655B00 with E87E640C00909090.
At offset 0E4170, replace 668B047DF8625B00 with E8AB3D0C00909090.
At offset 00E90C, replace 8B048558605B00 with E80F9619009090.
At offset 091674, replace 8B048558605B00 with E8A76811009090.
At offset 09169D, replace 8B0DC4337B00 with E87E68110090.
At offset 0916B5, replace 8B048558605B00 with E8666811009090.
At offset 09251C, replace 8B148558605B00 with E8FF5911009090.
At offset 0929DC, replace 8B04B558605B00 with E83F5511009090.
At offset 0929F9, replace 8B0CB558605B00 with E8225511009090.
At offset 093660, replace A120655B00 with E8BB481100.
At offset 0A6D7E, replace 8B048558605B00 with E89D1110009090.
At offset 0A7163, replace 8B048558605B00 with E8B80D10009090.
At offset 0A7241, replace 8B048558605B00 with E8DA0C10009090.
At offset 0E1A8C, replace 8B04BDB8645B00 with E88F640C009090.
At offset 0E41AA, replace 8B14BD58605B00 with E8713D0C009090.
At offset 119031, replace 8B150C655B00 with E8EAEE080090.

# To call the hook that filters weapon racks
At offset 01B241, replace 668B1445C0125B00 with 8BD0909090909090.
At offset 0909F1, replace E8BA080000 with E83A751100.
At offset 090FFE, replace E8AD020000 with E82D6F1100.
At offset 0E1044, replace E88763FAFF with E8E76E0C00.
At offset 0312D2, replace E8A9570500 with E8596C1700.
At offset 0323FD, replace 81FF000100007D40 with E81E5B1700909090.

# To call the hook that checks mesh index on fire
At offset 0912C2, replace BBFFFF0000 with E8596C1100.

# To call the hook that sets the warheads profiles
At offset 01B250, replace E83B8E0700 with E8DBCC1800.
At offset 05BFF8, replace E893800300 with E833BF1400.
At offset 05FCFF, replace E88C430300 with E82C821400.
At offset 0B0821, replace E86A38FEFF with E80A770F00.

# To call the hook that sets the energy profiles
At offset 02BAA9, replace 6683F90775076800100000 with E882C41700509090909090.

# To call the hook that sets the weapon linking profiles
At offset 0A6BC1, replace 8B15CCA17C00 with E85A13100090.
At offset 09074F, replace 8A840DB6010000 with E8DC7711009090.
At offset 0FBE72, replace 8A45F033C9 with E8A9C00A00.
At offset 0A6CF9, replace 0F8234010000 with E82212100090.
At offset 0A6E03, replace 8B1578F38B00 with E81811100090.
At offset 0FBDBA, replace 660FB68403B6010000 with E861C10A0090909090.

# To call the hook that sets the warhead lock distance
At offset 08F213, replace BBAD8D0100722C with E8088D11009090.

# To call the hook that sets the projectile turret threat
At offset 0A80C9, replace E8D29AFFFF with E852FE0F00.

# To call the hook that draws the energy bar
At offset 06A08A, replace BE01000000 with E891DE1300.
At offset 0692A3, replace A1C81C8C00 with E878EC1300.

# To call the hook that sets the weapons count
At offset 0912D6, replace 8D0440C745D8060000008D34818A82F0645B0084C08D7E0C with E8456C1100C745D8060000008A82F0645B0084C090909090.
At offset 014C5A, replace 8D047FC1E002 with E8C132190090.
At offset 01B25C, replace B001 with 9090.
At offset 01B288, replace B001 with 9090.
At offset 01B2A6, replace B009 with 9090.
At offset 05C004, replace B001 with 9090.
At offset 05C034, replace B001 with 9090.
At offset 05C042, replace B009 with 9090.
At offset 05FD15, replace C744243801000000 with 9090909090909090.
At offset 05FD7B, replace C744243801000000 with 9090909090909090.
At offset 05FD95, replace C744243809000000 with 9090909090909090.
At offset 0B0835, replace 896C2414 with 90909090.
At offset 0B0877, replace 896C2414 with 90909090.

# To call the hook that filters ship type for weapons
At offset 00E825, replace 3C0474083C050F8508010000 with 909090909090909090909090.

# To call the hook that reloads warheads in hangar
At offset 05FDAD, replace 663BCA7369 with E86E811400.
At offset 06088B, replace 89156C6D6800 with E89076140090.


*** Usage ***

# model decharge and recharge rates.

Suppose that the craft is "FlightModels\[Model].opt".

To define the decharge and recharge rates, create a file named "FlightModels\[Model]WeaponRate.txt" or create a section named "[WeaponRate]" in "FlightModels\[Model].ini".
You can override the settings in "FlightModels\WeaponRate.txt" or in the "[WeaponRate]" section in "FlightModels\default.ini".

The format is:
DechargeRate = decharge value
RechargeRate = recharge value
CooldownTimeFactor = cooldown time value
EnergyTransferRate = energy transfer value
EnergyTransferRatePenalty = energy transfer penalty value
EnergyTransferWeaponLimit = energy transfer limit for weapons
EnergyTransferShieldLimit = energy transfer limit for shields
MaxTorpedoCountPerPass = value
MaxTorpedoCountPerTarget = value
IsImpactSpinningEnabled = enabled value
ImpactSpinningSpeedFactorPercent = percent value
ImpactSpinningAngleFactorPercent = percent value

If the file does not exist, default values are used.

Suppose that the mission is "[MissionDir]\[Mission].tie".
To define a DechargeRate and RechargeRate for a flightgroup, create a file named "[MissionDir]\[Mission]_WeaponRates.txt" or create a section named "[WeaponRates]" in "[MissionDir]\[Mission].ini".
The format is
DechargeRate_fg_# = decharge value
RechargeRate_fg_# = recharge value
EnergyTransferRate_fg_# = energy transfer value
EnergyTransferRatePenalty_fg_# = energy transfer penalty value
EnergyTransferWeaponLimit_fg_# = energy transfer limit for weapons
EnergyTransferShieldLimit_fg_# = energy transfer limit for shields
MaxTorpedoCountPerPass_fg_# = value
MaxTorpedoCountPerTarget_fg_# = value
IsImpactSpinningEnabled_fg_# = enabled value
ImpactSpinningSpeedFactorPercent_fg_# = percent value
ImpactSpinningAngleFactorPercent_fg_# = percent value

Replace # with the flightgroup index.
Set the value to -1 to use per-craft settings.

By default, the decharge rate is controlled as this:
if the ship category is Starfighter,
  if the craft is TieFighter or TieBomber, the decharge rate is 3,
  else the decharge rate is 4.
else
  if the weapon sequence is < 4, the decharge rate is 3,
  else there is no decharge.

By default, the recharge rate is controlled as this:
if the craft is TieFighter or TieBomber, the recharge rate is 3 * PresetLaser,
else the recharge rate is 2 * PresetLaser.

The default value for CooldownTimeFactor is 47.

By default, the energy transfer rate is controlled as this:
if the craft is MissileBoat, the recharge rate is 32,
else the energy transfer rate is 4.

By default, the energy transfer rate penalty is 0.

By default, the energy transfer limit for weapons is 100.
By default, the energy transfer limit for shields is 800.

By default, the value for MaxTorpedoCountPerPass is 6.
If the value is -1, then the global value is used.
By default, the value for MaxTorpedoCountPerTarget is 16.
If the value is -1, then the global value is used.

By default, the "is impact spinning enabled" setting is the global value (1).
The ImpactSpinningSpeedFactorPercent is a percent applied to the craft speed on computing the spinning. The default value is 100.
The ImpactSpinningAngleFactorPercent is a percent applied to the angle between the craft and the weapon on computing the spinning. The default value is 100.

See TieFighterWeaponRate.txt and TieBomberWeaponRate.txt.

# weapons profiles

Suppose that the craft is "FlightModels\[Model].opt".

To define the weapons stats, create a file named "FlightModels\[Model]WeaponStats.txt" or create a section named "[WeaponStats]" in "FlightModels\[Model].ini".
You can override the settings in "FlightModels\WeaponStats.txt" or in the "[WeaponStats]" section in "FlightModels\default.ini".

The format of the default profile is:
WeaponXXX_DurationIntegerPart = value
WeaponXXX_DurationDecimalPart = value
WeaponXXX_Score = value
WeaponXXX_Speed = value
WeaponXXX_Power = value
WeaponXXX_PowerSpeedPercent = value
WeaponXXX_FireRate = value
WeaponXXX_DurationOffset = value
WeaponXXX_FireRatio = value
WeaponXXX_Range = value
WeaponXXX_IsPrecise = value
WeaponXXX_DegreeOfSpreadMask = value
WeaponXXX_DechargeRate = value
WeaponXXX_RechargeRate = value
WeaponXXX_EnergyLowHighSeparation = value
WeaponXXX_Side = value
WeaponXXX_SideModel = value

The format of a profile is:
ProfileName_WeaponXXX_DurationIntegerPart = value
ProfileName_WeaponXXX_DurationDecimalPart = value
ProfileName_WeaponXXX_Score = value
ProfileName_WeaponXXX_Speed = value
ProfileName_WeaponXXX_Power = value
ProfileName_WeaponXXX_PowerSpeedPercent = value
ProfileName_WeaponXXX_FireRate = value
ProfileName_WeaponXXX_DurationOffset = value
ProfileName_WeaponXXX_FireRatio = value
ProfileName_WeaponXXX_Range = value
ProfileName_WeaponXXX_IsPrecise = value
ProfileName_WeaponXXX_DegreeOfSpreadMask = value
ProfileName_WeaponXXX_DechargeRate = value
ProfileName_WeaponXXX_RechargeRate = value
ProfileName_WeaponXXX_EnergyLowHighSeparation = value
ProfileName_WeaponXXX_Side = value
ProfileName_WeaponXXX_SideModel = value

ProfileName is the name of the profile.
XXX is the weapon index, between 280 and 307.
If the value is -1 then default values from the weapons arrays in the exe are used. You can view them with XwaWorskpaceViewer.
For the PowerSpeedPercent setting, the value is between 0 and 100. The default value is 100. This setting is the percent factor applied to the weapon speed used to compute the weapon power.
For the FireRate setting, for lasers the default value is 47 , and for warheads the default value is 118 for TieWarheads and 472 for other crafts.
For the DurationOffset setting, the default value is 59.
For the FireRatio setting, the default value is -1. This means that the value is taken from the exe craft struct. You can view it with XwaWorskpaceViewer.
For the Range setting, the default value is -1. This means that the value is taken from the exe craft struct. You can view it with XwaWorskpaceViewer.
For the IsPrecise setting, the default value is 1. 1 means that the weapon is precise. 0 means that there is a degree a spread like player turret.
For the DegreeOfSpreadMask setting, the default value is 255 (0xff). This setting is a binary mask which control the degree of spread.
For the DechargeRate setting, the default value is -1. This means that value is taken from the model DechargeRate and DechargeRate_fg_# values.
For the RechargeRate setting, the default value is -1. This means that value is taken from the model RechargeRate and RechargeRate_fg_# values.
For the EnergyLowHighSeparation setting, the default value is -1. This means that the value is 64. The value is between 0 and 127. This setting define where there is the separation between low and high power weapon. The low power weapon is used to read this setting.
To define a profile for the player, append _Player after WeaponXXX. For example: WeaponXXX_Player_DurationIntegerPart or ProfileName_WeaponXXX_Player_DurationIntegerPart.
To define a profile based on the difficulty, append _Easy, _Medium, or _Hard after WeaponXXX. For examples: WeaponXXX_Easy_DurationIntegerPart or ProfileName_WeaponXXX_Easy_DurationIntegerPart.
To define a profile for the player based on the difficulty, append the player key and the difficulty key. For example: WeaponXXX_Player_Easy_DurationIntegerPart or ProfileName_WeaponXXX_Player_Easy_DurationIntegerPart.

Suppose that the mission is "[MissionDir]\[Mission].tie".
To define a weapons profile for a flightgroup, create a file named "[MissionDir]\[Mission]_WeaponProfiles.txt" or create a section named "[WeaponProfiles]" in "[MissionDir]\[Mission].ini".
The format is
WeaponProfile_fg_# = ProfileName
Replace # with the flightgroup index.
The default ProfileName is "Default".

# warhead profiles

Suppose that the craft is "FlightModels\[Model].opt".

To define the warheads stats, create a file named "FlightModels\[Model]WarheadStats.txt" or create a section named "[WarheadStats]" in "FlightModels\[Model].ini".
You can override the settings in "FlightModels\WarheadStats.txt" or in the "[WarheadStats]" section in "FlightModels\default.ini".

The format of the default profile is:
WarheadXXX_CapacityPercent = value

The format of a profile is:
ProfileName_WarheadXXX_CapacityPercent = value

ProfileName is the name of the profile.
XXX is the warhead index, between 0 and 10.
WarheadType_None = 0,
WarheadType_SpaceBombs = 1,
WarheadType_HeavyRockets = 2,
WarheadType_Missiles = 3,
WarheadType_ProtonTorpedos = 4,
WarheadType_AdvancedMissiles = 5,
WarheadType_AdvancedTorpedos = 6,
WarheadType_MagPulse = 7,
WarheadType_IonPulse = 8,
WarheadType_AdvancedMagPulse = 9,
WarheadType_ClusterBombs = 10,
If the value is -1 then default values from the warheads arrays in the exe are used.
For the CapacityPercent setting, the value is a percent. The default value is 100. This setting is the percent factor applied to warheads capacity.

Suppose that the mission is "[MissionDir]\[Mission].tie".
To define a warheads profile for a flightgroup, create a file named "[MissionDir]\[Mission]_WarheadProfiles.txt" or create a section named "[WarheadProfiles]" in "[MissionDir]\[Mission].ini".
The format is
WarheadProfile_fg_# = ProfileName
Replace # with the flightgroup index.
The default ProfileName is "Default".

# energy profiles

Suppose that the craft is "FlightModels\[Model].opt".

To define the energy stats, create a file named "FlightModels\[Model]EnergyStats.txt" or create a section named "[EnergyStats]" in "FlightModels\[Model].ini".
You can override the settings in "FlightModels\EnergyStats.txt" or in the "[EnergyStats]" section in "FlightModels\default.ini".

The format of the default profile is:
SpeedFactor = value

The format of a profile is:
ProfileName_SpeedFactor = value

ProfileName is the name of the profile.
If the value is -1 then default values are used.
For the SpeedFactor setting, the value is a percent. This setting is the factor applied to the speed. For the TieBomber, the default value is 6. For the TieFighter, the default value is 18. For other crafts, the default value is 12.

Suppose that the mission is "[MissionDir]\[Mission].tie".
To define a energy profile for a flightgroup, create a file named "[MissionDir]\[Mission]_EnergyProfiles.txt" or create a section named "[EnergyProfiles]" in "[MissionDir]\[Mission].ini".
The format is
EnergyProfile_fg_# = ProfileName
Replace # with the flightgroup index.
The default ProfileName is "Default".

# linking profiles

Suppose that the craft is "FlightModels\[Model].opt".

To define the linking stats, create a file named "FlightModels\[Model]LinkingStats.txt" or create a section named "[LinkingStats]" in "FlightModels\[Model].ini".
You can override the settings in "FlightModels\LinkingStats.txt" or in the "[LinkingStats]" section in "FlightModels\default.ini".

The format of the default profile is:
LaserLink = value
PreventAILasersIonWhenDisabling = value
PreventAIIonWhenNotDisabling = value
PreventPlayerLinkedLasersIon = value

The format of a profile is:
ProfileName_LaserLink = value
ProfileName_PreventAILasersIonWhenDisabling = value
ProfileName_PreventAIIonWhenNotDisabling = value
ProfileName_PreventPlayerLinkedLasersIon = value

ProfileName is the name of the profile.
If the value is -1 then default values are used.
For the LaserLink setting, the value is between -1 and 4.
For the PreventAILasersIonWhenDisabling setting, the value is between -1 and 1. -1 means global value. 0 means disabled. 1 means enabled.
For the PreventAIIonWhenNotDisabling setting, the value is between -1 and 1. -1 means global value. 0 means disabled. 1 means enabled.
For the PreventPlayerLinkedLasersIon setting, the value is between -1 and 1. -1 means global value. 0 means disabled. 1 means enabled.

Suppose that the mission is "[MissionDir]\[Mission].tie".
To define a linking profile for a flightgroup, create a file named "[MissionDir]\[Mission]_LinkingProfiles.txt" or create a section named "[LinkingProfiles]" in "[MissionDir]\[Mission].ini".
The format is
LinkingProfile_fg_# = ProfileName
Replace # with the flightgroup index.
The default ProfileName is "Default".

# mesh filter

To enable or disable the fire mesh filter set the "EnableFireMeshFilter" setting. It is enabled by default.

# warheads reload in hangar

Suppose that the mission is "[MissionDir]\[Mission].tie".
To limit the warheads reloading for a flightgroup, create a file named "[MissionDir]\[Mission]_WarheadTypeCount.txt" or create a section named "[WarheadTypeCount]" in "[MissionDir]\[Mission].ini".
The format is
WarheadTypeCount_fg_#_## = value
The value is the warheads count. To not limit the reloading, set the value to -1. For example if the value is 10 then the ship can reload up to 10 warheads.
Replace # with the flightgroup index.
Replace ## with the warhead type.
The warhead type is one of these values:
- SpaceBombs
- HeavyRockets
- Missiles
- ProtonTorpedos
- AdvancedMissiles
- AdvancedTorpedos
- MagPulse
- IonPulse
- AdvancedMagPulse
- ClusterBombs


*** Credits ***

- Jérémy Ansel (JeremyaFr)
