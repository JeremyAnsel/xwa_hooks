#pragma once
#include "hook_function.h"

#include "hangar.h"

static const HookFunction g_hookFunctions[] =
{
	{ 0x4579F3, HangarOptLoadHook },
	{ 0x455AAC, HangarOptReloadHook },
	{ 0x456AFD, HangarObjectCreateHook },
	{ 0x457C26, HangarReloadHook },
	{ 0x45FCA8, HangarCameraPositionHook },
	{ 0x4563DD, HangarLoadShuttleHook },
	{ 0x45C1AA, HangarShuttleUpdateHook },
	{ 0x4586E0, HangarShuttleReenterPositionHook },
	{ 0x45860A, HangarShuttleOptReadInfosHook },
	{ 0x456312, HangarLoadDroidsHook },
	{ 0x45643A, HangarLoadHangarRoofCraneHook },
	{ 0x455F4B, HangarMapHook },
	{ 0x456479, FamHangarMapHook },
	{ 0x57EB9D, SelectHangarTypeHook },
	{ 0x4095B5, SelectHangarInsideAnimation },
	{ 0x457F80, SelectHangarModelIndex },
	{ 0x46264F, CraftElevationHook },
	{ 0x455670, CraftSelectionMissionHook },
	{ 0x455741, CraftSelectionMeleeHook },
	{ 0x45B836, HangarLaunchAnimation1Hook },
	{ 0x45B8FF, HangarLaunchAnimation2Hook },
	{ 0x456F1B, HangarObjectsElevationHook },
	{ 0x455DBE, HangarFloorElevationHook },
	{ 0x455E28, HangarPositionXYHook },
	{ 0x45C9CA, HangarPlayerCraftElevationHook },
	{ 0x4582F4, HangarReenterInitPositionZHook },
	{ 0x45BB6E, HangarReenterAnimation51Hook },
	{ 0x45BBB2, HangarReenterAnimation52Hook },
	{ 0x45BBE9, HangarReenterAnimation53Hook },
	{ 0x45BCB5, HangarReenterAnimation5YCheckHook },
	{ 0x45BE29, HangarReenterAnimation6Hook },
	{ 0x45BF11, HangarReenterAnimation71Hook },
	{ 0x45BF76, HangarReenterAnimation72Hook },
	{ 0x45C012, HangarReenterAnimation73Hook },
	{ 0x45F19E, HangarShuttleLaunchReenterAnimations4And6Hook },
	{ 0x45F373, HangarShuttleLaunchReenterAnimations4And6Hook },
	{ 0x45F0D5, HangarShuttleLaunchReenterAnimation3CheckHook },
	{ 0x45ED42, HangarShuttleLaunchReenterAnimation1UpdateHook },
	{ 0x45EE64, HangarShuttleLaunchReenterAnimation1CheckHook },
	{ 0x45F6AC, HangarShuttleLaunchReenterAnimation7And10CheckHook },
	{ 0x45FB6F, HangarShuttleLaunchReenterAnimation7And10CheckHook },
	{ 0x45FBB6, HangarShuttleLaunchReenterAnimation7And10SetHook },
	{ 0x45F79F, HangarShuttleLaunchReenterAnimation9UpdateHook },
	{ 0x45F8A7, HangarShuttleLaunchReenterAnimation9CheckHook },
	{ 0x45EEA1, HangarShuttleLaunchReenterAnimation2UpdateHook },
	{ 0x45EF41, HangarShuttleLaunchReenterAnimation2CheckHook },
	{ 0x45F6D0, HangarShuttleLaunchReenterAnimation8UpdateHook },
	{ 0x45F776, HangarShuttleLaunchReenterAnimation8CheckHook },
	{ 0x45EDAD, HangarShuttleAnimationSound1Hook },
	{ 0x45F048, HangarShuttleAnimationSound3Hook },
	{ 0x45F5DE, HangarShuttleAnimationSound7Hook },
	{ 0x45F822, HangarShuttleAnimationSound9Hook },
	{ 0x45FAA5, HangarShuttleAnimationSound10Hook },
	{ 0x459CBF, HangarGetCraftIndexHook },
	{ 0x45AF1E, HangarDisableShadowWhenInvertedHook },
};

