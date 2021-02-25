xwa_hook_sfoils

This hook enables S-Foils for any craft.

For more informations, please see the following thread at xwaupgrade.com:
http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11693


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_sfoils.dll and hook_sfoils.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# S-Foils and Landing Gears keys
At offset 100E0D, replace 26 with 10.

# Disable S-Foil model filter
At offset 0FB86A, replace 6683F90174196683F904 with 50E8B0C60A005985C990.
At offset 002EEC, replace 6683F901740A6683F904 with 50E82E501A005985C990.
At offset 058917, replace 668B443102663D01007406663D0400 with 8D0431909050E8FEF514005885C090.
At offset 1081DB, replace 668B440102663D01007405663BC3 with 03C190909050E83AFD09005885C0.

# To call the hook that defines S-Foils
At offset 000AF3, replace 4883F8310F872001000033C98A8840184000 with 50E827741A0083C4045F5E5D5BC390909090.
At offset 000D14, replace 4883F8310F87B101000033D28A90C42C4000 with 50E816721A0083C4048BF8E94E0100009090.
At offset 000CA7, replace 66C70536168C000E00 with 66C70536168C000200.

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

# To call the hook that set S-Foils for hyperspace
At offset 0AF214, replace E807B0FEFF with E8078D0F00.
At offset 0AF22A, replace E81152FFFF with E8F18C0F00.
At offset 0AD7FC, replace E85F52FFFF with E81FA70F00.
At offset 0B9C8D, replace E85EBCFFFF with E88EE20E00.

# Init S-Foils and Landing Gears
At offset 01BC59, replace E802C10600 with E8D2C21800.

# To call the hook that set enter hangar state
At offset 008A8E, replace 8A8732958B00 with E89DF4190090.
At offset 008BF3, replace 8A8732958B00 with E838F3190090.

# To call the hook that set enter hyperspace state
At offset 008D15, replace 8A8732958B00 with E816F2190090.

# To call the hook that shows the no fire message
At offset 09070D, replace 8A4127668B7104 with 56E81D7811005E.

# To call the hook that set the park state
At offset 0BD934, replace 8B0DB0A17C00894147 with 50E8E6A50E00589090.
At offset 0B3580, replace A0F7538000 with E8AB490F00.
At offset 0B428F, replace FEC088415D with E88C3C0F00.


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".

To create s-foils, create a file named "FlightModels\[Model]SFoils.txt" or create a section named "[SFoils]" in "FlightModels\[Model].ini".
If the file does not exist, default values are used (S-Foils for XWing, BWing, and Shuttle).
The format is a line per mesh:
mesh index, angle, closing speed, opening speed.

To enable or disable the S-Foils closing/opening in before/after hyperspace, create a setting named "CloseSFoilsInHyperspace". To enable, set "1". To disable, set "0". The default value is "0".
To slow down the animation, create a setting named "SFoilsAnimationSpeed". For instance, if you set "2", the animation will be slowed down by a factor of 2. The default value is "1" (normal speed).
To allow fire when s-foils are closed, set the AllowFireWhenSFoilsAreClosed setting. When set to 0, fire is not allowed. When set to 1, fire is allowed. The default value is 0.
To close the S-Foils when the craft is parked, set the ParkOrderSFoilsClosed setting. To enable, set "1". To disable, set "0". The default value is "0".

See XWingSFoils.txt, BWingSFoils.txt, ShuttleSFoils.txt.

To create landing gears, create a file named "FlightModels\[Model]SFoilsLandingGears.txt" or create a section named "[SFoilsLandingGears]" in "FlightModels\[Model].ini".
The format is a line per mesh:
mesh index, angle, opening speed, closing speed.

Landing gears are deployed when a craft in is the hangar. When the s-foils are closed in space, the landing gears are not deployed.

To open or close s-foils, use the V key. To deploy or retract landing gears, use the Ctrl+L key.

Suppose that the mission is "[MissionDir]\[Mission].tie".

To override the mission data, create a file named "[MissionDir]\[Mission].txt" or create a section named "[mission_tie]" in "[MissionDir]\[Mission].ini".
The format is a line per replacement.

To replace the flight group s-foils state, the format is: "fg", fg number, "close_SFoils", 1.
To replace the flight group landing gears state, the format is: "fg", fg number, "open_LandingGears", 1.

To require s-foils being closed before enter hangar, set the CloseSFoilsAndOpenLangingGearsBeforeEnterHangar setting.
When set to 0, normal behavior
When set to 1, s-foils must be closed and landing gears opened before enter hangar

To require landing gears being closed before enter hyperspace, set the CloseLangingGearsBeforeEnterHyperspace setting.
When set to 0, normal behavior
When set to 1, landing gears must be closed before enter hyperspace

To manualy control the s-foils and landing gears, set the AutoCloseSFoils setting.
When set to 0, s-foils and landing gears are manualy closed
When set to 1, s-foils and landing gears are automaticaly closed
The default value is 1.

See the "hook_sfoils.cfg" file or the "hook_sfoils" section of "hooks.ini".


*** Credits ***

- Jérémy Ansel (JeremyaFr)
