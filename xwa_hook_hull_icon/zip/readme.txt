xwa_hook_hull_icon

This hook enables setting hull icon and map icon for any craft.

For more informations, please see the following thread at xwaupgrade.com:
http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11761


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_hull_icon.dll, HullIconList.txt and MapIconList.txt next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that defines hull icons
At offset 067089, replace 663D4700771925FFFF0000668B0C4508965A00 with 25FFFF000050E89C0E140083C4048BC8909090.
At offset 06A1C0, replace 663D4700770E25FFFF0000668B044508965A00 with 25FFFF000050E865DD130083C4049090909090.
At offset 06A470, replace 663D4700771925FFFF0000668B0C4508965A00 with 25FFFF000050E8B5DA130083C4048BC8909090.
At offset 06A841, replace 663D4700771925FFFF0000668B0C4508965A00 with 25FFFF000050E8E4D6130083C4048BC8909090.

# To call the hook that defines map icons
At offset 09FA2B, replace 6681FEDE00771A with 90909090909090.
At offset 09FA38, replace 668B0C75D89A5A00 with 56E8E28410005990.

# To call the hook that renders in-flight icons
At offset 09FAB7, replace E8C4B6F8FF with E864841000.
At offset 06A501, replace E87A0CFCFF with E81ADA1300.
At offset 06A8D2, replace E8A908FCFF with E849D61300.


*** Usage ***

To set the hull icon for a craft, open "HullIconList.txt" and change the corresponding line to the desired value.
If the file does not exist, default values are used.
See HullIconList.txt.

To set the map icon for a craft, open "MapIconList.txt" and change the corresponding line to the desired value.
If the file does not exist, default values are used.
See MapIconList.txt.

Suppose that the mission is "[MissionDir]\[Mission].tie".
To replace the player hull icon for a given mission, create a file named "[MissionDir]\[Mission]_HullIcon.txt" or create a section named "[HullIcon]" in "[MissionDir]\[Mission].ini".
The format is:
PlayerHullIcon = value
If the value is 0, then no icon is replaced.

To replace the craft hull icon for a given mission, create a file named "[MissionDir]\[Mission]_Objects.txt" or create a section named "[Objects]" in "[MissionDir]\[Mission].ini".
The format is a line per object: "FlightModels\ObjectA_HullIcon = value"
If the value is 0, then no icon is replaced.
See "Objects.txt".

To define the scale of the in-flight map and hull icons, set the MapIconScale and HullIconScale settings.
The default value for the MapIconScale setting is 256.
The default value for the HullIconScale setting is 256.

See the "hook_hull_icon.cfg" file or the "hook_hull_icon" section of "hooks.ini".


*** Credits ***

- Jérémy Ansel (JeremyaFr)
