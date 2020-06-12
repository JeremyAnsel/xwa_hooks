xwa_hook_music

This hook fixes the music freeze.

It replaces the DirectSound API with the XAudio2 API to play the music.
With this hook, it is no longer needed to set the process affinity mask to a single core.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_music.dll and xaudio2_9redist.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that fixes the music freeze
At offset 191F44, replace 0F84C4000000 with 90E9C4000000.
At offset 192015, replace 7504 with EB04.
At offset 19201B, replace 6A016A006A00A1040F7B00508B0D040F7B008B11FF5230 with 909090909090909090909090909090909090E8EE5E0100.
At offset 192252, replace C705080F7B0001000000 with E8D95C01009090909090.
At offset 19216A, replace E8EC020000 with E8C15D0100.
At offset 1921B8, replace 6A0068CC0E7B0068240F7B0068F80E7B0068200F7B006800200000A114876000C1E00D508B0D040F7B00518B15040F7B008B02FF502C with 90909090909090909090909090909090909090909090909090909090909090909090909090909090909090909090909090E8425D0100.
At offset 19212C, replace 8B0DCC0E7B00518B15240F7B0052A1F80E7B00508B0D200F7B00518B15040F7B0052A1040F7B008B08FF514C with 909090909090909090909090909090909090909090909090909090909090909090909090909090E8D85D0100.
At offset 13D3DC, replace 57FF15D8925A00 with E84FAB06009090.
At offset 1A7C84, replace 6A00FF15D8925A00 with E8A7020000909090.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
