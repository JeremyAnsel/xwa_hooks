xwa_hook_tourmultiplayer

This hook allows you to use the built in campaign/tour mode within XWA to play the story campaign with other players, or fly custom campaigns.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_tourmultiplayer.dll and hook_tourmultiplayer.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that enables multiplayer tour
...


*** Usage ***

To enable or disable the hook, set the "IsHookEnabled" setting in hook_tourmultiplayer.cfg. By default the hook is disabled.
...


*** Credits ***

- Justagai
- Jérémy Ansel (JeremyaFr)
