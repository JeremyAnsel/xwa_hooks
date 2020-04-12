xwa_hook_resolution

This hook displays the screen resolution in the video options screen.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_resolution.dll and hook_resolution.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that displays screen resolution
At offset 120747, replace E804B80300 with E8E4770800.
At offset 12089E, replace E8ADB60300 with E88D760800.
At offset 1208D9, replace E872B60300 with E852760800.

# To call the hook that set the resolution
At offset 10F546, replace E825CFFFFF with E8D5890900.
At offset 10A41E, replace E8FD0B0000 with E80DDB0900.


*** Usage ***

To enable the auto resolution setting, set the IsAutoResolutionEnabled setting.
To define the resolution, set the ResolutionWidth and ResolutionHeight settings.

See hook_resolution.cfg


*** Credits ***

- Jérémy Ansel (JeremyaFr)
