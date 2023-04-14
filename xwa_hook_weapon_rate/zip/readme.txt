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

# To call the hook that filters weapon racks
At offset 0909F1, replace E8BA080000 with E83A751100.
At offset 090FFE, replace E8AD020000 with E82D6F1100.
At offset 0E1044, replace E88763FAFF with E8E76E0C00.
At offset 0312D2, replace E8A9570500 with E8596C1700.
At offset 0323FD, replace 81FF000100007D40 with E81E5B1700909090.

# To call the hook that checks mesh index on fire
At offset 0912C2, replace BBFFFF0000 with E8596C1100.


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".

To define the decharge and recharge rates, create a file named "FlightModels\[Model]WeaponRate.txt" or create a section named "[WeaponRate]" in "FlightModels\[Model].ini".
You can override the settings in "FlightModels\WeaponRate.txt" or in the "[WeaponRate]" section in "FlightModels\default.ini".

The format is:
DechargeRate = decharge value
RechargeRate = recharge value
CooldownTimeFactor = cooldown time value

If the file does not exist, default values are used.

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

See TieFighterWeaponRate.txt and TieBomberWeaponRate.txt.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
