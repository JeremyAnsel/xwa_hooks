xwa_hook_shield

This hook permits to define shield recharge rates for any craft.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_shield.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that defines shield recharge rate
At offset 090254, replace 0F8510F8FFFF with 90E910F8FFFF.
At offset 08F72A, replace C744241814000000 with E8F1871100909090.
At offset 08FA6A, replace C744241814000000 with E8B1841100909090.
;At offset 08F034, replace 8B0D78F38B00 with E8E78E110090.
;At offset 08FACF, replace 0FAF542418 with E84C841100.
;At offset 08FA82, replace A1C4337B00 with E899841100.
At offset 08F99B, replace 66396C2418736C with E8808511009090.
At offset 08FB0E, replace 8B0DFC0D9100 with E80D84110090.
At offset 08FB72, replace 8B0DFC0D9100 with E8A983110090.


*** Usage ***

Suppose that the mission is "[MissionDir]\[Mission].tie".
Suppose that the craft is "FlightModels\[Model].opt".

The possible involved files are:
- "[MissionDir]\[Mission]_Shield.txt"
- "[MissionDir]\[Mission].ini", section "[Shield]"
- "FlightModels\[Model]Shield.txt"
- "FlightModels\[Model].ini", section "[Shield]"

To define the recharge rate for a craft, create a file named "FlightModels\[Model]Shield.txt" or create a section named "[Shield]" in "FlightModels\[Model].ini".
The format is:
UseGenerator = 0 to use RechargeRate or 1 to use PerGeneratorRechargeRate
PerGeneratorRechargeRate = per generator recharge value
RechargeRate =  recharge value
RechargeRateDelay =  delay value

If the file does not exist, default values are used.

By default, the recharge rate is controlled as this:
For starfighters the recharge rate is 20.
For Interdictor, Victory Star Destroyer,  Imperial Star Destroyer, and Super Star Destroyer, the recharge rate is 5 multiplied by the number of shield generators still alive.
For other starships, the recharge rate is 5.
For other crafts, the recharge rate is 0.

For player craft, if the rate is 0 then it is set to 20.

The RechargeRateDelay defines the delay from when the shield is 0 to the time where the shield is recharged.

To define the recharge rate for a craft for a given mission, create a file named "[MissionDir]\[Mission]_Shield.txt" or create a section named "[Shield]" in "[MissionDir]\[Mission].ini".
The format is a line per craft:
ModelIndex, UseGenerator, PerGeneratorRechargeRate, RechargeRate

See *Shield.txt.

You can enable or disable the shield recharge for starships with the IsShieldRechargeForStarshipsEnabled setting.
;To set the time between the updates, set the CraftUpdateTime setting. The default value is 123.
To control the recharge rate speed, set the RechargeRatePercent setting. The default value is 100.

See "hook_shield.cfg" or the "hook_shield" section of "Hooks.ini".


*** Credits ***

- Jérémy Ansel (JeremyaFr)
