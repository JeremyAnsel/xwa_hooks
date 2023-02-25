xwa_hook_pilot

This hook enables pilot meshes animation for any craft.

For more informations, please see the following thread at xwaupgrade.com:
http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11695


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_pilot.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that defines pilot meshes animation
At offset 0025FC, replace 483DCB0000000F87C501000033D28A90F8334000FF2495D4334000 with 508B441E238BA8DD00000055E813591A0083C4085F5E5D5BC39090.
At offset 0018EB, replace 742A with EB3D.
At offset 05809E, replace 66A124168C00663BC3 with E8DD8BFAFFEB539090.
At offset 05D5EF, replace 890DFC0D9100 with E82CA9140090.
At offset 05D553, replace A3FC0D9100 with E8C8A91400.


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".

To define wich meshes are animated, create a file named "FlightModels\[Model]Pilot.txt" or create a section named "[Pilot]" in "FlightModels\[Model].ini".

If the file does not exist, default values are used:
- X-Wing (meshes 6 and 9)
- Y-Wing (meshes 8 et 13)
- A-Wing (meshes 4)
- Corellian Transport 2 (meshes 19 et 17)
- Millenium Falcon 2 (meshes 21 and 19)
- Family Transport (meshes 13 and 9)

The format is a line per mesh:
mesh index, angle, speed, behavior
Speed must be a multiple of 2.
Behavior can be 0, 1, 2, 3, 4, 5:
- 0 means constant rotation
- 1 means rotation with random
- 2 means rotation with random and pause
- 3 means the behavior of the mesh 3 of the vanilla HangarDroid
- 4 means the behavior of the mesh 1 of the vanilla HangarDroid2
- 5 means the behavior of the mesh 2 of the vanilla HangarDroid2

To define wich meshes are animated when the cockpit is displayed, create a file named "FlightModels\[Model]PilotCockpit.txt" or create a section named "[PilotCockpit]" in "FlightModels\[Model].ini".
When the "[PilotCockpit]" is empty, the values of the "[Pilot]" section are used.
The format is the same as the "[Pilot]" section.

See *Pilot.txt.
See AWingPilotCockpit.txt.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
