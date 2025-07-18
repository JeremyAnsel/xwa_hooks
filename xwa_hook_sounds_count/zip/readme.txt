xwa_hook_sounds_count

This hook increases the sounds count.


*** Requirements ***

This dll requires:
- Windows 7 or superior
- xwa_hook_main


*** Setup ***

Place hook_sounds_count.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that defines sounds count
// SoundEffectNames
At offset 039470, replace 57B9C0590000 with E8ABEA1600C3.
At offset 0397C8, replace E0239D00 with g_SoundEffectNamesArray00.c_str().
At offset 039900, replace E0239D00 with g_SoundEffectNamesArray00.c_str().
At offset 039917, replace E08A9E00 with g_SoundEffectNamesArray_0_00.c_str().
At offset 0397BC, replace C1E005 with C1E006.
At offset 03990C, replace 83C620 with 83C640.
// SfxVolumes and SfxDistances
At offset 0395EC, replace 731B with EB1B.
At offset 03989A, replace 7408 with 9090.
At offset 0398A0, replace 103C5B00 with g_SfxDistancesArray00.c_str().
At offset 0398AA, replace 740A with 9090.
At offset 0398B2, replace A03D5B00 with g_SfxVolumesArray00.c_str().
At offset 03B77E, replace 730C with 9090.
At offset 03B784, replace A03D5B00 with g_SfxVolumesArray00.c_str().
At offset 03B81D, replace 720C with EB0C.
At offset 03B831, replace A03D5B00 with g_SfxVolumesArray00.c_str().
At offset 03B839, replace 103C5B00 with g_SfxDistancesArray00.c_str().
At offset 03C5CE, replace A03D5B00 with g_SfxVolumesArray00.c_str().
At offset 03B867, replace 103C5B00 with g_SfxDistancesArray00.c_str().
At offset 03D1C5, replace 1C3D5B00 with g_SfxDistancesArray86.c_str().
At offset 03D2EA, replace 1C3D5B00 with g_SfxDistancesArray86.c_str().
// SoundEffectIds
At offset 0394FD, replace BF807E9100F3AB with E81EEA16009090.
At offset 057CA0, replace 5CAB9100 with g_SoundEffectIdsArray_1_00.c_str().
// SoundEffectIds
// :004035C1
{ 0x0029C3, "4C809100", g_SoundEffectIdsArray_000073.c_str() },
// :00403F55
{ 0x003357, "50809100", g_SoundEffectIdsArray_000074.c_str() },
// :004289C4
{ 0x027DC7, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :004289FA
{ 0x027DFD, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :00428A30
{ 0x027E33, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043A0AA
{ 0x0394AD, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043A0C4
{ 0x0394C7, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043A0D9
{ 0x0394DC, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043A35B
{ 0x03975E, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043A469
{ 0x03986C, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043A48D
{ 0x039890, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043A4FA
{ 0x0398FB, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043BC82
{ 0x03B084, "50809100", g_SoundEffectIdsArray_000074.c_str() },
// :0043BC95
{ 0x03B097, "50809100", g_SoundEffectIdsArray_000074.c_str() },
// :0043BE28
{ 0x03B22B, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043BE7B
{ 0x03B27E, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043BEF9
{ 0x03B2FC, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043C00C
{ 0x03B40F, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043C0D7
{ 0x03B4DA, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043C158
{ 0x03B55B, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043C801
{ 0x03BC02, "B47F9100", g_SoundEffectIdsArray_00004D.c_str() },
// :0043C813
{ 0x03BC15, "B47F9100", g_SoundEffectIdsArray_00004D.c_str() },
// :0043C822
{ 0x03BC24, "B07F9100", g_SoundEffectIdsArray_00004C.c_str() },
// :0043C835
{ 0x03BC36, "B07F9100", g_SoundEffectIdsArray_00004C.c_str() },
// :0043C843
{ 0x03BC45, "B87F9100", g_SoundEffectIdsArray_00004E.c_str() },
// :0043C856
{ 0x03BC58, "B87F9100", g_SoundEffectIdsArray_00004E.c_str() },
// :0043C883
{ 0x03BC84, "B47F9100", g_SoundEffectIdsArray_00004D.c_str() },
// :0043C895
{ 0x03BC97, "B47F9100", g_SoundEffectIdsArray_00004D.c_str() },
// :0043C89E
{ 0x03BCA0, "B07F9100", g_SoundEffectIdsArray_00004C.c_str() },
// :0043C8B1
{ 0x03BCB2, "B07F9100", g_SoundEffectIdsArray_00004C.c_str() },
// :0043C8BF
{ 0x03BCC1, "B87F9100", g_SoundEffectIdsArray_00004E.c_str() },
// :0043C8E8
{ 0x03BCEA, "B87F9100", g_SoundEffectIdsArray_00004E.c_str() },
// :0043C91F
{ 0x03BD20, "B47F9100", g_SoundEffectIdsArray_00004D.c_str() },
// :0043C931
{ 0x03BD33, "B47F9100", g_SoundEffectIdsArray_00004D.c_str() },
// :0043C93A
{ 0x03BD3C, "B87F9100", g_SoundEffectIdsArray_00004E.c_str() },
// :0043C94D
{ 0x03BD4E, "B87F9100", g_SoundEffectIdsArray_00004E.c_str() },
// :0043CA14
{ 0x03BE16, "B07F9100", g_SoundEffectIdsArray_00004C.c_str() },
// :0043CA4E
{ 0x03BE50, "B07F9100", g_SoundEffectIdsArray_00004C.c_str() },
// :0043CA61
{ 0x03BE63, "B07F9100", g_SoundEffectIdsArray_00004C.c_str() },
// :0043CA70
{ 0x03BE71, "B87F9100", g_SoundEffectIdsArray_00004E.c_str() },
// :0043CA82
{ 0x03BE84, "B87F9100", g_SoundEffectIdsArray_00004E.c_str() },
// :0043CA91
{ 0x03BE93, "B47F9100", g_SoundEffectIdsArray_00004D.c_str() },
// :0043CAB0
{ 0x03BEB1, "B47F9100", g_SoundEffectIdsArray_00004D.c_str() },
// :0043CB71
{ 0x03BF73, "BC7F9100", g_SoundEffectIdsArray_00004F.c_str() },
// :0043CB97
{ 0x03BF99, "C87F9100", g_SoundEffectIdsArray_000052.c_str() },
// :0043CBC1
{ 0x03BFC4, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043CBEC
{ 0x03BFEF, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043CC1A
{ 0x03C01D, "847E9100", g_SoundEffectIdsArray_000001.c_str() },
// :0043CC2E
{ 0x03C031, "847E9100", g_SoundEffectIdsArray_000001.c_str() },
// :0043CC3E
{ 0x03C041, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043CC69
{ 0x03C06C, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043CC8C
{ 0x03C08F, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043CCA0
{ 0x03C0A3, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043CCB0
{ 0x03C0B3, "847E9100", g_SoundEffectIdsArray_000001.c_str() },
// :0043CCDE
{ 0x03C0E1, "847E9100", g_SoundEffectIdsArray_000001.c_str() },
// :0043CD05
{ 0x03C107, "D47F9100", g_SoundEffectIdsArray_000055.c_str() },
// :0043CD1C
{ 0x03C11D, "D87F9100", g_SoundEffectIdsArray_000056.c_str() },
// :0043CD46
{ 0x03C148, "D87F9100", g_SoundEffectIdsArray_000056.c_str() },
// :0043CD68
{ 0x03C169, "DC7F9100", g_SoundEffectIdsArray_000057.c_str() },
// :0043CD7A
{ 0x03C17C, "E07F9100", g_SoundEffectIdsArray_000058.c_str() },
// :0043CDAB
{ 0x03C1AC, "E07F9100", g_SoundEffectIdsArray_000058.c_str() },
// :0043CDC2
{ 0x03C1C3, "BC7F9100", g_SoundEffectIdsArray_00004F.c_str() },
// :0043CDE4
{ 0x03C1E6, "E07F9100", g_SoundEffectIdsArray_000058.c_str() },
// :0043CE20
{ 0x03C221, "7C7F9100", g_SoundEffectIdsArray_00003F.c_str() },
// :0043CE32
{ 0x03C234, "7C7F9100", g_SoundEffectIdsArray_00003F.c_str() },
// :0043CE41
{ 0x03C243, "807F9100", g_SoundEffectIdsArray_000040.c_str() },
// :0043CE58
{ 0x03C259, "807F9100", g_SoundEffectIdsArray_000040.c_str() },
// :0043CE85
{ 0x03C286, "807F9100", g_SoundEffectIdsArray_000040.c_str() },
// :0043CE97
{ 0x03C299, "807F9100", g_SoundEffectIdsArray_000040.c_str() },
// :0043CEB6
{ 0x03C2B8, "7C7F9100", g_SoundEffectIdsArray_00003F.c_str() },
// :0043CEC9
{ 0x03C2CA, "7C7F9100", g_SoundEffectIdsArray_00003F.c_str() },
// :0043CED9
{ 0x03C2DB, "7C7F9100", g_SoundEffectIdsArray_00003F.c_str() },
// :0043CEEC
{ 0x03C2EE, "7C7F9100", g_SoundEffectIdsArray_00003F.c_str() },
// :0043CF0B
{ 0x03C30C, "807F9100", g_SoundEffectIdsArray_000040.c_str() },
// :0043CF1D
{ 0x03C31F, "807F9100", g_SoundEffectIdsArray_000040.c_str() },
// :0043CF80
{ 0x03C381, "D87E9100", g_SoundEffectIdsArray_000016.c_str() },
// :0043CF96
{ 0x03C398, "D87E9100", g_SoundEffectIdsArray_000016.c_str() },
// :0043CFD5
{ 0x03C3D7, "D87E9100", g_SoundEffectIdsArray_000016.c_str() },
// :0043CFEC
{ 0x03C3EE, "D87E9100", g_SoundEffectIdsArray_000016.c_str() },
// :0043D029
{ 0x03C42B, "D87E9100", g_SoundEffectIdsArray_000016.c_str() },
// :0043D03C
{ 0x03C43D, "D87E9100", g_SoundEffectIdsArray_000016.c_str() },
// :0043D05D
{ 0x03C45F, "D87E9100", g_SoundEffectIdsArray_000016.c_str() },
// :0043D070
{ 0x03C472, "D87E9100", g_SoundEffectIdsArray_000016.c_str() },
// :0043D233
{ 0x03C636, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043D24D
{ 0x03C650, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043D286
{ 0x03C689, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043D29F
{ 0x03C6A2, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043D2C2
{ 0x03C6C5, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043D2D6
{ 0x03C6D9, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043D2E6
{ 0x03C6E8, "44809100", g_SoundEffectIdsArray_000071.c_str() },
// :0043D2F9
{ 0x03C6FA, "44809100", g_SoundEffectIdsArray_000071.c_str() },
// :0043D307
{ 0x03C709, "48809100", g_SoundEffectIdsArray_000072.c_str() },
// :0043D31E
{ 0x03C720, "48809100", g_SoundEffectIdsArray_000072.c_str() },
// :0043D38F
{ 0x03C792, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043D3A3
{ 0x03C7A6, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043D3B3
{ 0x03C7B5, "44809100", g_SoundEffectIdsArray_000071.c_str() },
// :0043D3C6
{ 0x03C7C8, "44809100", g_SoundEffectIdsArray_000071.c_str() },
// :0043D3D5
{ 0x03C7D6, "48809100", g_SoundEffectIdsArray_000072.c_str() },
// :0043D3E7
{ 0x03C7E9, "48809100", g_SoundEffectIdsArray_000072.c_str() },
// :0043D5AA
{ 0x03C9AC, "E87F9100", g_SoundEffectIdsArray_00005A.c_str() },
// :0043D5BD
{ 0x03C9BF, "E87F9100", g_SoundEffectIdsArray_00005A.c_str() },
// :0043D5CC
{ 0x03C9CD, "EC7F9100", g_SoundEffectIdsArray_00005B.c_str() },
// :0043D5DE
{ 0x03C9E0, "EC7F9100", g_SoundEffectIdsArray_00005B.c_str() },
// :0043D5ED
{ 0x03C9EF, "F07F9100", g_SoundEffectIdsArray_00005C.c_str() },
// :0043D600
{ 0x03CA01, "F07F9100", g_SoundEffectIdsArray_00005C.c_str() },
// :0043D60E
{ 0x03CA10, "F47F9100", g_SoundEffectIdsArray_00005D.c_str() },
// :0043D625
{ 0x03CA27, "F47F9100", g_SoundEffectIdsArray_00005D.c_str() },
// :0043D65A
{ 0x03CA5C, "E87F9100", g_SoundEffectIdsArray_00005A.c_str() },
// :0043D66D
{ 0x03CA6F, "E87F9100", g_SoundEffectIdsArray_00005A.c_str() },
// :0043D68C
{ 0x03CA8D, "EC7F9100", g_SoundEffectIdsArray_00005B.c_str() },
// :0043D6A2
{ 0x03CAA4, "F07F9100", g_SoundEffectIdsArray_00005C.c_str() },
// :0043D6E0
{ 0x03CAE3, "E87F9100", g_SoundEffectIdsArray_00005A.c_str() },
// :0043D6F2
{ 0x03CAF3, "E87F9100", g_SoundEffectIdsArray_00005A.c_str() },
// :0043D704
{ 0x03CB06, "E87F9100", g_SoundEffectIdsArray_00005A.c_str() },
// :0043D713
{ 0x03CB15, "EC7F9100", g_SoundEffectIdsArray_00005B.c_str() },
// :0043D726
{ 0x03CB27, "EC7F9100", g_SoundEffectIdsArray_00005B.c_str() },
// :0043D734
{ 0x03CB36, "F07F9100", g_SoundEffectIdsArray_00005C.c_str() },
// :0043D747
{ 0x03CB49, "F07F9100", g_SoundEffectIdsArray_00005C.c_str() },
// :0043D75D
{ 0x03CB5E, "F47F9100", g_SoundEffectIdsArray_00005D.c_str() },
// :0043D771
{ 0x03CB73, "F47F9100", g_SoundEffectIdsArray_00005D.c_str() },
// :0043D798
{ 0x03CB9A, "F47F9100", g_SoundEffectIdsArray_00005D.c_str() },
// :0043D7AB
{ 0x03CBAC, "F47F9100", g_SoundEffectIdsArray_00005D.c_str() },
// :0043D8D9
{ 0x03CCDC, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043D901
{ 0x03CD04, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043D923
{ 0x03CD26, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043D936
{ 0x03CD38, "44809100", g_SoundEffectIdsArray_000071.c_str() },
// :0043D949
{ 0x03CD4A, "44809100", g_SoundEffectIdsArray_000071.c_str() },
// :0043D957
{ 0x03CD59, "48809100", g_SoundEffectIdsArray_000072.c_str() },
// :0043D96A
{ 0x03CD6C, "48809100", g_SoundEffectIdsArray_000072.c_str() },
// :0043DA1C
{ 0x03CE1E, "18809100", g_SoundEffectIdsArray_000066.c_str() },
// :0043DA42
{ 0x03CE44, "20809100", g_SoundEffectIdsArray_000068.c_str() },
// :0043DA65
{ 0x03CE67, "28809100", g_SoundEffectIdsArray_00006A.c_str() },
// :0043DA85
{ 0x03CE87, "30809100", g_SoundEffectIdsArray_00006C.c_str() },
// :0043DAA5
{ 0x03CEA7, "38809100", g_SoundEffectIdsArray_00006E.c_str() },
// :0043DAC5
{ 0x03CEC6, "40809100", g_SoundEffectIdsArray_000070.c_str() },
// :0043DE31
{ 0x03D232, "98809100", g_SoundEffectIdsArray_000086.c_str() },
// :0043DE4D
{ 0x03D24E, "98809100", g_SoundEffectIdsArray_000086.c_str() },
// :0043DE68
{ 0x03D26A, "98809100", g_SoundEffectIdsArray_000086.c_str() },
// :0043DE75
{ 0x03D276, "98809100", g_SoundEffectIdsArray_000086.c_str() },
// :0043DE88
{ 0x03D28A, "98809100", g_SoundEffectIdsArray_000086.c_str() },
// :0043DEC8
{ 0x03D2C9, "98809100", g_SoundEffectIdsArray_000086.c_str() },
// :0043DEDA
{ 0x03D2DC, "98809100", g_SoundEffectIdsArray_000086.c_str() },
// :0043E02D
{ 0x03D42F, "787F9100", g_SoundEffectIdsArray_00003E.c_str() },
// :0043E8B8
{ 0x03DCBB, "A8829100", g_SoundEffectIdsArray_00010A.c_str() },
// :0043F4E8
{ 0x03E8EB, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043F6C2
{ 0x03EAC5, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0043F791
{ 0x03EB94, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :0045887C
{ 0x057C7D, "907E9100", g_SoundEffectIdsArray_000004.c_str() },
// :00458E25
{ 0x058226, "907E9100", g_SoundEffectIdsArray_000004.c_str() },
// :00458E47
{ 0x058249, "90819100", g_SoundEffectIdsArray_0000C4.c_str() },
// :00458E91
{ 0x058293, "18809100", g_SoundEffectIdsArray_000066.c_str() },
// :00458EB6
{ 0x0582B8, "20809100", g_SoundEffectIdsArray_000068.c_str() },
// :00458ED8
{ 0x0582DA, "28809100", g_SoundEffectIdsArray_00006A.c_str() },
// :00458EF7
{ 0x0582F9, "30809100", g_SoundEffectIdsArray_00006C.c_str() },
// :00458F16
{ 0x058318, "38809100", g_SoundEffectIdsArray_00006E.c_str() },
// :00458F35
{ 0x058336, "40809100", g_SoundEffectIdsArray_000070.c_str() },
// :0045EDFD
{ 0x05E1FF, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045EE14
{ 0x05E216, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045EE30
{ 0x05E232, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045EE41
{ 0x05E243, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045EE6F
{ 0x05E271, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045EFD8
{ 0x05E3DA, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045F092
{ 0x05E494, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045F0A4
{ 0x05E4A6, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045F0B0
{ 0x05E4B1, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045F3B2
{ 0x05E7B3, "CC809100", g_SoundEffectIdsArray_000093.c_str() },
// :0045F623
{ 0x05EA25, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045F63A
{ 0x05EA3B, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045F655
{ 0x05EA57, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045F666
{ 0x05EA67, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045F7C3
{ 0x05EBC4, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045F86C
{ 0x05EC6E, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045F87D
{ 0x05EC7E, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045F889
{ 0x05EC8B, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045F8C9
{ 0x05ECCA, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045F998
{ 0x05ED9A, "CC809100", g_SoundEffectIdsArray_000093.c_str() },
// :0045FAEA
{ 0x05EEEC, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045FB01
{ 0x05EF02, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045FB1C
{ 0x05EF1E, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :0045FB2D
{ 0x05EF2E, "D8809100", g_SoundEffectIdsArray_000096.c_str() },
// :004627BC
{ 0x061BBF, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :004627D0
{ 0x061BD3, "807E9100", g_SoundEffectIdsArray_000000.c_str() },
// :004DB1DF
{ 0x0DA5E1, "647F9100", g_SoundEffectIdsArray_000039.c_str() },
// :004DB24F
{ 0x0DA651, "647F9100", g_SoundEffectIdsArray_000039.c_str() },
// :004DB255
{ 0x0DA657, "507F9100", g_SoundEffectIdsArray_000034.c_str() },
// :004DB25D
{ 0x0DA65F, "547F9100", g_SoundEffectIdsArray_000035.c_str() },
// :004DB265
{ 0x0DA667, "587F9100", g_SoundEffectIdsArray_000036.c_str() },
// :004DB26D
{ 0x0DA66F, "5C7F9100", g_SoundEffectIdsArray_000037.c_str() },
// :004DB275
{ 0x0DA677, "607F9100", g_SoundEffectIdsArray_000038.c_str() },
// :004DB281
{ 0x0DA683, "507F9100", g_SoundEffectIdsArray_000034.c_str() },
// :004DB289
{ 0x0DA68B, "547F9100", g_SoundEffectIdsArray_000035.c_str() },
// :004DB291
{ 0x0DA693, "587F9100", g_SoundEffectIdsArray_000036.c_str() },
// :004DB299
{ 0x0DA69B, "5C7F9100", g_SoundEffectIdsArray_000037.c_str() },
// :004DB2A1
{ 0x0DA6A3, "607F9100", g_SoundEffectIdsArray_000038.c_str() },
// :005168A6
{ 0x115CA8, "68819100", g_SoundEffectIdsArray_0000BA.c_str() },
// :005168EE
{ 0x115CEF, "84819100", g_SoundEffectIdsArray_0000C1.c_str() },
// :00516902
{ 0x115D04, "84819100", g_SoundEffectIdsArray_0000C1.c_str() },
// :0051691B
{ 0x115D1D, "84819100", g_SoundEffectIdsArray_0000C1.c_str() },
// SoundEffectBuffer
// :004DAD6E
{ 0x0DA16F, "00060000", g_SoundEffectBufferArrayCount.c_str() },
// :004142D0
{ 0x0136D1, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :00414339
{ 0x01373A, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DA709
{ 0x0D9B0A, "14B99100", g_SoundEffectBufferArrayC4.c_str() },
// :004DA71E
{ 0x0D9B1F, "146F9600", g_SoundEffectBufferArray_0_C4.c_str() },
// :004DAAE4
{ 0x0D9EE5, "876D9600", g_SoundEffectBufferArray_1_00.c_str() },
// :004DAB0F
{ 0x0D9F12, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DAB6E
{ 0x0D9F70, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DAB8F
{ 0x0D9F92, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DABEC
{ 0x0D9FEE, "14B99100", g_SoundEffectBufferArrayC4.c_str() },
// :004DABF8
{ 0x0D9FFA, "10B99100", g_SoundEffectBufferArrayC0.c_str() },
// :004DAC76
{ 0x0DA078, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DACB9
{ 0x0DA0BA, "14B99100", g_SoundEffectBufferArrayC4.c_str() },
// :004DACCF
{ 0x0DA0D0, "146F9600", g_SoundEffectBufferArray_0_C4.c_str() },
// :004DAD92
{ 0x0DA195, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DAF38
{ 0x0DA33B, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DAF85
{ 0x0DA386, "876D9600", g_SoundEffectBufferArray_1_00.c_str() },
// :004DAFB0
{ 0x0DA3B3, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DB00F
{ 0x0DA411, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DB030
{ 0x0DA433, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DB08D
{ 0x0DA48F, "14B99100", g_SoundEffectBufferArrayC4.c_str() },
// :004DB099
{ 0x0DA49B, "10B99100", g_SoundEffectBufferArrayC0.c_str() },
// :004DB117
{ 0x0DA519, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DB2C8
{ 0x0DA6CB, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DB2F3
{ 0x0DA6F6, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DB327
{ 0x0DA72A, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DB3D2
{ 0x0DA7D5, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DB4A3
{ 0x0DA8A6, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DB667
{ 0x0DAA6A, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DB6F0
{ 0x0DAAF2, "18B99100", g_SoundEffectBufferArrayC8.c_str() },
// :004DB6F6
{ 0x0DAAF8, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DB703
{ 0x0DAB06, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DB7DC
{ 0x0DABDE, "10B99100", g_SoundEffectBufferArrayC0.c_str() },
// :004DB7F9
{ 0x0DABFB, "10B99100", g_SoundEffectBufferArrayC0.c_str() },
// :004DB91C
{ 0x0DAD1E, "10B99100", g_SoundEffectBufferArrayC0.c_str() },
// :004DBC8C
{ 0x0DB08E, "18B99100", g_SoundEffectBufferArrayC8.c_str() },
// :004DBCAA
{ 0x0DB0AC, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DBCBF
{ 0x0DB0C1, "10B99100", g_SoundEffectBufferArrayC0.c_str() },
// :004DBCC8
{ 0x0DB0CA, "90B89100", g_SoundEffectBufferArray40.c_str() },
// :004DC521
{ 0x0DB924, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DC528
{ 0x0DB92B, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DC549
{ 0x0DB94C, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DC659
{ 0x0DBA5C, "50B89100", g_SoundEffectBufferArray00.c_str() },
// :004DC838
{ 0x0DBC3B, "18B99100", g_SoundEffectBufferArrayC8.c_str() },
// :004DC8DA
{ 0x0DBCDD, "50B89100", g_SoundEffectBufferArray00.c_str() },
// Load effect sounds
At offset 0306B0, replace E89B8E0000 with E86B781700.
At offset 039618, replace 8A44242C8D74242C with E803E91600909090.
At offset 0397BF, replace 8D7C242C83C9FF with E85CE716009090.
// s_XwaSound3dEnabled
At offset 1B9D90, replace 02 with 00.


*** Usage ***

To add new sound effects list, create a file with the ".lst" extension and with a filename starting with "Sfx_" in the "Wave" directory. The format of this file is the same as the "SFXBLASTNEW.LST" file.
Place the new sounds in subdirectories in the "Wave" directory. The name of these directories is the basename of the ".lst" files without the extension.

When the hook is executed, it creates a file named "Hook_Sounds_Count.txt". This file contains the definition of the index and count for the custom sound effect lists.
This file is then used by the other hooks to find where are the custom sounds in the sound effects arrays.


*** Credits ***

- Jérémy Ansel (JeremyaFr)
