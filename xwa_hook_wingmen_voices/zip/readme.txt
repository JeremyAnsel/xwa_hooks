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

# To call the hook that increases the sound effect buffer size
At offset 0DA16F, replace 00060000 with g_SoundEffectBufferArrayCount.
At offset 0136D1, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 01373A, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0D9B0A, replace 14B99100 with g_SoundEffectBufferArrayC4.
At offset 0D9B1F, replace 146F9600 with g_SoundEffectBufferArray_0_C4.
At offset 0D9EE5, replace 876D9600 with g_SoundEffectBufferArray_1_00.
At offset 0D9F12, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0D9F70, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0D9F92, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0D9FEE, replace 14B99100 with g_SoundEffectBufferArrayC4.
At offset 0D9FFA, replace 10B99100 with g_SoundEffectBufferArrayC0.
At offset 0DA078, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DA0BA, replace 14B99100 with g_SoundEffectBufferArrayC4.
At offset 0DA0D0, replace 146F9600 with g_SoundEffectBufferArray_0_C4.
At offset 0DA195, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DA33B, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DA386, replace 876D9600 with g_SoundEffectBufferArray_1_00.
At offset 0DA3B3, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DA411, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DA433, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DA48F, replace 14B99100 with g_SoundEffectBufferArrayC4.
At offset 0DA49B, replace 10B99100 with g_SoundEffectBufferArrayC0.
At offset 0DA519, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DA6CB, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DA6F6, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DA72A, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DA7D5, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DA8A6, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DAA6A, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DAAF2, replace 18B99100 with g_SoundEffectBufferArrayC8.
At offset 0DAAF8, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DAB06, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DABDE, replace 10B99100 with g_SoundEffectBufferArrayC0.
At offset 0DABFB, replace 10B99100 with g_SoundEffectBufferArrayC0.
At offset 0DAD1E, replace 10B99100 with g_SoundEffectBufferArrayC0.
At offset 0DB08E, replace 18B99100 with g_SoundEffectBufferArrayC8.
At offset 0DB0AC, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DB0C1, replace 10B99100 with g_SoundEffectBufferArrayC0.
At offset 0DB0CA, replace 90B89100 with g_SoundEffectBufferArray40.
At offset 0DB924, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DB92B, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DB94C, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DBA5C, replace 50B89100 with g_SoundEffectBufferArray00.
At offset 0DBC3B, replace 18B99100 with g_SoundEffectBufferArrayC8.
At offset 0DBCDD, replace 50B89100 with g_SoundEffectBufferArray00.


*** Usage ***

To choose  an imperial wingman voice, set pilot voice to the desired value or set IFF to 1 (imperial).
For values IP1 to IP12, ISP1.LST to ISP12.LST will be loaded.
If IFF is set to 1 (imperial), random imperial voices will be loaded. Else random rebel voices will be loaded.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
