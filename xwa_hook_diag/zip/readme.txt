xwa_hook_diag

This hook shows diagnostic messages.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_diag.dll and hook_diag.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that shows diag messages
At offset 0CBD2A, replace E8114F0600 with E801C20D00.
At offset 13E440, replace A12A709F00 with E8DB9A0600.
At offset 05A3E5, replace A1B0127700 with E836DB1400.
At offset 0F1655, replace A1B0127700 with E8C6680B00.
At offset 0CBE3B, replace E810190400 with E8E0C00D00.
At offset 0317E3, replace E858BA0900 with E838671700.
At offset 13DB8F, replace A171C0A100 with E88CA30600.
At offset 127F3C, replace E86F4E0300 with E8DFFF0700.


*** Usage ***

To shows diagnostic messages, set the "ShowDiagMessages" setting in the "hook_diag.cfg" file or in the "hook_diag" section of "Hooks.ini".

See "hook_diag.cfg"

Reference for the loading times
step 0 is start.
step 1 is mission file loading, global buffers initializing, and hangar sounds loading.
step 2 is tactical officers and wingmen sounds loading.
step 3 is 3d initializing, and OPTs loading.
step 4 is dat images loading
step 5 is ResData_LightingEffects initializing
step 6 is nothing
step 7 is backdrops initializing, and HUD initializing
step 8 is starships debris initializing

*** Credits ***

- Jérémy Ansel (JeremyaFr)
