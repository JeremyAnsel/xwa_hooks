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

# To call the hook that defines crafts count
At offset 014C3D, replace 890D5CF38B00892D507E9100 with 56E8DD3219005B9090909090.
At offset 014C60, replace 83FF01A3804B7D00 with E8CB321900EB1290.
At offset 014C83, replace 8BC6C1E005 with E8A8321900.
At offset 014E06, replace A180FD7F00 with E825311900.
At offset 0A95AA, replace 8A847980F19A00 with 57E880E90F005F.
At offset 0A9AD5, replace C6847080F19A0000 with 56E845E40F005E90.
At offset 0A9B5F, replace C6847180F19A0001 with 56E8BBE30F005E90.
At offset 039521, replace 8D3C8560FD9C00 with 50E8F9E916005F.
At offset 03E842, replace 8B0CB560FD9C00 with 56E8D896160059.
At offset 03E85D, replace 8904B560FD9C00 with 50E8BD96160058.

# To call the hook that applies RS's crafts count
At offset 0BFC55, replace 25FFFF0000 with E8C6820E00.
At offset 0C0D57, replace 25FFFF0000 with E8C4710E00.
At offset 030216, replace 68007D0000 with 6880A21900.
At offset 03B4E9, replace 7407 with EB07.

# To call the hook that defines radar2d items count
At offset 074F09, replace 742AC70568C86800C8BE6800 with E812301300EB4D9090909090.
At offset 075741, replace 0F85C300000066C70558C868002F00 with E8DA27130090909090909090909090.
At offset 0757FF, replace 750966C7055CC868002F00 with E81C271300909090909090.


*** Usage ***

To enable or disable the hook, set the "IsHookEnabled" setting in hook_crafts_count.cfg. By default the hook is disabled.
You can define a value for CraftsCount, ProjectilesCount, ExplosionsCount, Radar2DItemsCount.

See "hook_crafts_count.cfg"


*** Credits ***

- Jérémy Ansel (JeremyaFr)
- Random Starfighter
