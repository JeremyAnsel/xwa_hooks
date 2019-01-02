xwa_hook_interdictor

This hook permits to enable interdictor feature for any craft.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_interdictor.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that enables interdictor feature
At offset 107FA4, replace 6681F9890074076681F98D007533 with 5051E885FF09005985C058743490.


*** Usage ***

Suppose that the craft is "FlightModels\[Model].opt".

To enable the interdictor feature, create a file named "FlightModels\[Model]Interdictor.txt" or create a section named "[Interdictor]" in "FlightModels\[Model].ini".
The format is:
IsInterdictor = 1
or
IsInterdictor = 0

If the file does not exist, default values are used. By default, the ModelIndex 137 Interdictor2 and ModelIndex 141 ModStrikeCruiser are interdictor.

See Interdictor2Interdictor.txt and ModStrikeCruiserInterdictor.txt.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
