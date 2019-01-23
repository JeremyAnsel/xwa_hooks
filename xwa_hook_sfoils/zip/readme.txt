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
At offset 0FB86A, replace 6683F90174196683F904 with 51E8B0C60A005985C990.
At offset 002EEC, replace 6683F901740A6683F904 with 51E82E501A005985C990.
At offset 05891C, replace 663D01007406663D0400 with 50E8FEF514005885C090.
At offset 1081E0, replace 663D01007405663BC3 with 50E83AFD09005885C0.

# To call the hook that defines S-Foils
At offset 000AF3, replace 4883F8310F872001000033C98A8840184000 with 50E827741A0083C4045F5E5D5BC390909090.
At offset 000D14, replace 4883F8310F87B101000033D28A90C42C4000 with 50E816721A0083C4048BF8E94E0100009090.

# To call the hook that defines hangar shuttle S-Foils
At offset 05E2C4, replace 8B15C4337B00668B4C320251E88B9A0200 with 6AFF5757E8539C140083C40CEB6A909090.
At offset 05E5C0, replace 8B0DC4337B00668B44310250E88F970200 with 6A005357E85799140083C40CEB5F909090.
At offset 05E7FF, replace 8B15C4337B0033C9668B4C320251 with 6A015357E81897140083C40CEB6C.
At offset 05EAF4, replace 8B0DC4337B00668B44310250E85B920200 with 6AFF5757E82394140083C40CEB69909090.
At offset 05EDBF, replace 8B15C4337B0033C9668B4C320251E88E8F0200 with 8B4424286A015057E85491140083C40CEB5A90.

# Align the camera with the bridge
At offset 08058C, replace 0F85F5000000 with 909090909090.
At offset 08059B, replace 7519 with 9090.
At offset 080D7C, replace 0F85E0000000 with 909090909090.
At offset 080D8A, replace 7517 with 9090.
At offset 08304B, replace 0F85E9000000 with 909090909090.
At offset 083059, replace 7517 with 9090.


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".
To create s-foils, create a file named "FlightModels\[Model]SFoils.txt" or create a section named "[SFoils]" in "FlightModels\[Model].ini".
If the file does not exist, default values are used (S-Foils for XWing, BWing, and Shuttle).
The format is a line per mesh:
mesh index, angle, closing speed, opening speed.
See XWingSFoils.txt, BWingSFoils.txt, ShuttleSFoils.txt.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
