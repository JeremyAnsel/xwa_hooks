xwa_hook_gimbal_lock

This hook fixes the gimbal lock.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_gimbal_lock.dll and hook_gimbal_lock.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that fixes the gimbal lock
At offset 0F9FA6, replace E845970000 with E875DF0A00.


*** Usage ***

To enable or disable the gimbal lock fix, set the "GimbalLockFixEnabled" setting in "hook_gimbal_lock.cfg".


*** Credits ***

- Jérémy Ansel (JeremyaFr)