static const HookPatchItem g_selectHangarTypePatch[] =
{
	{ 0x17DF97, "390C858E2AAE000F92C2", "50E8939F02008AD05890" },
};

static const HookPatchItem g_selectInsideAnimationPatch[] =
{
	{ 0x89AF, "663D8700740A663D9400", "50E87BF5190085C05890" },
};

static const HookPatchItem g_selectHangarModelIndexPatch[] =
{
	{ 0x5737A, "663D8700740F663D9400", "50E8B00B150085C05890" },
};

static const HookPatchItem g_loadHangarOptPatch[] =
{
	{ 0x056DEE, "E84D4F0700", "E83D111500" },
};

static const HookPatchItem g_reloadHangarOptPatch[] =
{
	{ 0x054EA4, "750E50E8F4140000", "909050E874301500" },
};

static const HookPatchItem g_createHangarObjectPatch[] =
{
	{ 0x055EF7, "66391C7DE0A67C007522", "57E82320150083C40490" },
};

static const HookPatchItem g_reloadHangarPatch[] =
{
	{ 0x057021, "A10C337700", "E80A0F1500" },
};

static const HookPatchItem g_removeOptFilterPatch[] =
{
	{ 0x563C3, "83FB0E0F8FCB0000000F84", "33C089442410E9BF090000" },
};

static const HookPatchItem g_setHangarCameraPatch[] =
{
	{ 0x5F0A0, "81C76A04000081ED1009000081EE2C010000", "565557E8788E14005F5D5E90909090909090" },
	{ 0x5F7E8, "B7FC4500", "7EFC4500" },
	{ 0x5F7EC, "CFFE4500", "7EFC4500" },
	{ 0x5F7F0, "F0FC4500", "7EFC4500" },
	{ 0x5F7F4, "F7FC4500", "7EFC4500" },
	{ 0x5F7F8, "26FD4500", "7EFC4500" },
	{ 0x5F7FC, "5FFD4500", "7EFC4500" },
	{ 0x5F800, "08FF4500", "7EFC4500" },
	{ 0x5F804, "4DFF4500", "7EFC4500" },
	{ 0x5F86C, "08004600", "7EFC4500" },
	{ 0x5F870, "41004600", "7EFC4500" },
	{ 0x5F874, "7A004600", "7EFC4500" },
	{ 0x5F87C, "E2004600", "7EFC4500" },
	{ 0x5F880, "CD014600", "7EFC4500" },
	{ 0x5F884, "06024600", "7EFC4500" },
	{ 0x5F888, "3C024600", "7EFC4500" },
	{ 0x5F88C, "6F024600", "7EFC4500" },
};

static const HookPatchItem g_loadShuttlePatch[] =
{
	{ 0x557D8, "E803070000", "E853271500" },
};

static const HookPatchItem g_updateShuttlePatch[] =
{
	{ 0x5B5A5, "E8C62A0000", "E876C91400" },
};

static const HookPatchItem g_setShuttleCameraPatch[] =
{
	{ 0x57ADB, "8D1480C1E2032BD0A108BC6800", "E840041500E9B4000000909090" },
};

static const HookPatchItem g_readShuttleOptInfosPatch[] =
{
	{ 0x57A05, "E8A68EFDFFE85193FDFF", "E8160515009090909090" },
};

static const HookPatchItem g_loadDroidsPatch[] =
{
	{ 0x05570D, "A110BC6800536870E50000", "E80E281500E98600000090" },
	{ 0x0557A3, "898680679C00", "909090909090" },
	{ 0x0557AE, "899E84679C00", "909090909090" },
	{ 0x0557B9, "899EA6679C00", "909090909090" },
	{ 0x0557C4, "899EA2679C00", "909090909090" },
	{ 0x0557CC, "899E88679C00891510BC6800", "909090909090909090909090" },
};

static const HookPatchItem g_loadHangarRoofCranePatch[] =
{
	{ 0x055835, "E8A6060000", "E8F6261500" },
};

