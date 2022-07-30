xwa_hook_hyperspace

This hook permits to define the hyperspace effect.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_hyperspace.dll and hook_hyperspace.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that defines the hyperspace animation
At offset 0AF172, replace 66C78085000000100E with E8A98D0F0090909090.
At offset 0AF189, replace C74062EC000000 with E8928D0F009090.
At offset 0AF1E4, replace C7425E240A0000 with E8378D0F009090.
At offset 0AF27E, replace 66898285000000 with E89D8C0F009090.
At offset 0AF2F5, replace 750F3B484B7E0D with E8268C0F00EB23.
At offset 0AE3B3, replace E8589DFEFF with E8689B0F00.
At offset 02BE9C, replace 66399085000000 with E87FC01700EB0D.


*** Usage ***

You can define settings for the hyperspace animation in "hook_hyperspace.cfg" or in the "hook_hyperspace" section of "hooks.ini".
The format is:
ShortHyperspaceEffect = value
value can be 0, or 1
0 means original behavior
1 means new behavior

Suppose that the mission is "[MissionDir]\[Mission].tie".

The possible involved files are:
- "[MissionDir]\[Mission]_Hyperspace.txt"
- "[MissionDir]\[Mission].ini", section "[Hyperspace]"

To define the hyperspace animation for a given mission, create a file named "[MissionDir]\[Mission]_Hyperspace.txt" or create a section named "[Hyperspace]" in "[MissionDir]\[Mission].ini".
The format is:
ShortHyperspaceEffect = value
value can be -1, 0, or 1
-1 means use the global setting
0 means original behavior
1 means new behavior

For EnterHyperspace sounds, sounds defined in "Wave\Sfx_EnterHyperspace.lst" are used if the lst file exists. The sounds are indexed by the crafts model index.
For ExitHyperspace sounds, sounds defined in "Wave\Sfx_ExitHyperspace.lst" are used if the lst file exists. The sounds are indexed by the crafts model index.

*** Credits ***

- Jérémy Ansel (JeremyaFr)
