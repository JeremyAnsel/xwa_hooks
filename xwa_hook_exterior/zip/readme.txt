xwa_hook_exterior

This hook displays the HUD in exterior view.

See https://xwaupgrade.com/phpBB3/viewtopic.php?f=9&t=12471


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_exterior.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that renders 3D exterior
At offset 0F1617, replace E8E4DBFFFF with E804690B00.
At offset 1080FD, replace 6689BE69A08B00 with 90909090909090.
At offset 10811F, replace 6689BE44A08B006689BE46A08B00 with 9090909090909090909090909090.
At offset 0AEF78, replace 66898669A08B00 with 90909090909090.
At offset 0AEF9F, replace 66898644A08B0066898646A08B00 with 9090909090909090909090909090.
At offset 0FFFFC, replace 0F85820C0000 with 909090909090.
At offset 0655F0, replace 8B4424048B4C2408 with E82B291400C39090.

# To call the hook that sets the MFD lod distance
At offset 077B5E, replace E8FD1B0700 with E8CD031300.


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".

To set the MFD lod distance, create a file named "FlightModels\[Model]Exterior.txt" or create a section named "[Exterior]" in "FlightModels\[Model].ini".
The format is:
LodDistance = value

If the file does not exist, default values are used.

See VictoryStarDestroyer2Exterior.txt


*** Credits ***

- Jérémy Ansel (JeremyaFr)
