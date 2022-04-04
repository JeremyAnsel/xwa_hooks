xwa_hook_weapon_color

This hook permits to define weapon color for any craft.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_weapon_color.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that defines weapon color
At offset 091449, replace A1C4337B00 with E8E26A1100.
At offset 0E19E1, replace A1C4337B00 with E84A650C00.
At offset 0E1692, replace 8A8848655B0080F9FF741B with 5756E887680C005E5FEB1B.
At offset 0E7ACF, replace 663D2802750E668B4608 with 5056E85A040C00EB1690.
At offset 00A3B5, replace E886E40D00 with E866DB1900.
;At offset 00A652, replace E8E9E10D00 with E8C9D81900.
At offset 069F29, replace 6681BDB00100001801 with E8F2DF1300EB539090.
At offset 069F7B, replace C7442418926900FF with E8A0DF1300909090.
At offset 0F1CC2, replace 3D190100007F403D18010000 with 56E858620B005EE992020000.


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".

To enable weapon switch based on IFF, set "WeaponSwitchBasedOnIff = 1" in the "hook_weapon_color.cfg" file or the "hook_weapon_color" section of "hooks.ini".
To disable it, set "WeaponSwitchBasedOnIff = 0".
The default value is "0".
You can override this value with a setting named "WeaponSwitchBasedOnIff" in the file named "FlightModels\[Model]WeaponColor.txt" or the section named "[WeaponColor]" in "FlightModels\[Model].ini".
Set "0" to disable or "1" to enable the feature or "-1" to use the global setting.
The default value is "-1".

To define the color, create a file named "FlightModels\[Model]WeaponColor.txt" or create a section named "[WeaponColor]" in "FlightModels\[Model].ini".
The format is:
WeaponColor = color marking value
WeaponColor_fgFG_ = color marking value
WeaponColorXXX = color marking value
WeaponColor_fgFG_XXX = color marking value
WeaponImpactColor_3100 = ARGB color
WeaponImpactColor_3200 = ARGB color
WeaponImpactColor_3300 = ARGB color
WeaponImpactColor_3400 = ARGB color
WeaponImpactColor_3500 = ARGB color
WeaponImpactColor_3100_fgFG_ = ARGB color
WeaponImpactColor_3200_fgFG_ = ARGB color
WeaponImpactColor_3300_fgFG_ = ARGB color
WeaponImpactColor_3400_fgFG_ = ARGB color
WeaponImpactColor_3500_fgFG_ = ARGB color
WeaponImpactColorXXX = ARGB color
WeaponImpactColor_fgFG_XXX = ARGB color
EnergyBarColorLaserHigh = ARGB color
EnergyBarColorLaserLow = ARGB color
EnergyBarColorIonHigh = ARGB color
EnergyBarColorIonLow = ARGB color
EnergyBarColorLaserHigh_fgFG_ = ARGB color
EnergyBarColorLaserLow_fgFG_ = ARGB color
EnergyBarColorIonHigh_fgFG_ = ARGB color
EnergyBarColorIonLow_fgFG_ = ARGB color
WeaponLightColorXXX = RGB color
WeaponLightColor_fgFG_XXX = RGB color

Suppose that the mission is "[MissionDir]\[Mission].tie".
To define settings on a per mission basis, create a file named "[MissionDir]\[Mission]_WeaponColor.txt" or create a section named "[WeaponColor]" in "[MissionDir]\[Mission].ini".
All the settings defined in a [Model] file can be replaced in a [Mission] file. The key of the setting in the mission file must be prefixed with "[Model]_".
For example, "WeaponColor" in "[Model].ini" become "[Model]_WeaponColor" in "[Mission].ini".

The format of the color is ARGB: FF000000 is black, FFFF0000 is red, FF00FF00 is green, FF0000FF is blue.

WeaponColor defines the default value used for all weapons.
XXX in WeaponColorXXX goes from 280 to 307. When present, this overrides the default value. You can define a different value for all weapons.
The default value is 0.

XXX in WeaponImpactColorXXX goes from 280 to 307. When present, this overrides the default impact color.  If it is set to 0, the default value will be used. You can define a different impact color for all weapons.
The default impact colors are defined in the "hook_weapon_color.cfg" file or the "hook_weapon_color" section of "hooks.ini".

EnergyBarColorLaserHigh and EnergyBarColorLaserLow define the color of the laser energy bar.
EnergyBarColorIonHigh and EnergyBarColorIonLow define the color of the ion energy bar.

XXX in WeaponLightColorXXX goes from 280 to 307. When present, this overrides the default lighting effect color.  If it is set to 0, the default value will be used. You can define a different light color for all weapons.
The default light colors are defined in the "hook_weapon_color.cfg" file or the "hook_weapon_color" section of "hooks.ini".

FG in _fgFG_ defines the flight group markings.

See XWingWeaponColor.txt.

You also need to add a flightgroup color/markings to the weapon OPTs.

"ANIMATIONS.DAT" file in the "RESDATA" directory is a sample dat file with impact colors.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
