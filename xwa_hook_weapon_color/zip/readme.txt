xwa_hook_weapon_color

This hook permits to define weapon color for any craft.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_weapon_color.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that defines weapon color
At offset 091449, replace A1C4337B00 with E8E26A1100.


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".

To define the color, create a file named "FlightModels\[Model]WeaponColor.txt" or create a section named "[WeaponColor]" in "FlightModels\[Model].ini".
The format is:
WeaponColor = color value
WeaponColorXXX = color value

WeaponColor defines the default value used for all weapons.
XXX in WeaponColorXXX goes from 280 to 307. When present, this overrides the default value. You can define a different value for all weapons.

If the file does not exist, 0 is used.

See XWingWeaponColor.txt.

You also need to add a flightgroup color/markings to the weapon OPTs.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
