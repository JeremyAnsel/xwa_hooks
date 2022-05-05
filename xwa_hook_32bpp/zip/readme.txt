xwa_hook_32bpp

This hook enables 32-bit mode.
8-bit OPTs are converted to 32 bits and the game can load 32-bit OPTs.
The game can load 32-bit DAT images. Existing DAT images are converted to 32 bits.
Skins are supported to customize the appearence of the 3d models.

For more informations, please see the following thread at xwaupgrade.com:
https://www.xwaupgrade.com/phpBB3/viewtopic.php?f=33&t=12257


*** Requirements ***

This dll requires:
- Windows 7 or superior
- .NET framework 4.8
- xwa_hook_main


*** Setup ***

Place hook_32bpp.dll, hook_32bpp_net.dll, JeremyAnsel.ColorQuant.dll, JeremyAnsel.IO.Locator.dll, JeremyAnsel.Xwa.Opt.dll, SharpCompress.dll next to xwingalliance.exe


*** Patch ***

The following modifications are applied at runtime to xwingalliance.exe:

# To call the hook that set the opt name
At offset 0CBF00, replace 8B442404558B6C240C with E82BC00D00C3909090.

# To call the hook that set textures bpp 8 to 32
At offset 1964C5, replace 8A028801 with 8B028901.
At offset 1964CF, replace 83C101 with 83C104.
At offset 1964DE, replace 83C201 with 83C204.
At offset 1955BE, replace 8B4804 with 909090.
At offset 1955C1, replace 898DE8FEFFFF83BDE8FEFFFF00751A with C785E8FEFFFF00000000909090EB1A.
At offset 195D30, replace 755E with EB5E.
At offset 040ED9, replace 8D040B with 8D0499.
At offset 040F0F, replace C7843C0806000008000000 with C7843C0806000020000000.
At offset 040F1A, replace 74098D140B8B4C24208911 with C1A43CFC05000002909090.
At offset 041064, replace 399424080B0000 with BE03000000EB56.
At offset 198A2F, replace 8B484C83E120 with 8B4864C1E918.
At offset 198AED, replace 83E108 with 83E128.
At offset 040F2B, replace 0F84F8000000 with 909090909090.
At offset 040E02, replace 7D0D6838475B00 with E819711600EB08.

# To call the hook that set TextureAlphaMask
At offset 081EA1, replace 8B45083BC20F84000100008B4D0C8B018B4814898EB4000000 with 33C98A4802898EB4000000890D30CA6800E93E010000909090.
At offset 081F8C, replace 8B45083BC274198B450C8B008B4814890D30CA6800 with 33C08A41028986B4000000A330CA6800EB57909090.
At offset 0411CB, replace 8A8C24080B0000 with E8606D16008BC8.
At offset 0CC08F, replace E8AC000000 with 9090909090.

# To call the hook that creates LightMap
At offset 041007, replace C7843CF800000008000000 with C7843CF800000020000000.
At offset 041012, replace 740D68F8465B00E872880C0083C404 with C1A43CEC0000000290909090909090.
At offset 041193, replace 6A01 with 6A03.
At offset 040F31, replace 8B8C24F80A000085C90F84E900000080395F0F84E0000000 with 33D28B4424140FAFC550E8F06F160083C404EB7490909090.

# To call the hook that set color intensity
At offset 0418C5, replace C7020000000033DBC70600000000 with E856661600E99800000090909090.

# To call the hook that set dat bpp to 32
At offset 031A17, replace EB3C with EBB8.
At offset 0319D1, replace 5800000000 with BB03000000.
At offset 0319EB, replace C744243C02000000 with C744243C00000000.
At offset 031552, replace 744081E3FFFF00008D1C5BC1E303668B8B50B25F00 with 33C06681FBA201740340EB0383C00489442410EB0B.
At offset 031572, replace 8BC12500400000F7D81BC024FD83C006 with 81E3FFFF00008D1C5BC1E303EB479090.
At offset 0CD847, replace E8A4000000 with E8D4A60D00.


*** Usage ***

# Skins

Suppose that the craft is "FlightModels\[Model].opt".
To create a skin named "[Skin1]", create a folder named "FlightModels\Skins\[Model]\[Skin1]\".
To replace a texture, place it in the skins folder with the same name, or the name of the skin appended, and dimensions of the original texture.
The supported image formats are bmp, png, jpg. Use the png format to get transparency.
For example, if the name of the skin is "Skin1", the name of the texture is "Tex00000", and the texture extension is ".png", then the texture filename can be "Tex00000_Skin1.png" or "Tex00000.png"
See "Skins\XWing\Default\Tex00033.png" for an example for the vanilla X-Wing.
To reduce the count of textures files, you can compress the directories of skins. If the directory is "FlightModels\Skins\[Model]\[Skin1]\", you can create "FlightModels\Skins\[Model]\[Skin1].zip" or "FlightModels\Skins\[Model]\[Skin1].7z". The textures are at the root of the arhive file.

Suppose that the mission is "[MissionDir]\[Mission].tie".
To define a skin for a craft, create a file named "[MissionDir]\[Mission]_Skins.txt" or create a section named "[Skins]" in "[MissionDir]\[Mission].ini".
The format is
CraftOptName = SkinNameA, SkinNameB
CraftOptName_fgc_# = SkinName1, SkinName2, SkinName3
# in CraftOptName_fgc_# is an integer for the opt color marking index, starting at 0.
The default SkinName is "Default".
See "Skins.txt"


*** Credits ***

- Jérémy Ansel (JeremyaFr)