static const HookPatchItem g_setHangarMapPatch[] =
{
	{ 0x55345, "68C896000068FFFFFF7F68", "58E8D52B1500E9BD030000" },
};

static const HookPatchItem g_setFamilyHangarMapPatch[] =
{
	{ 0x55873, "688877000068FFFFFF7F68", "58E8A7261500E98E020000" },
};

static const HookPatchItem g_setCraftElevationPatch[] =
{
	{ 0x61A49, "83F804740F50E8CC310200", "50E8E164140083C404C390" },
	{ 0x55183, "83F804741C50E892FA0200", "50E8B7C8000083C404EB1B" },
	{ 0x562F4, "83F804741050E821E90200", "50E846B7000083C404EB0F" },
	{ 0x5AF8C, "83F804741650E8899C0200", "50E8AE6A000083C404EB15" },
	{ 0x5B203, "83F804741650E8129A0200", "50E83768000083C404EB15" },
	{ 0x5B350, "83F804741650E8C5980200", "50E8EA66000083C404EB15" },
	{ 0x5BDA4, "83F804741050E8718E0200", "50E8965C000083C404EB0F" },
};

static const HookPatchItem g_setCraftSelectionPatch[] =
{
	{ 0x54A69, "6683F93A0F84400100006683F941", "5150E8B03415005859E9EA010000" },
	{ 0x54B39, "80FA02BE02000000755A6683F901740B", "525150E8DF33150058595AE918010000" },
};

static const HookPatchItem g_hangarLaunchAnimationPatch[] =
{
	{ 0x05AC31, "03CB894C300F", "E8EAD2140090" },
	{ 0x05ACFA, "81C2F70000003BC20F8ED9070000", "E831D214009085C00F84D9070000" },
};

static const HookPatchItem g_hangarObjectsElevationPatch[] =
{
	{ 0x056314, "8B15C4337B0003C18944320F", "5055E8051C150083C4089090" },
	{ 0x0551B8, "2BF8893D38BC6800", "50E8622D15005890" },
	{ 0x055223, "66A154679C00", "E8082D150090" },
	{ 0x05BDC4, "8B15C4337B0003C18944320F", "50E856C11400589090909090" },
};

static const HookPatchItem g_hangarReenterAnimationPatch[] =
{
	{ 0x0576EF, "A138BC68008D8823020000", "E83C0815008BC890909090" },
	{ 0x05AF69, "81C2930000003954350F7E7A", "E8C2CF140090909085C0747A" },
	{ 0x05AFAC, "8B1538BC68008B7C350F03C23BF87E33", "50E87ECF14008B7C350F85C058907433" },
	{ 0x05AFE3, "2BF8897C350F8B2DC4337B00", "50E837CF1400589090909090" },
	{ 0x05B0B0, "05E0FCFFFF", "E87BCE1400" },
	{ 0x05B223, "8B1538BC68008B4C370F", "50E8F7CC140058EB1590" },
	{ 0x05B30C, "2BCA894C300F", "E80FCC140090" },
	{ 0x05B370, "8B1538BC680003C28B54310F3BD00F8F5F010000", "50E8BACB140083C40490909085C00F845F010000" },
	{ 0x05B40D, "E82E660000", "E81ECB1400" },
};

static const HookPatchItem g_hangarShuttleLaunchReenterAnimationsPatch[] =
{
	{ 0x05E598, "0FAFC301443507", "53E8829914005B" },
	{ 0x05E76D, "0FAFC301443507", "53E8AD9714005B" },
	{ 0x05E8E9, "D80DF8955A00", "D80DD0955A00" },
	{ 0x05E4CF, "A1C4337B008B4C010B", "51E85B9A1400599090" },
	{ 0x05E13C, "8D14098B0803CA8908", "51E8DE9D1400599090" },
	{ 0x05E25F, "81C2610100003954300F0F8E870D0000", "E8CC9C14009085C090900F85870D0000" },
	{ 0x05E923, "7E03", "EB0F" },
	{ 0x05EAA6, "8B540A0B81C2BF030000", "50E8749414005A909090" },
	{ 0x05EF69, "8B540A0B81C2BF030000", "50E8B18F14005A909090" },
	{ 0x05EFB1, "66C74430170000", "E86A8F14009090" },
	{ 0x05EB99, "F7D9D1E103D18910", "51E8819314005990" },
	{ 0x05ECA1, "8B1538BC680081C28A000000", "50E8899214008BD058909090" },
	{ 0x05ECAD, "3954300F0F8F3F030000", "85D290900F853F030000" },
	{ 0x05E29C, "D1E2660110", "E87F9C1400" },
	{ 0x05E33C, "A1C4337B0066817C3013204E0F83A80C0000", "E8EF9B140085C0A1C4337B000F85A80C0000" },
	{ 0x05EACB, "D1E1660108", "E850941400" },
	{ 0x05EB71, "66817C321380A80F8278040000", "E8BA93140085C00F8578040000" },
};

