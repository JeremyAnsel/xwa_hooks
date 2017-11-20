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
At offset 03AC24, replace 8B0D78405B008B157C405B00 with 53E8F6D2160083C404EB7B90.
At offset 03AE4C, replace 8B0D78405B008B157C405B00 with 53E8CED0160083C404EB7990.


*** Usage ***

To choose  an imperial wingman voice, set IFF to 1 (imperial) and set pilot voice to the desired value.
Values are IP1, IP2, IP3, IP4, IP5, IP5 to load (ISP1.LST, ISP2.LST, ISP3.LST, ISP4.LST, ISP5.LST, ISP6.LST). The default value is IP1.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
