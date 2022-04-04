xwa_hook_jamming

This hook permits to define the jamming behavior for any craft.

See https://forums.xwaupgrade.com/viewtopic.php?f=33&t=13857


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_jamming.dll and hook_jamming.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that defines jamming
At offset 08F78F, replace E8AC2A0500 with E88C871100.


*** Usage ***

To enable or disabled jamming, edit the "hook_jamming.cfg" file.
The format is:
; IsJammingEnabled = 0 or 1
; When set to 0, disable jamming
; WHen set to 1, enable jamming
IsJammingEnabled = 1

Suppose that the craft is "FlightModels\[Model].opt".
To define the jamming behavior, create a file named "FlightModels\[Model]Jamming.txt" or create a section named "[Jamming]" in "FlightModels\[Model].ini".

The format is:
; HasJamming = 0 or 1
HasJamming = 1
; JammingDirection = All or Up or Down
JammingDirection = All
; JammingDistance = integer
; -1 = disabled
JammingDistance = -1
; JammingBehavior = None or HangarMeshCenter or InsideHangarHardpoint or InsideHangarHardpointAndJammingPoints
JammingBehavior = InsideHangarHardpointAndJammingPoints

If the file does not exist, default values are used..
See *Jamming.txt


*** Credits ***

- Jérémy Ansel (JeremyaFr)
