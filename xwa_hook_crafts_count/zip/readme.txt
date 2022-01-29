xwa_hook_crafts_count

This hook increases the crafts count.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_crafts_count.dll and hook_crafts_count.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that ...
At offset , replace  with .


*** Usage ***

To enable or disable the hook, set the IsHookEnabled setting in hook_crafts_count.cfg.
See "hook_crafts_count.cfg"


*** Credits ***

- Jérémy Ansel (JeremyaFr)
- Random Starfighter
