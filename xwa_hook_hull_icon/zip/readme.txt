xwa_hook_hull_icon

This hook enables setting hull icon for any craft.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

1) add hook_hull_icon.dll to dinput_hooks.lst

2) place hook_hull_icon.dll and HullIconList.txt next to xwingalliance.exe

3) edit xwingalliance.exe:

You can manually apply the modifications with an hexadecimal editor, or you can use XwaExePatcher to have a GUI.

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
