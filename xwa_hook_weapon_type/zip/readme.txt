xwa_hook_weapon_type

This hook fix the warheads selection for crafts that have dual warheads.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_weapon_type.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that sets the warhead menu
At offset 060798, replace 0F872D020000 with 0F8711000000.
At offset 060885, replace 8B0DFC0D9100 with E89676140090.
At offset 05BFCB, replace 7516 with EB16.
At offset 05BEA8, replace A1FC0D9100 with E873C01400.
At offset 05BF04, replace FEC1 with 9090.
At offset 0B0801, replace 7505 with EB05.
At offset 0B07F8, replace 660FB63455EEDC8000 with E823770F0090909090.
;At offset 073B6A, replace 03C1 with 03C8.
;At offset 073B75, replace 6685C0 with 6685C9.
;At offset 073B7E, replace 6683FE01 with 6685C090.
;At offset 073B82, replace 0F8542010000 with 0F854C010000.
At offset 0FBA87, replace 3A83D10100007218 with E894C40A0090EB18.
At offset 0FBAA0, replace C68613958B0000 with E87BC40A009090.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