static const HookPatchItem g_hangarShuttleAnimationSoundsPatch[] =
{
	{ 0x05E1A8, "8B44300F2BC283E840", "E8839D140085C09090" },
	{ 0x05E443, "2BC20560090000", "E8E89A140085C0" },
	{ 0x05E9D9, "2BC20560090000", "E85295140085C0" },
	{ 0x05EC1D, "8B44300F2BC283E820", "E80E93140085C09090" },
	{ 0x05EEA0, "2BC20560090000", "E88B90140085C0" },
};

static const HookPatchItem g_hangarGetCraftIndexPatch[] =
{
	{ 0x0590BA, "E871310800", "E871EE1400" },
	{ 0x0590C9, "7467", "EB67" },
};

static const HookPatchItem g_hangarDisableShadowWhenInvertedPatch[] =
{
	{ 0x05A319, "E8D2870200", "E802DC1400" },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("To call the hook that selects the hangar type", g_selectHangarTypePatch),
	MAKE_HOOK_PATCH("To call the hook that selects the inside animation", g_selectInsideAnimationPatch),
	MAKE_HOOK_PATCH("To call the hook that selects the hangar model index", g_selectHangarModelIndexPatch),
	MAKE_HOOK_PATCH("To call the hook that loads hangar opt", g_loadHangarOptPatch),
	MAKE_HOOK_PATCH("To call the hook that reloads hangar opt", g_reloadHangarOptPatch),
	MAKE_HOOK_PATCH("To call the hook that creates hangar object", g_createHangarObjectPatch),
	MAKE_HOOK_PATCH("To call the hook that reloads the hangar", g_reloadHangarPatch),
	MAKE_HOOK_PATCH("To remove the opt filter", g_removeOptFilterPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the hangar camera", g_setHangarCameraPatch),
	MAKE_HOOK_PATCH("To call the hook that loads the shuttle", g_loadShuttlePatch),
	MAKE_HOOK_PATCH("To call the hook that updates the shuttle", g_updateShuttlePatch),
	MAKE_HOOK_PATCH("To call the hook that sets the shuttle camera", g_setShuttleCameraPatch),
	MAKE_HOOK_PATCH("To call the hook that reads the shuttle opt infos", g_readShuttleOptInfosPatch),
	MAKE_HOOK_PATCH("To call the hook that loads the droids", g_loadDroidsPatch),
	MAKE_HOOK_PATCH("To call the hook that loads the Hangar Roof Crane", g_loadHangarRoofCranePatch),
	MAKE_HOOK_PATCH("To call the hook that sets the hangar map", g_setHangarMapPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the family hangar map", g_setFamilyHangarMapPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the craft elevation", g_setCraftElevationPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the craft selection", g_setCraftSelectionPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the hangar launch animation", g_hangarLaunchAnimationPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the hangar objects elevation", g_hangarObjectsElevationPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the hangar re-enter animation", g_hangarReenterAnimationPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the shuttle launch and re-enter animations", g_hangarShuttleLaunchReenterAnimationsPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the shuttle animation sounds", g_hangarShuttleAnimationSoundsPatch),
	MAKE_HOOK_PATCH("To call the hook that gets the craft index", g_hangarGetCraftIndexPatch),
	MAKE_HOOK_PATCH("To call the hook that disable shadow when inverted", g_hangarDisableShadowWhenInvertedPatch),
};
