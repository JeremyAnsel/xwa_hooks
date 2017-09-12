xwa_hook_hull_icon

This hook enables setting hull icon for any craft.

For more informations, please see the following thread at xwaupgrade.com:
http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11761


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_hull_icon.dll and HullIconList.txt next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that defines hull icons
At offset 067089, replace 663D4700771925FFFF0000668B0C4508965A00 with 25FFFF000050E89C0E140083C4048BC8909090.
At offset 06A1C0, replace 663D4700770E25FFFF0000668B044508965A00 with 25FFFF000050E865DD130083C4049090909090.
At offset 06A470, replace 663D4700771925FFFF0000668B0C4508965A00 with 25FFFF000050E8B5DA130083C4048BC8909090.
At offset 06A841, replace 663D4700771925FFFF0000668B0C4508965A00 with 25FFFF000050E8E4D6130083C4048BC8909090.


*** Usage ***

To set the hull icon for a craft, open "HullIconList.txt" and change the corresponding line to the desired value.
If the file does not exist, default values are used.
See HullIconList.txt.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
