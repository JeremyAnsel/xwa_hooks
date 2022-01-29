xwa_hook_dock

This hook fixes the docking position.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_dock.dll and hook_dock.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that reads the dock position
At offset 03204C, replace 898EEAB65B00 with E8CF5E170090.

# To call the hook that fixes the ai dock position
At offset 0AFD71, replace E83A26FFFF with E8AA810F00.
At offset 0B0015, replace E89623FFFF with E8067F0F00.
At offset 0959DA, replace E8D1C90000 with E841251100.

# To call the hook that modifies the player dock position
At offset 0B5BAF, replace E8FCC7FEFF with E86C230F00.
At offset 0B5B70, replace 742B with 9090.
At offset 0B5B8D, replace 8B9C49EEB65B002BD3 with 50E88D230F005A9090.
At offset 0B5EF7, replace 8996EC000000 with E824200F0090.


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".

To define a dock elevation, create a file named "FlightModels\[Model]Dock.txt" or create a section named "[Dock]" in "FlightModels\[Model].ini".
The format is:
DockElevation = value
If the file does not exist, default values are used. The default value is 0 for the ModelIndex 170 SpaceColony2, and 932 for the other crafts.
See SpaceColony2Dock.txt

To force a dock elevation value, set the ForceDockElevation setting.
Set -1 to use per model values, or set a int value to use the same elevation for all crafts.

See the "hook_dock.cfg" file or the "hook_dock" section of "hooks.ini".


*** Credits ***

- Jérémy Ansel (JeremyaFr)
