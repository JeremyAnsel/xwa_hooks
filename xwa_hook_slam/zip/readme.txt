xwa_hook_slam

This hook enables the SLAM system when the K key pressed.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_slam.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that enables SLAM
At offset 100DB3, replace 19 with 00.
At offset 100D5E, replace 26 with 19.
At offset 100CF8, replace AEC14F00 with 74185000.
At offset 100C72, replace 5F5E5B8BE55DC3 with EB10E8A7720A00.


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".

To enable the SLAM feature, create a file named "FlightModels\[Model]Slam.txt" or create a section named "[Slam]" in "FlightModels\[Model].ini".
The format is:
HasSlam = 1
or
HasSlam = 0

If the file does not exist, default values are used. By default, the Missile Boat has a SLAM system.

See MissileBoatSlam.txt.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
