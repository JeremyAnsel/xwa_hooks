xwa_hook_reticle

This hook permits to select the reticle image index for any craft.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_reticle.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that sets the reticle image index
At offset 06B58E, replace E8BD65FCFF with E88DC91300.
At offset 06B5C0, replace E88B65FCFF with E85BC91300.
At offset 06B7E4, replace E86763FCFF with E837C71300.
At offset 06B81B, replace E83063FCFF with E800C71300.
At offset 06B853, replace E8F862FCFF with E8C8C61300.
At offset 06B8D5, replace E87662FCFF with E846C61300.
At offset 06B912, replace E83962FCFF with E809C61300.
At offset 07739B, replace E8B0A7FBFF with E8800B1300.
At offset 077454, replace E8F7A6FBFF with E8C70A1300.
At offset 07750F, replace E83CA6FBFF with E80C0A1300.
At offset 0775D8, replace E873A5FBFF with E843091300.


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".

To select the reticle image index, create a file named "FlightModels\[Model]Reticle.txt" or create a section named "[Reticle]" in "FlightModels\[Model].ini".
If the file does not exist, default values are used.

The format is:
Reticle_XX = image index

XX is in { 5, 6, 7, 8, 9, 10, 15, 16, 17, 18, 19, 20, 21, 22 }.

The reticle images are in "HUD.dat", group ID 12000.
The images are referenced by their position in the group, starting at 1.

See XWingReticle.txt.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
