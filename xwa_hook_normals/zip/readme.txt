xwa_hook_normals

This hook transmits vertex normals to DDraw via the specular component.

The w part of the specular component is set to 1.0f.
The xyz parts contains the normal:
0.0 means -1.0
0.5 means 0.0
1.0 means 1.0

*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main


*** Setup ***

Place hook_normals.dll and hook_normals.cfg next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that transmits normals
At offset 88554, replace 8915C66C9B00 with E8C7F9110090.
At offset 41E2B, replace E850FAFFFF with E8F0601600.
At offset 429D3, replace E808520000 with E858551600.
At offset 429EB, replace E8F0510000 with E840551600.
At offset 4294D, replace E86E540000 with E8DE551600.
At offset 4296F, replace E84C540000 with E8BC551600.
At offset 426AB, replace 8B79108978108B79148978148B79188978188B791C89781C with 5150E86E58160083C4089090909090909090909090909090.
At offset 427A9, replace E832440000 with E872571600.
At offset 4281B, replace E8100C0000 with E800571600.
At offset 4285D, replace E88E190000 with E8BE561600.
At offset 4289F, replace E85C270000 with E87C561600.
At offset 428E2, replace E8E9340000 with E839561600.


*** Usage ***

See the "hook_normals.cfg" file or the "hook_normals" section of "hooks.ini".


*** Credits ***

- Jérémy Ansel (JeremyaFr)
