xwa_hook_weapon_color

This hook permits to define weapon color for any craft.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_weapon_color.dll next to xwingalliance.exe

Copy the "ANIMATIONS.DAT" file in the "RESDATA" directory.


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that defines weapon color
At offset 091449, replace A1C4337B00 with E8E26A1100.
At offset 0E19E1, replace A1C4337B00 with E84A650C00.
At offset 0E7ACF, replace 663D2802750E668B4608 with 5056E85A040C00EB1690.
At offset 00A3B5, replace E886E40D00 with E866DB1900.
At offset 00A652, replace E8E9E10D00 with E8C9D81900.
At offset 069F29, replace 6681BDB00100001801 with E8F2DF1300EB539090.
At offset 069F7B, replace C7442418926900FF with E8A0DF1300909090.


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".

To define the color, create a file named "FlightModels\[Model]WeaponColor.txt" or create a section named "[WeaponColor]" in "FlightModels\[Model].ini".
The format is:
WeaponColor = color marking value
WeaponColorXXX = color marking value
WeaponImpactColor_3100 = ARGB color
WeaponImpactColor_3200 = ARGB color
WeaponImpactColor_3300 = ARGB color
WeaponImpactColor_3400 = ARGB color
WeaponImpactColor_3500 = ARGB color
WeaponImpactColorXXX = ARGB color
EnergyBarColorLaserHigh = ARGB color
EnergyBarColorLaserLow = ARGB color
EnergyBarColorIonHigh = ARGB color
EnergyBarColorIonLow = ARGB color

The format of the color is ARGB: FF000000 is black, FFFF0000 is red, FF00FF00 is green, FF0000FF is blue.

WeaponColor defines the default value used for all weapons.
XXX in WeaponColorXXX goes from 280 to 307. When present, this overrides the default value. You can define a different value for all weapons.
The default value is 0.

The default impact colors are:
WeaponImpactColor_3100 = FF0000FF
WeaponImpactColor_3200 = FFFF0000
WeaponImpactColor_3300 = FF00FF00
WeaponImpactColor_3400 = FFFF00FF
WeaponImpactColor_3500 = FFFFFF00
XXX in WeaponImpactColorXXX goes from 280 to 307. When present, this overrides the default impact color.  If it is set to 0, the default value will be used. You can define a different impact color for all weapons.

EnergyBarColorLaserHigh and EnergyBarColorLaserLow define the color of the laser energy bar.
EnergyBarColorIonHigh and EnergyBarColorIonLow define the color of the ion energy bar.

See XWingWeaponColor.txt.

You also need to add a flightgroup color/markings to the weapon OPTs.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
