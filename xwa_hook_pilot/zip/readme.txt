xwa_hook_pilot

This hook enables pilot meshes animation for any craft.

For more informations, please see the following thread at xwaupgrade.com:
http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11695


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_pilot.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that defines pilot meshes animation
At offset 0025FC, replace 483DCB0000000F87C501000033D28A90F8334000FF2495D4334000 with 508B441E238BA8DD00000055E813591A0083C4085F5E5D5BC39090.
At offset 0018EB, replace 742A with EB3D.
At offset 05809E, replace 66A124168C00663BC3 with E8DD8BFAFFEB539090.


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".
To define wich meshes are animated, create a file named "FlightModels\[Model]Pilot.txt".
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
Behavior can be 0, 1, or 2:
- 0 means constant rotation
- 1 means rotation with random
- 2 means rotation with random and pause
See *Pilot.txt.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
