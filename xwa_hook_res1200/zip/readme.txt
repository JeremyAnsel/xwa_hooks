xwa_hook_res1200

This hook replaces a static buffer with a dynamic one in initializing 2d buffer. It fixes a crash when resolution height > 1200 is used.

It also fixes a crash at offset 0x000d332d.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_res1200.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that fix res 1200 crash
At offset 0D31FA, replace 8901 with 9090.
At offset 0D31E0, replace 8B156C4B7D00 with E83B4D0D00C3.
At offset 0D32F0, replace C705A0D57400A0907F00 with E82B4C0D009090909090.
At offset 0D330D, replace 8B54241C33C985D27620 with E80E4C0D005F5E5D5BC3.
At offset 0D3374, replace 892C85E0C27400 with 90909090909090.
At offset 0D3399, replace C705A0D57400E0C27400 with E8824B0D009090909090.

# To call the hook that fix offset d332d crash
At offset 048056, replace 6A01 with 6A00.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
