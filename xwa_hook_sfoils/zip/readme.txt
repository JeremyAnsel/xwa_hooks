xwa_hook_sfoils

This hook enables S-Foils for any craft.

For more informations, please see the following thread at xwaupgrade.com:
http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11693


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_sfoils.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# Disable S-Foil model filter
At offset 0FB86E, replace 7419 with EB19.

# To call the hook that defines S-Foils
At offset 000AF3, replace 4883F8310F872001000033C98A8840184000 with 50E827741A0083C4045F5E5D5BC390909090.
At offset 000D14, replace 4883F8310F87B101000033D28A90C42C4000 with 50E816721A0083C4048BF8E94E0100009090.


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".
To create s-foils, create a file named "FlightModels\[Model]SFoils.txt".
If the file does not exist, default values are used (S-Foils for XWing, BWing, and Shuttle).
The format is a line per mesh:
mesh index, angle, closing speed, opening speed.
See XWingSFoils.txt, BWingSFoils.txt, ShuttleSFoils.txt.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
