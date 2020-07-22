xwa_hook_d3d

This hook improves FPS.


*** Requirements ***

This dll requires:
- Windows XP SP2 or superior
- xwa_hook_main
- a CPU with SSE2 support
- custom DDraw.dll with support for the d3d hook


*** Setup ***

Place hook_d3d.dll and hook_d3d.cfg next to xwingalliance.exe

Set "IsHookD3DEnabled = 1" in "hook_d3d.cfg" or in the "hook_d3d" section of "hooks.ini".


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that disables flush texture cache inflight
At offset 499E0, replace E808D21400 with 9090909090.
At offset 196B96, replace 8B5508 with EB5190.

# To call the hook that sets 3d scene
At offset 194270, replace A180117B00 with E8BB3C0100.
At offset 1942FE, replace A180117B00 with E82D3C0100.
At offset 1944B4, replace 760432C0EB4A with 909090909090.
At offset 19440A, replace C705281D7B0000000000 with E8213B010085C0747E90.
At offset 194498, replace A100AA9E0003450CA300AA9E00 with E8933A010085C0746390909090.
At offset 194509, replace A1CC1C7B00A3D01C7B00 with E8223A010085C0747D90.
At offset 194597, replace A16CA99E0003450CA36CA99E00 with E89439010085C00F842E030000.
At offset 1948DC, replace A1CC1C7B00C6000B8B0DCC1C7B00C6410100 with E84F36010085C00F84030100009090909090.
At offset 19486E, replace A1CC1C7B008B4DE8 with 9090909090909090.
At offset 19487E, replace A1CC1C7B008B4DE8 with 9090909090909090.
At offset 19488E, replace A1CC1C7B00 with 9090909090.
At offset 194899, replace 8B0DCC1C7B00 with 8BC890909090.
At offset 1946E7, replace A1CC1C7B008B4DE8 with 9090909090909090.
At offset 1946F7, replace A1CC1C7B008B4DE8 with 9090909090909090.
At offset 194707, replace A1CC1C7B00 with 9090909090.
At offset 194712, replace 8B0DCC1C7B00 with 8BC890909090.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
