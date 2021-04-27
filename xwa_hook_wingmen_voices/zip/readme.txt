xwa_hook_wingmen_voices

This hook enables setting wingmen voices for imperial IFF.

For more informations, please see the following thread at xwaupgrade.com:
http://www.xwaupgrade.com/phpBB3008/viewtopic.php?f=9&t=11770


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_wingmen_voices.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that defines wingmen voices
At offset 03AC22, replace 7528 with 9090.
At offset 03AC24, replace 8B0D78405B008B157C405B00A080405B00 with 53E806D3160083C40485C07479EB199090.
At offset 03AE4A, replace 7528 with 9090.
At offset 03AE4C, replace 8B0D78405B008B157C405B00A080405B00 with 53E8DED0160083C40485C07477EB199090.


*** Usage ***

To choose  an imperial wingman voice, set pilot voice to the desired value or set IFF to 1 (imperial).
For values IP1 to IP12, ISP1.LST to ISP12.LST will be loaded.
If IFF is set to 1 (imperial), random imperial voices will be loaded. Else random rebel voices will be loaded.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
