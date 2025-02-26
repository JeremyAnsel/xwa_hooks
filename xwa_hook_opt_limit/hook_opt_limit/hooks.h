#pragma once
#include "hook_function.h"

#include "opt_limit.h"

#include <string>
#include <sstream>
#include <iomanip>

std::string int_to_hex(int i)
{
	std::stringstream stream;
	stream << std::setfill('0') << std::hex
		<< std::setw(2) << (i & 0xff)
		<< std::setw(2) << ((i >> 8) & 0xff)
		<< std::setw(2) << ((i >> 16) & 0xff)
		<< std::setw(2) << ((i >> 24) & 0xff);
	return stream.str();
}

static const HookFunction g_hookFunctions[] =
{
	{ 0x528BD4, GenerateSpecRciHook },
	{ 0x5774CC, GenerateSpecRciProgressHook },
	{ 0x426A84, CraftInitHook },
	{ 0x50E9E3, Set0x07821E8Hook },
	{ 0x457A30, OptModelMeshesInfoHook1 },
	{ 0x48845D, OptModelMeshesInfoHook2 },
	{ 0x48897C, OptModelMeshesInfoHook3 },
	{ 0x488A14, OptModelMeshesInfoHook4 },
	{ 0x488B1D, OptModelMeshesInfoHook5 },
	{ 0x4316DA, CockpitOptInfoHook },
	{ 0x488AB3, CockpitOptInfoGetDescriptorHook },
	{ 0x431B05, ExteriorOptInfoHook },
	{ 0x488B06, ExteriorOptInfoGetDescriptorHook },
	{ 0x404C8A, EscapePodCraftInitHook },
	{ 0x514BB0, CurrentCraftInitHook },
	{ 0x41C82D, CurrentCraftInitHook },
	{ 0x455C5C, CurrentCraftInitHook },
	{ 0x456D02, CurrentCraftInitHook },
	{ 0x487521, CraftMeshesFilterHook },
	{ 0x487E8F, CraftMeshesFilterHook },
	{ 0x487F48, CraftMeshesFilterHook },
	{ 0x405BF8, CraftZeroMemoryHook },
	{ 0x41E85E, CraftZeroMemoryHook },
	{ 0x42D67A, ExeCraftEnginesArrayHook1 },
	{ 0x42DA33, ExeCraftEnginesArrayHook2 },
	{ 0x432B4E, ExeCraftEnginesArrayHook3 },
	{ 0x4E0E7F, ExeCraftEnginesArrayHook4 },
	{ 0x4EA010, ExeCraftEnginesArrayHook5 },
	{ 0x4F2BC9, ExeCraftEnginesArrayHook6 },
	{ 0x40B0EE, ExeCraftEngineMeshIdsArrayHook1 },
	{ 0x40B1C2, ExeCraftEngineMeshIdsArrayHook2 },
	{ 0x42D680, ExeCraftEngineMeshIdsArrayHook3 },
	{ 0x42DA39, ExeCraftEngineMeshIdsArrayHook4 },
	{ 0x432B65, ExeCraftEngineMeshIdsArrayHook5 },
	{ 0x47AC59, ExeCraftEngineMeshIdsArrayHook6 },
	{ 0x4E0E99, ExeCraftEngineMeshIdsArrayHook7 },
	{ 0x4E2CB1, ExeCraftEngineMeshIdsArrayHook8 },
	{ 0x4F2BDF, ExeCraftEngineMeshIdsArrayHook9 },
	{ 0x432B16, OptGetMeshEngineGlowCountExeCraftHook },
	{ 0x43293E, OptGetMeshEngineGlowCountSpaceBombHook },
	{ 0x4335E7, OptGetMeshEngineGlowCountCockpitHook },
	{ 0x4337BA, OptGetMeshEngineGlowCountExteriorHook },
	{ 0x4E2AC5, L004E28F0_GetMeshHook },
	{ 0x47AB16, L0047AAA0_SetCraftHook },
	{ 0x4E0E69, L004E0E10_GetEnginesCountHook },
	{ 0x4E1027, L004E0FA0_ComputePercentOfActiveEnginesHook },
	{ 0x4F2C2D, L004F22B0_EngineGlowIsDisabledHook },
	{ 0x42D889, L0042D590_EngineGlowIsDisabledHook },
	{ 0x42DBA1, L0042DB60_EngineGlowHook },
	{ 0x4825BE, SelectLodVersionHook },
	{ 0x4DF31C, MeshesCollisionHook },
	{ 0x4A2DE1, L004A2DD0Hook },
};

static const std::string g_hitDataArray0 = int_to_hex(GetHitDataArrayPtr());
static const std::string g_hitDataArray4 = int_to_hex(GetHitDataArrayPtr() + 4);
static const std::string g_hitDataArray8 = int_to_hex(GetHitDataArrayPtr() + 8);

static const std::string g_craftSize = int_to_hex(GetCraftSize());
static const std::string g_craftMulSizeEax = "69C0" + int_to_hex(GetCraftSize()) + "9090";
static const std::string g_craftMulSizeEdx = "69C0" + int_to_hex(GetCraftSize()) + "8BD0";
static const std::string g_craftOffset_22E__1 = int_to_hex(GetCraftOffset_22E() - 1);
static const std::string g_craftOffset_22E_0 = int_to_hex(GetCraftOffset_22E());
static const std::string g_craftOffset_22E_4 = int_to_hex(GetCraftOffset_22E() + 4);
static const std::string g_craftOffset_22E_49 = int_to_hex(GetCraftOffset_22E() + GetCraftMeshesCount() - 1);
static const std::string g_craftOffset_260_Neg = int_to_hex(-GetCraftOffset_260());
static const std::string g_craftOffset_260__1 = int_to_hex(GetCraftOffset_260() - 1);
static const std::string g_craftOffset_260_0 = int_to_hex(GetCraftOffset_260());
static const std::string g_craftOffset_260_1 = int_to_hex(GetCraftOffset_260() + 1);
static const std::string g_craftOffset_260_2 = int_to_hex(GetCraftOffset_260() + 2);
static const std::string g_craftOffset_260_3 = int_to_hex(GetCraftOffset_260() + 3);
static const std::string g_craftOffset_260_4 = int_to_hex(GetCraftOffset_260() + 4);
static const std::string g_craftOffset_260_6 = int_to_hex(GetCraftOffset_260() + 6);
static const std::string g_craftOffset_292__1 = int_to_hex(GetCraftOffset_292() - 1);
static const std::string g_craftOffset_292_0 = int_to_hex(GetCraftOffset_292());
static const std::string g_craftOffset_292_1 = int_to_hex(GetCraftOffset_292() + 1);
static const std::string g_craftOffset_292_2 = int_to_hex(GetCraftOffset_292() + 2);
static const std::string g_craftOffset_292_3 = int_to_hex(GetCraftOffset_292() + 3);
static const std::string g_craftOffset_292_4 = int_to_hex(GetCraftOffset_292() + 4);
static const std::string g_craftOffset_292_6 = int_to_hex(GetCraftOffset_292() + 6);
static const std::string g_craftOffset_292_10 = int_to_hex(GetCraftOffset_292() + 10);

static const std::string g_craftOffset_2CF_0 = int_to_hex(GetCraftOffset_2CF());

static const std::string g_spaceBombEnginesArray_0 = int_to_hex(GetSpaceBombEnginesArrayPtr());
static const std::string g_spaceBombEngineMeshIdsArray__1 = int_to_hex(GetSpaceBombEngineMeshIdsArrayPtr() - 1);
static const std::string g_spaceBombEngineMeshIdsArray_0 = int_to_hex(GetSpaceBombEngineMeshIdsArrayPtr());
static const std::string g_cockpitEnginesArray_0 = int_to_hex(GetCockpitEnginesArrayPtr());
static const std::string g_cockpitEngineMeshIdsArray__1 = int_to_hex(GetCockpitEngineMeshIdsArrayPtr() - 1);
static const std::string g_cockpitEngineMeshIdsArray_0 = int_to_hex(GetCockpitEngineMeshIdsArrayPtr());
static const std::string g_exteriorEnginesArray_0 = int_to_hex(GetExteriorEnginesArrayPtr());
static const std::string g_exteriorEngineMeshIdsArray__1 = int_to_hex(GetExteriorEngineMeshIdsArrayPtr() - 1);
static const std::string g_exteriorEngineMeshIdsArray_0 = int_to_hex(GetExteriorEngineMeshIdsArrayPtr());

static const std::string g_weaponRack_000002E4 = int_to_hex(GetCraftOffset_2DF() + 0x000002E4 - 0x2DF);
static const std::string g_weaponRack_000002E1 = int_to_hex(GetCraftOffset_2DF() + 0x000002E1 - 0x2DF);
static const std::string g_weaponRack_000002E9 = int_to_hex(GetCraftOffset_2DF() + 0x000002E9 - 0x2DF);
static const std::string g_weaponRack_000002E5 = int_to_hex(GetCraftOffset_2DF() + 0x000002E5 - 0x2DF);
static const std::string g_weaponRack_000002E2 = int_to_hex(GetCraftOffset_2DF() + 0x000002E2 - 0x2DF);
static const std::string g_weaponRack_000002E8 = int_to_hex(GetCraftOffset_2DF() + 0x000002E8 - 0x2DF);
static const std::string g_weaponRack_000002DF = int_to_hex(GetCraftOffset_2DF() + 0x000002DF - 0x2DF);
static const std::string g_weaponRack_000002E7 = int_to_hex(GetCraftOffset_2DF() + 0x000002E7 - 0x2DF);
static const std::string g_weaponRack_000002E3 = int_to_hex(GetCraftOffset_2DF() + 0x000002E3 - 0x2DF);
static const std::string g_weaponRack_000002F2 = int_to_hex(GetCraftOffset_2DF() + 0x000002F2 - 0x2DF);
static const std::string g_weaponRack_000002EB = int_to_hex(GetCraftOffset_2DF() + 0x000002EB - 0x2DF);
static const std::string g_weaponRack_000002D5 = int_to_hex(GetCraftOffset_2DF() + 0x000002D5 - 0x2DF);
static const std::string g_weaponRack_000002D6 = int_to_hex(GetCraftOffset_2DF() + 0x000002D6 - 0x2DF);
static const std::string g_weaponRack_000002D9 = int_to_hex(GetCraftOffset_2DF() + 0x000002D9 - 0x2DF);
static const std::string g_weaponRack_000002DA = int_to_hex(GetCraftOffset_2DF() + 0x000002DA - 0x2DF);
static const std::string g_weaponRack_000002D4 = int_to_hex(GetCraftOffset_2DF() + 0x000002D4 - 0x2DF);
static const std::string g_weaponRack_000002DB = int_to_hex(GetCraftOffset_2DF() + 0x000002DB - 0x2DF);
static const std::string g_weaponRack_000002D1 = int_to_hex(GetCraftOffset_2DF() + 0x000002D1 - 0x2DF);
static const std::string g_weaponRack_000002D3 = int_to_hex(GetCraftOffset_2DF() + 0x000002D3 - 0x2DF);

static const std::string g_weaponSlots_Offset1 = int_to_hex(GetWeaponSlotsOffset1());
static const std::string g_weaponSlots_Offset2 = int_to_hex(GetWeaponSlotsOffset2());
static const std::string g_weaponSlots_005BB5E2 = int_to_hex(GetWeaponSlotsOffset1() + 0);
static const std::string g_weaponSlots_005BB5E4 = int_to_hex(GetWeaponSlotsOffset1() + 4);
static const std::string g_weaponSlots_005BB5E6 = int_to_hex(GetWeaponSlotsOffset2() + 0);
static const std::string g_weaponSlots_005BB5E8 = int_to_hex(GetWeaponSlotsOffset2() + 4);
static const std::string g_weaponSlots_005BB5E9 = int_to_hex(GetWeaponSlotsOffset2() + 5);

static const HookPatchItem g_trianglingPatch[] =
{
	{ 0x193C4A, "750C", "9090" },
	{ 0x193C52, "0002", "0080" },
};

static const HookPatchItem g_briefingWireframeOverlayPatch[] =
{
	{ 0x07E09C, "7D", "EB" },
	{ 0x07E12E, "7D", "EB" },
	{ 0x07E1B9, "47", "90" },
};

static const HookPatchItem g_disableHitzoneBoundsCheckOnRenderPatch[] =
{
	{ 0x081A38, "7562", "EB62" },
};

static const HookPatchItem g_texturesSizePatch[] =
{
	{ 0x19564A, "81790C000100007717", "909090909090909090" },
	{ 0x1956C2, "817A10000100007717", "909090909090909090" },
};

static const HookPatchItem g_texturesCountPatch[] =
{
	{ 0x0CC432, "2003", "0010" },
	{ 0x0CC43A, "2803", "0810" },
	{ 0x0CC445, "2C03", "0C10" },
	{ 0x0CC44C, "2003", "0010" },
	{ 0x0CC466, "C800", "0004" },
	{ 0x0CC497, "3003", "1010" },
	{ 0x0CC4C9, "3003", "1010" },
	{ 0x0CC4E6, "3003", "1010" },
	{ 0x0CC4EE, "2003", "0010" },
};

static const HookPatchItem g_optLimitPatch[] =
{
	{ 0x193BFE, "01", "20" },
	{ 0x0E7616, "68D37600", g_hitDataArray8.c_str() },
	{ 0x0E7888, "68D37600", g_hitDataArray8.c_str() },
	{ 0x0E7898, "68D37600", g_hitDataArray8.c_str() },
	{ 0x0E78A6, "68D37600", g_hitDataArray8.c_str() },
	{ 0x0E78B9, "68D37600", g_hitDataArray8.c_str() },
	{ 0x0E78DE, "60D37600", g_hitDataArray0.c_str() },
	{ 0x0E78F5, "60D37600", g_hitDataArray0.c_str() },
	{ 0x0E790C, "60D37600", g_hitDataArray0.c_str() },
	{ 0x0E792C, "60D37600", g_hitDataArray0.c_str() },
	{ 0x0E79DA, "60D37600", g_hitDataArray0.c_str() },
	{ 0x0E79E1, "64D37600", g_hitDataArray4.c_str() },
	{ 0x0E7A02, "60D37600", g_hitDataArray0.c_str() },
	{ 0x0E7A08, "64D37600", g_hitDataArray4.c_str() },
};

static const HookPatchItem g_glowMarkUVArrayPatch[] =
{
	{ 0x0E758F, "A148D37600", "A1066D9B00" },
	{ 0x0E75B8, "A148D37600", "A1066D9B00" },
};

static const HookPatchItem g_generateSpecRciPatch[] =
{
	{ 0x127FCF, "E87C0D0000", "E84CFF0700" },
	{ 0x1768C7, "A12CD2AB00", "E864160300" },
};

static const HookPatchItem g_fixDepthBufferBugPatch[] =
{
	{ 0x0415C0, "8B442404A3B4465B00C390", "C705B4465B0000008042C3" },
	{ 0x0415D0, "C705B4465B0000000045C3", "C705B4465B0000008042C3" },
};

static const HookPatchItem g_craftSizePatch[] =
{
	{ 0x015046 + 2, "F9030000", g_craftSize.c_str() },
	{ 0x0F4A7C + 2, "F9030000", g_craftSize.c_str() },
	{ 0x0F4E04 + 2, "F9030000", g_craftSize.c_str() },
	{ 0x0F55CB + 2, "F9030000", g_craftSize.c_str() },
	{ 0x0F5704 + 2, "F9030000", g_craftSize.c_str() },
	{ 0x0F570E + 2, "F9030000", g_craftSize.c_str() },
	{ 0x0F5725 + 2, "F9030000", g_craftSize.c_str() },
	{ 0x0F5738 + 2, "F9030000", g_craftSize.c_str() },
	{ 0x0F575B + 2, "F9030000", g_craftSize.c_str() },
	{ 0x0F46F5, "C1E2072BD08D04D0", g_craftMulSizeEax.c_str() },
	{ 0x0F6205, "C1E2072BD08D04D0", g_craftMulSizeEax.c_str() },
	{ 0x0F66D0, "C1E2072BD08D04D0", g_craftMulSizeEax.c_str() },
	{ 0x014E64, "C1E1072BC88D14C8", g_craftMulSizeEdx.c_str() },
	{ 0x01E22F + 1, "FE000000", g_craftSize.c_str() },
	{ 0x01E234, "F3A5A4", "F3A490" },
	{ 0x025DC4 + 1, "FE000000", g_craftSize.c_str() },
	{ 0x025DCD, "F3ABAA", "F3AA90" },
	{ 0x025E7F, "E85C56FEFF", "E89C201800" },
	{ 0x0F4A75 + 1, "FE000000", g_craftSize.c_str() },
	{ 0x0F4A88, "F3A5A4", "F3A490" },
	{ 0x0F4DF4 + 1, "FE000000", g_craftSize.c_str() },
	{ 0x0F4DFB, "F3A5A4", "F3A490" },
	{ 0x10DDDE, "C70505277800E8217800", "E83DA109009090909090" },
};

static const HookPatchItem g_craftOffsetsPatch[] =
{
	// :0045EEDD B9A0FDFFFF              mov ecx, FFFFFDA0 // -0x260
	{ 0x0005E2DE, "A0FDFFFF", g_craftOffset_260_Neg.c_str() },
	// :0045EEE2 8D9F60020000            lea ebx, dword ptr [edi+00000260]
	{ 0x0005E2E4, "60020000", g_craftOffset_260_0.c_str() },
	// :0045F1D9 B9A0FDFFFF              mov ecx, FFFFFDA0 // -0x260
	{ 0x0005E5DA, "A0FDFFFF", g_craftOffset_260_Neg.c_str() },
	// :0045F1DE 8DAF60020000            lea ebp, dword ptr [edi+00000260]
	{ 0x0005E5E0, "60020000", g_craftOffset_260_0.c_str() },
	// :0045F41A B9A0FDFFFF              mov ecx, FFFFFDA0 // -0x260
	{ 0x0005E81B, "A0FDFFFF", g_craftOffset_260_Neg.c_str() },
	// :0045F41F 8DAF60020000            lea ebp, dword ptr [edi+00000260]
	{ 0x0005E821, "60020000", g_craftOffset_260_0.c_str() },
	// :0045F70D B9A0FDFFFF              mov ecx, FFFFFDA0 // -0x260
	{ 0x0005EB0E, "A0FDFFFF", g_craftOffset_260_Neg.c_str() },
	// :0045F712 8DAF60020000            lea ebp, dword ptr [edi+00000260]
	{ 0x0005EB14, "60020000", g_craftOffset_260_0.c_str() },
	// :0045F9DA BDA0FDFFFF              mov ebp, FFFFFDA0 // -0x260
	{ 0x0005EDDB, "A0FDFFFF", g_craftOffset_260_Neg.c_str() },
	// :0045F9DF 8D9F60020000            lea ebx, dword ptr [edi+00000260]
	{ 0x0005EDE1, "60020000", g_craftOffset_260_0.c_str() },

	// :0040328D 8DB42A60020000          lea esi, dword ptr [edx+ebp+00000260]
	{ 0x00002690, "60020000", g_craftOffset_260_0.c_str() },
	// :004032E7 8DB42960020000          lea esi, dword ptr [ecx+ebp+00000260]
	{ 0x000026EA, "60020000", g_craftOffset_260_0.c_str() },
	// :0040335A 8DB562020000            lea esi, dword ptr [ebp+00000262]
	{ 0x0000275C, "62020000", g_craftOffset_260_2.c_str() },
	// :005029DC 8D9892020000            lea ebx, dword ptr [eax+00000292]
	{ 0x00101DDE, "92020000", g_craftOffset_292_0.c_str() },
	// :00519726 8D8663020000            lea eax, dword ptr [esi+00000263]
	{ 0x00118B28, "63020000", g_craftOffset_260_3.c_str() },
	// :00519E98 8D840F60020000          lea eax, dword ptr [edi+ecx+00000260]
	{ 0x0011929B, "60020000", g_craftOffset_260_0.c_str() },
	// :00519EF9 8DBC1760020000          lea edi, dword ptr [edi+edx+00000260]
	{ 0x001192FC, "60020000", g_craftOffset_260_0.c_str() },
	// :0051A02B 8DBC0F60020000          lea edi, dword ptr [edi+ecx+00000260]
	{ 0x0011942E, "60020000", g_craftOffset_260_0.c_str() },
	// :0051A10D 8DBC0760020000          lea edi, dword ptr [edi+eax+00000260]
	{ 0x00119510, "60020000", g_craftOffset_260_0.c_str() },
	// :0051A3F6 8D840760020000          lea eax, dword ptr [edi+eax+00000260]
	{ 0x001197F9, "60020000", g_craftOffset_260_0.c_str() },

	// :00518662 81C161020000            add ecx, 00000261
	{ 0x00117A64, "61020000", g_craftOffset_260_1.c_str() },
	// :00518749 81C161020000            add ecx, 00000261
	{ 0x00117B4B, "61020000", g_craftOffset_260_1.c_str() },
	// :00519758 81C663020000            add esi, 00000263
	{ 0x00118B5A, "63020000", g_craftOffset_260_3.c_str() },

	// :00401604 8A8C0592020000          mov cl, byte ptr [ebp+eax+00000292]
	{ 0x00000A07, "92020000", g_craftOffset_292_0.c_str() },
	// :00401660 8A8196020000            mov al, byte ptr [ecx+00000296]
	{ 0x00000A62, "96020000", g_craftOffset_292_4.c_str() },
	// :0040174E 889C3260020000          mov byte ptr [edx+esi+00000260], bl
	{ 0x00000B51, "60020000", g_craftOffset_260_0.c_str() },
	// :00401766 C684316002000020        mov byte ptr [ecx+esi+00000260], 20
	{ 0x00000B69, "60020000", g_craftOffset_260_0.c_str() },
	// :00401792 C68432600200000C        mov byte ptr [edx+esi+00000260], 0C
	{ 0x00000B95, "60020000", g_craftOffset_260_0.c_str() },
	// :004017A1 C684306002000008        mov byte ptr [eax+esi+00000260], 08
	{ 0x00000BA4, "60020000", g_craftOffset_260_0.c_str() },
	// :004017B7 889C3160020000          mov byte ptr [ecx+esi+00000260], bl
	{ 0x00000BBA, "60020000", g_craftOffset_260_0.c_str() },
	// :004017CC C684326002000060        mov byte ptr [edx+esi+00000260], 60
	{ 0x00000BCF, "60020000", g_craftOffset_260_0.c_str() },
	// :004017E1 889C3060020000          mov byte ptr [eax+esi+00000260], bl
	{ 0x00000BE4, "60020000", g_craftOffset_260_0.c_str() },
	// :004017F6 C684316002000042        mov byte ptr [ecx+esi+00000260], 42
	{ 0x00000BF9, "60020000", g_craftOffset_260_0.c_str() },
	// :00401977 8A943160020000          mov dl, byte ptr [ecx+esi+00000260]
	{ 0x00000D7A, "60020000", g_craftOffset_260_0.c_str() },
	// :00401988 88943160020000          mov byte ptr [ecx+esi+00000260], dl
	{ 0x00000D8B, "60020000", g_craftOffset_260_0.c_str() },
	// :00401998 88943160020000          mov byte ptr [ecx+esi+00000260], dl
	{ 0x00000D9B, "60020000", g_craftOffset_260_0.c_str() },
	// :004019A5 80BC316002000080        cmp byte ptr [ecx+esi+00000260], 80
	{ 0x00000DA8, "60020000", g_craftOffset_260_0.c_str() },
	// :004019AF C684316002000000        mov byte ptr [ecx+esi+00000260], 00
	{ 0x00000DB2, "60020000", g_craftOffset_260_0.c_str() },
	// :004019D3 8A943160020000          mov dl, byte ptr [ecx+esi+00000260]
	{ 0x00000DD6, "60020000", g_craftOffset_260_0.c_str() },
	// :004019EB 88943160020000          mov byte ptr [ecx+esi+00000260], dl
	{ 0x00000DEE, "60020000", g_craftOffset_260_0.c_str() },
	// :00401A30 8A843160020000          mov al, byte ptr [ecx+esi+00000260]
	{ 0x00000E33, "60020000", g_craftOffset_260_0.c_str() },
	// :00401A52 88843160020000          mov byte ptr [ecx+esi+00000260], al
	{ 0x00000E55, "60020000", g_craftOffset_260_0.c_str() },
	// :00401E25 8A843192020000          mov al, byte ptr [ecx+esi+00000292]
	{ 0x00001228, "92020000", g_craftOffset_292_0.c_str() },
	// :0040214A 888C0260020000          mov byte ptr [edx+eax+00000260], cl
	{ 0x0000154D, "60020000", g_craftOffset_260_0.c_str() },
	// :00402153 8A843160020000          mov al, byte ptr [ecx+esi+00000260]
	{ 0x00001556, "60020000", g_craftOffset_260_0.c_str() },
	// :00402164 88843160020000          mov byte ptr [ecx+esi+00000260], al
	{ 0x00001567, "60020000", g_craftOffset_260_0.c_str() },
	// :0040217B 80B4306002000001        xor byte ptr [eax+esi+00000260], 01
	{ 0x0000157E, "60020000", g_craftOffset_260_0.c_str() },
	// :004021F0 660FB6915F020000        movzx dx, byte ptr [ecx+0000025F]
	{ 0x000015F4, "5F020000", g_craftOffset_22E_49.c_str() },
	// :004022CC 88815F020000            mov byte ptr [ecx+0000025F], al
	{ 0x000016CE, "5F020000", g_craftOffset_22E_49.c_str() },
	// :00402315 80BC322E02000004        cmp byte ptr [edx+esi+0000022E], 04
	{ 0x00001718, "2E020000", g_craftOffset_22E_0.c_str() },
	// :004023AE 8A843160020000          mov al, byte ptr [ecx+esi+00000260]
	{ 0x000017B1, "60020000", g_craftOffset_260_0.c_str() },
	// :004023BF 88843160020000          mov byte ptr [ecx+esi+00000260], al
	{ 0x000017C2, "60020000", g_craftOffset_260_0.c_str() },
	// :004023D6 80B4306002000001        xor byte ptr [eax+esi+00000260], 01
	{ 0x000017D9, "60020000", g_craftOffset_260_0.c_str() },
	// :00402C03 8A843160020000          mov al, byte ptr [ecx+esi+00000260]
	{ 0x00002006, "60020000", g_craftOffset_260_0.c_str() },
	// :00402C1C 8A843160020000          mov al, byte ptr [ecx+esi+00000260]
	{ 0x0000201F, "60020000", g_craftOffset_260_0.c_str() },
	// :00402C39 8A843160020000          mov al, byte ptr [ecx+esi+00000260]
	{ 0x0000203C, "60020000", g_craftOffset_260_0.c_str() },
	// :00402C52 8A843160020000          mov al, byte ptr [ecx+esi+00000260]
	{ 0x00002055, "60020000", g_craftOffset_260_0.c_str() },
	// :00402C81 8A843160020000          mov al, byte ptr [ecx+esi+00000260]
	{ 0x00002084, "60020000", g_craftOffset_260_0.c_str() },
	// :00402C92 88843160020000          mov byte ptr [ecx+esi+00000260], al
	{ 0x00002095, "60020000", g_craftOffset_260_0.c_str() },
	// :00403279 C684296002000000        mov byte ptr [ecx+ebp+00000260], 00
	{ 0x0000267C, "60020000", g_craftOffset_260_0.c_str() },
	// :00403286 8A842A60020000          mov al, byte ptr [edx+ebp+00000260]
	{ 0x00002689, "60020000", g_craftOffset_260_0.c_str() },
	// :004032D3 C684286002000000        mov byte ptr [eax+ebp+00000260], 00
	{ 0x000026D6, "60020000", g_craftOffset_260_0.c_str() },
	// :004032E0 8A842960020000          mov al, byte ptr [ecx+ebp+00000260]
	{ 0x000026E3, "60020000", g_craftOffset_260_0.c_str() },
	// :00404CE1 888C3E92020000          mov byte ptr [esi+edi+00000292], cl
	{ 0x000040E4, "92020000", g_craftOffset_292_0.c_str() },
	// :0040FD61 8A843391020000          mov al, byte ptr [ebx+esi+00000291]
	{ 0x0000F164, "91020000", g_craftOffset_292_1.c_str() },
	// :00411CCD C6825F02000001          mov byte ptr [edx+0000025F], 01
	{ 0x000110CF, "5F020000", g_craftOffset_22E_49.c_str() },
	// :00411CD9 C6805F02000001          mov byte ptr [eax+0000025F], 01
	{ 0x000110DB, "5F020000", g_craftOffset_22E_49.c_str() },
	// :00411CE5 C6815F02000001          mov byte ptr [ecx+0000025F], 01
	{ 0x000110E7, "5F020000", g_craftOffset_22E_49.c_str() },
	// :0041C8AA 88843192020000          mov byte ptr [ecx+esi+00000292], al
	{ 0x0001BCAD, "92020000", g_craftOffset_292_0.c_str() },
	// :0041C8BE 88943092020000          mov byte ptr [eax+esi+00000292], dl
	{ 0x0001BCC1, "92020000", g_craftOffset_292_0.c_str() },
	// :0041C8F7 889C3192020000          mov byte ptr [ecx+esi+00000292], bl
	{ 0x0001BCFA, "92020000", g_craftOffset_292_0.c_str() },
	// :0041C904 C684322E02000004        mov byte ptr [edx+esi+0000022E], 04
	{ 0x0001BD07, "2E020000", g_craftOffset_22E_0.c_str() },
	// :0041C9BA 889C0692020000          mov byte ptr [esi+eax+00000292], bl
	{ 0x0001BDBD, "92020000", g_craftOffset_292_0.c_str() },
	// :0041C9C7 C684062E02000004        mov byte ptr [esi+eax+0000022E], 04
	{ 0x0001BDCA, "2E020000", g_craftOffset_22E_0.c_str() },
	// :0041DBDF 8A84292E020000          mov al, byte ptr [ecx+ebp+0000022E]
	{ 0x0001CFE2, "2E020000", g_craftOffset_22E_0.c_str() },
	// :0041DE93 C684282E02000004        mov byte ptr [eax+ebp+0000022E], 04
	{ 0x0001D296, "2E020000", g_craftOffset_22E_0.c_str() },
	// :0041DED3 C684082E02000004        mov byte ptr [eax+ecx+0000022E], 04
	{ 0x0001D2D6, "2E020000", g_craftOffset_22E_0.c_str() },
	// :004244FB 8A8093020000            mov al, byte ptr [eax+00000293]
	{ 0x000238FD, "93020000", g_craftOffset_292_1.c_str() },
	// :00424590 8A8593020000            mov al, byte ptr [ebp+00000293]
	{ 0x00023992, "93020000", g_craftOffset_292_1.c_str() },
	// :004245E3 8A8593020000            mov al, byte ptr [ebp+00000293]
	{ 0x000239E5, "93020000", g_craftOffset_292_1.c_str() },
	// :00424660 8A8594020000            mov al, byte ptr [ebp+00000294]
	{ 0x00023A62, "94020000", g_craftOffset_292_2.c_str() },
	// :0042BA3B 8A8C062E020000          mov cl, byte ptr [esi+eax+0000022E]
	{ 0x0002AE3E, "2E020000", g_craftOffset_22E_0.c_str() },
	// :0042BA4A 660FB6B05F020000        movzx si, byte ptr [eax+0000025F]
	{ 0x0002AE4E, "5F020000", g_craftOffset_22E_49.c_str() },
	// :0042DBCB 660FB69C0260020000      movzx bx, byte ptr [edx+eax+00000260]
	{ 0x0002CFD0, "60020000", g_craftOffset_260_0.c_str() },
	// :0045880F 889C2E60020000          mov byte ptr [esi+ebp+00000260], bl
	{ 0x00057C12, "60020000", g_craftOffset_260_0.c_str() },
	// :0045E19B 8A9161020000            mov dl, byte ptr [ecx+00000261]
	{ 0x0005D59D, "61020000", g_craftOffset_260_1.c_str() },
	// :0045E1A3 889161020000            mov byte ptr [ecx+00000261], dl
	{ 0x0005D5A5, "61020000", g_craftOffset_260_1.c_str() },
	// :0045E1B5 8A8862020000            mov cl, byte ptr [eax+00000262]
	{ 0x0005D5B7, "62020000", g_craftOffset_260_2.c_str() },
	// :0045E1BF 888862020000            mov byte ptr [eax+00000262], cl
	{ 0x0005D5C1, "62020000", g_craftOffset_260_2.c_str() },
	// :0045E1CD 888862020000            mov byte ptr [eax+00000262], cl
	{ 0x0005D5CF, "62020000", g_craftOffset_260_2.c_str() },
	// :0045E1F5 8A8163020000            mov al, byte ptr [ecx+00000263]
	{ 0x0005D5F7, "63020000", g_craftOffset_260_3.c_str() },
	// :0045E201 888163020000            mov byte ptr [ecx+00000263], al
	{ 0x0005D603, "63020000", g_craftOffset_260_3.c_str() },
	// :0045E20D 80BA63020000F7          cmp byte ptr [edx+00000263], F7
	{ 0x0005D60F, "63020000", g_craftOffset_260_3.c_str() },
	// :0045E226 80806302000002          add byte ptr [eax+00000263], 02
	{ 0x0005D628, "63020000", g_craftOffset_260_3.c_str() },
	// :0045E232 80B863020000E1          cmp byte ptr [eax+00000263], E1
	{ 0x0005D634, "63020000", g_craftOffset_260_3.c_str() },
	// :0045E23B C68063020000E0          mov byte ptr [eax+00000263], E0
	{ 0x0005D63D, "63020000", g_craftOffset_260_3.c_str() },
	// :0045E246 888163020000            mov byte ptr [ecx+00000263], al
	{ 0x0005D648, "63020000", g_craftOffset_260_3.c_str() },
	// :0045E251 80B86302000004          cmp byte ptr [eax+00000263], 04
	{ 0x0005D653, "63020000", g_craftOffset_260_3.c_str() },
	// :0045E26A 808063020000FE          add byte ptr [eax+00000263], FE
	{ 0x0005D66C, "63020000", g_craftOffset_260_3.c_str() },
	// :0045E276 80B86302000020          cmp byte ptr [eax+00000263], 20
	{ 0x0005D678, "63020000", g_craftOffset_260_3.c_str() },
	// :0045E27F C6806302000021          mov byte ptr [eax+00000263], 21
	{ 0x0005D681, "63020000", g_craftOffset_260_3.c_str() },
	// :0045E2BB 8A8160020000            mov al, byte ptr [ecx+00000260]
	{ 0x0005D6BD, "60020000", g_craftOffset_260_0.c_str() },
	// :0045E2D4 888160020000            mov byte ptr [ecx+00000260], al
	{ 0x0005D6D6, "60020000", g_craftOffset_260_0.c_str() },
	// :0045E2F7 80B06002000001          xor byte ptr [eax+00000260], 01
	{ 0x0005D6F9, "60020000", g_craftOffset_260_0.c_str() },
	// :0045EA25 8A8863020000            mov cl, byte ptr [eax+00000263]
	{ 0x0005DE27, "63020000", g_craftOffset_260_3.c_str() },
	// :0045EA2D 888863020000            mov byte ptr [eax+00000263], cl
	{ 0x0005DE2F, "63020000", g_craftOffset_260_3.c_str() },
	// :0045EA40 8A8860020000            mov cl, byte ptr [eax+00000260]
	{ 0x0005DE42, "60020000", g_craftOffset_260_0.c_str() },
	// :0045EA4F 888860020000            mov byte ptr [eax+00000260], cl
	{ 0x0005DE51, "60020000", g_craftOffset_260_0.c_str() },
	// :0045EA63 8A8164020000            mov al, byte ptr [ecx+00000264]
	{ 0x0005DE65, "64020000", g_craftOffset_260_4.c_str() },
	// :0045EA7D 888164020000            mov byte ptr [ecx+00000264], al
	{ 0x0005DE7F, "64020000", g_craftOffset_260_4.c_str() },
	// :0045EA89 80B964020000F7          cmp byte ptr [ecx+00000264], F7
	{ 0x0005DE8B, "64020000", g_craftOffset_260_4.c_str() },
	// :0045EAA5 80806402000002          add byte ptr [eax+00000264], 02
	{ 0x0005DEA7, "64020000", g_craftOffset_260_4.c_str() },
	// :0045EAB2 80B964020000E1          cmp byte ptr [ecx+00000264], E1
	{ 0x0005DEB4, "64020000", g_craftOffset_260_4.c_str() },
	// :0045EABB C68164020000E0          mov byte ptr [ecx+00000264], E0
	{ 0x0005DEBD, "64020000", g_craftOffset_260_4.c_str() },
	// :0045EAC6 888164020000            mov byte ptr [ecx+00000264], al
	{ 0x0005DEC8, "64020000", g_craftOffset_260_4.c_str() },
	// :0045EAD2 80B96402000004          cmp byte ptr [ecx+00000264], 04
	{ 0x0005DED4, "64020000", g_craftOffset_260_4.c_str() },
	// :0045EAEE 808064020000FE          add byte ptr [eax+00000264], FE
	{ 0x0005DEF0, "64020000", g_craftOffset_260_4.c_str() },
	// :0045EAFB 80B96402000020          cmp byte ptr [ecx+00000264], 20
	{ 0x0005DEFD, "64020000", g_craftOffset_260_4.c_str() },
	// :0045EB04 C6816402000021          mov byte ptr [ecx+00000264], 21
	{ 0x0005DF06, "64020000", g_craftOffset_260_4.c_str() },
	// :0045EB11 8A8166020000            mov al, byte ptr [ecx+00000266]
	{ 0x0005DF13, "66020000", g_craftOffset_260_6.c_str() },
	// :0045EB1D 888166020000            mov byte ptr [ecx+00000266], al
	{ 0x0005DF1F, "66020000", g_craftOffset_260_6.c_str() },
	// :0045EB28 80B866020000F7          cmp byte ptr [eax+00000266], F7
	{ 0x0005DF2A, "66020000", g_craftOffset_260_6.c_str() },
	// :0045EB44 80806602000002          add byte ptr [eax+00000266], 02
	{ 0x0005DF46, "66020000", g_craftOffset_260_6.c_str() },
	// :0045EB50 80B866020000E1          cmp byte ptr [eax+00000266], E1
	{ 0x0005DF52, "66020000", g_craftOffset_260_6.c_str() },
	// :0045EB59 C68066020000E0          mov byte ptr [eax+00000266], E0
	{ 0x0005DF5B, "66020000", g_craftOffset_260_6.c_str() },
	// :0045EB64 888166020000            mov byte ptr [ecx+00000266], al
	{ 0x0005DF66, "66020000", g_craftOffset_260_6.c_str() },
	// :0045EB70 80B96602000004          cmp byte ptr [ecx+00000266], 04
	{ 0x0005DF72, "66020000", g_craftOffset_260_6.c_str() },
	// :0045EB8C 808066020000FE          add byte ptr [eax+00000266], FE
	{ 0x0005DF8E, "66020000", g_craftOffset_260_6.c_str() },
	// :0045EB98 80B86602000020          cmp byte ptr [eax+00000266], 20
	{ 0x0005DF9A, "66020000", g_craftOffset_260_6.c_str() },
	// :0045EBA1 C6806602000021          mov byte ptr [eax+00000266], 21
	{ 0x0005DFA3, "66020000", g_craftOffset_260_6.c_str() },
	// :0045EC14 8A8160020000            mov al, byte ptr [ecx+00000260]
	{ 0x0005E016, "60020000", g_craftOffset_260_0.c_str() },
	// :0045EC2D 888160020000            mov byte ptr [ecx+00000260], al
	{ 0x0005E02F, "60020000", g_craftOffset_260_0.c_str() },
	// :0045EC50 80B06002000001          xor byte ptr [eax+00000260], 01
	{ 0x0005E052, "60020000", g_craftOffset_260_0.c_str() },
	// :004626AC 889C022D020000          mov byte ptr [edx+eax+0000022D], bl
	{ 0x00061AAF, "2D020000", g_craftOffset_22E__1.c_str() },
	// :004626B9 889C025F020000          mov byte ptr [edx+eax+0000025F], bl
	{ 0x00061ABC, "5F020000", g_craftOffset_260__1.c_str() },
	// :00477315 8A842A92020000          mov al, byte ptr [edx+ebp+00000292]
	{ 0x00076718, "92020000", g_craftOffset_292_0.c_str() },
	// :0047ACF2 8A941992020000          mov dl, byte ptr [ecx+ebx+00000292]
	{ 0x0007A0F5, "92020000", g_craftOffset_292_0.c_str() },
	// :00481178 8A8C072D020000          mov cl, byte ptr [edi+eax+0000022D]
	{ 0x0008057B, "2D020000", g_craftOffset_22E__1.c_str() },
	// :004811BF 80BC0A6002000000        cmp byte ptr [edx+ecx+00000260], 00
	{ 0x000805C2, "60020000", g_craftOffset_260_0.c_str() },
	// :004811EF 8A8C0260020000          mov cl, byte ptr [edx+eax+00000260]
	{ 0x000805F2, "60020000", g_craftOffset_260_0.c_str() },
	// :004813CD 8A94395F020000          mov dl, byte ptr [ecx+edi+0000025F]
	{ 0x000807D0, "5F020000", g_craftOffset_260__1.c_str() },
	// :0048195B 8A94012D020000          mov dl, byte ptr [ecx+eax+0000022D]
	{ 0x00080D5E, "2D020000", g_craftOffset_22E__1.c_str() },
	// :00481971 8A94015F020000          mov dl, byte ptr [ecx+eax+0000025F]
	{ 0x00080D74, "5F020000", g_craftOffset_260__1.c_str() },
	// :004819AC 80BC086002000000        cmp byte ptr [eax+ecx+00000260], 00
	{ 0x00080DAF, "60020000", g_craftOffset_260_0.c_str() },
	// :004819D9 8A8C1060020000          mov cl, byte ptr [eax+edx+00000260]
	{ 0x00080DDC, "60020000", g_craftOffset_260_0.c_str() },
	// :00483C37 8A8C022D020000          mov cl, byte ptr [edx+eax+0000022D]
	{ 0x0008303A, "2D020000", g_craftOffset_22E__1.c_str() },
	// :00483C7B 80BC086002000000        cmp byte ptr [eax+ecx+00000260], 00
	{ 0x0008307E, "60020000", g_craftOffset_260_0.c_str() },
	// :00483CA8 8A8C1060020000          mov cl, byte ptr [eax+edx+00000260]
	{ 0x000830AB, "60020000", g_craftOffset_260_0.c_str() },
	// :00483E2E 8A840A5F020000          mov al, byte ptr [edx+ecx+0000025F]
	{ 0x00083231, "5F020000", g_craftOffset_260__1.c_str() },
	// :004879EF 80BC109202000000        cmp byte ptr [eax+edx+00000292], 00
	{ 0x00086DF2, "92020000", g_craftOffset_292_0.c_str() },
	// :0048807D 8A9C1792020000          mov bl, byte ptr [edi+edx+00000292]
	{ 0x00087480, "92020000", g_craftOffset_292_0.c_str() },
	// :0048810D 8A9C3892020000          mov bl, byte ptr [eax+edi+00000292]
	{ 0x00087510, "92020000", g_craftOffset_292_0.c_str() },
	// :00490E2B 8A8C0692020000          mov cl, byte ptr [esi+eax+00000292]
	{ 0x0009022E, "92020000", g_craftOffset_292_0.c_str() },
	// :00496AAD 80BC029202000000        cmp byte ptr [edx+eax+00000292], 00
	{ 0x00095EB0, "92020000", g_craftOffset_292_0.c_str() },
	// :004A7E92 389C0A92020000          cmp byte ptr [edx+ecx+00000292], bl
	{ 0x000A7295, "92020000", g_craftOffset_292_0.c_str() },
	// :004A85E1 8A840A2E020000          mov al, byte ptr [edx+ecx+0000022E]
	{ 0x000A79E4, "2E020000", g_craftOffset_22E_0.c_str() },
	// :004BDD7D 8A940192020000          mov dl, byte ptr [ecx+eax+00000292]
	{ 0x000BD180, "92020000", g_craftOffset_292_0.c_str() },
	// :004DF299 80B89602000000          cmp byte ptr [eax+00000296], 00
	{ 0x000DE69B, "96020000", g_craftOffset_292_4.c_str() },
	// :004DF34C 8A8C0691020000          mov cl, byte ptr [esi+eax+00000291]
	{ 0x000DE74F, "91020000", g_craftOffset_292__1.c_str() },
	// :004DF363 8A94065F020000          mov dl, byte ptr [esi+eax+0000025F]
	{ 0x000DE766, "5F020000", g_craftOffset_260__1.c_str() },
	// :004E0125 8A842992020000          mov al, byte ptr [ecx+ebp+00000292]
	{ 0x000DF528, "92020000", g_craftOffset_292_0.c_str() },
	// :004E0283 8A840E92020000          mov al, byte ptr [esi+ecx+00000292]
	{ 0x000DF686, "92020000", g_craftOffset_292_0.c_str() },
	// :004E0304 8A9C1192020000          mov bl, byte ptr [ecx+edx+00000292]
	{ 0x000DF707, "92020000", g_craftOffset_292_0.c_str() },
	// :004E0359 8A9C1192020000          mov bl, byte ptr [ecx+edx+00000292]
	{ 0x000DF75C, "92020000", g_craftOffset_292_0.c_str() },
	// :004E0381 8A842992020000          mov al, byte ptr [ecx+ebp+00000292]
	{ 0x000DF784, "92020000", g_craftOffset_292_0.c_str() },
	// :004E0396 88842992020000          mov byte ptr [ecx+ebp+00000292], al
	{ 0x000DF799, "92020000", g_craftOffset_292_0.c_str() },
	// :004E03AB 8A842992020000          mov al, byte ptr [ecx+ebp+00000292]
	{ 0x000DF7AE, "92020000", g_craftOffset_292_0.c_str() },
	// :004E042F 8A841692020000          mov al, byte ptr [esi+edx+00000292]
	{ 0x000DF832, "92020000", g_craftOffset_292_0.c_str() },
	// :004E049D 8A8C2892020000          mov cl, byte ptr [eax+ebp+00000292]
	{ 0x000DF8A0, "92020000", g_craftOffset_292_0.c_str() },
	// :004E04E5 C6842A2E02000004        mov byte ptr [edx+ebp+0000022E], 04
	{ 0x000DF8E8, "2E020000", g_craftOffset_22E_0.c_str() },
	// :004E056A 8A8C022E020000          mov cl, byte ptr [edx+eax+0000022E]
	{ 0x000DF96D, "2E020000", g_craftOffset_22E_0.c_str() },
	// :004E05E3 8A94012E020000          mov dl, byte ptr [ecx+eax+0000022E]
	{ 0x000DF9E6, "2E020000", g_craftOffset_22E_0.c_str() },
	// :004E125E 8A9C1192020000          mov bl, byte ptr [ecx+edx+00000292]
	{ 0x000E0661, "92020000", g_craftOffset_292_0.c_str() },
	// :004E1839 80BC019202000000        cmp byte ptr [ecx+eax+00000292], 00
	{ 0x000E0C3C, "92020000", g_craftOffset_292_0.c_str() },
	// :004E2005 8AB40160020000          mov dh, byte ptr [ecx+eax+00000260]
	{ 0x000E1408, "60020000", g_craftOffset_260_0.c_str() },
	// :004E347A 8A940191020000          mov dl, byte ptr [ecx+eax+00000291]
	{ 0x000E287D, "91020000", g_craftOffset_292__1.c_str() },
	// :004E348B 8A94015F020000          mov dl, byte ptr [ecx+eax+0000025F]
	{ 0x000E288E, "5F020000", g_craftOffset_260__1.c_str() },
	// :004E90B3 8A940191020000          mov dl, byte ptr [ecx+eax+00000291]
	{ 0x000E84B6, "91020000", g_craftOffset_292__1.c_str() },
	// :004E90CB 8A94015F020000          mov dl, byte ptr [ecx+eax+0000025F]
	{ 0x000E84CE, "5F020000", g_craftOffset_260__1.c_str() },
	// :004F2CB1 8A9C2860020000          mov bl, byte ptr [eax+ebp+00000260]
	{ 0x000F20B4, "60020000", g_craftOffset_260_0.c_str() },
	// :004F3C65 8A8896020000            mov cl, byte ptr [eax+00000296]
	{ 0x000F3067, "96020000", g_craftOffset_292_4.c_str() },
	// :004F4739 8A843192020000          mov al, byte ptr [ecx+esi+00000292]
	{ 0x000F3B3C, "92020000", g_craftOffset_292_0.c_str() },
	// :00500D74 8A9C0A2E020000          mov bl, byte ptr [edx+ecx+0000022E]
	{ 0x00100177, "2E020000", g_craftOffset_22E_0.c_str() },
	// :00500E4E 8A8C1092020000          mov cl, byte ptr [eax+edx+00000292]
	{ 0x00100251, "92020000", g_craftOffset_292_0.c_str() },
	// :00500F10 8A9C0A2E020000          mov bl, byte ptr [edx+ecx+0000022E]
	{ 0x00100313, "2E020000", g_craftOffset_22E_0.c_str() },
	// :00500FEA 8A8C1092020000          mov cl, byte ptr [eax+edx+00000292]
	{ 0x001003ED, "92020000", g_craftOffset_292_0.c_str() },
	// :005040AB 8A8296020000            mov al, byte ptr [edx+00000296]
	{ 0x001034AD, "96020000", g_craftOffset_292_4.c_str() },
	// :005040C1 8A8196020000            mov al, byte ptr [ecx+00000296]
	{ 0x001034C3, "96020000", g_craftOffset_292_4.c_str() },
	// :00509138 8A8661020000            mov al, byte ptr [esi+00000261]
	{ 0x0010853A, "61020000", g_craftOffset_260_1.c_str() },
	// :00509158 8A9663020000            mov dl, byte ptr [esi+00000263]
	{ 0x0010855A, "63020000", g_craftOffset_260_3.c_str() },
	// :0050917D 8A8662020000            mov al, byte ptr [esi+00000262]
	{ 0x0010857F, "62020000", g_craftOffset_260_2.c_str() },
	// :0050919B 8A8664020000            mov al, byte ptr [esi+00000264]
	{ 0x0010859D, "64020000", g_craftOffset_260_4.c_str() },
	// :005091A6 8A8E66020000            mov cl, byte ptr [esi+00000266]
	{ 0x001085A8, "66020000", g_craftOffset_260_6.c_str() },
	// :00513FF8 C6806402000020          mov byte ptr [eax+00000264], 20
	{ 0x001133FA, "64020000", g_craftOffset_260_4.c_str() },
	// :00514C08 C684069202000002        mov byte ptr [esi+eax+00000292], 02
	{ 0x0011400B, "92020000", g_craftOffset_292_0.c_str() },
	// :00514C16 889C0E2E020000          mov byte ptr [esi+ecx+0000022E], bl
	{ 0x00114019, "2E020000", g_craftOffset_22E_0.c_str() },
	// :00514C25 C684169202000014        mov byte ptr [esi+edx+00000292], 14
	{ 0x00114028, "92020000", g_craftOffset_292_0.c_str() },
	// :00514C32 889C062E020000          mov byte ptr [esi+eax+0000022E], bl
	{ 0x00114035, "2E020000", g_craftOffset_22E_0.c_str() },
	// :00516159 8A8896020000            mov cl, byte ptr [eax+00000296]
	{ 0x0011555B, "96020000", g_craftOffset_292_4.c_str() },
	// :00518695 8A8262020000            mov al, byte ptr [edx+00000262]
	{ 0x00117A97, "62020000", g_craftOffset_260_2.c_str() },
	// :005187FB 8A8162020000            mov al, byte ptr [ecx+00000262]
	{ 0x00117BFD, "62020000", g_craftOffset_260_2.c_str() },
	// :005197A8 8A8194020000            mov al, byte ptr [ecx+00000294]
	{ 0x00118BAA, "94020000", g_craftOffset_292_2.c_str() },
	// :005197B8 8A819C020000            mov al, byte ptr [ecx+0000029C]
	{ 0x00118BBA, "9C020000", g_craftOffset_292_10.c_str() },
	// :00519840 8AAC0260020000          mov ch, byte ptr [edx+eax+00000260]
	{ 0x00118C43, "60020000", g_craftOffset_260_0.c_str() },
	// :00519E9F 8A8C0F60020000          mov cl, byte ptr [edi+ecx+00000260]
	{ 0x001192A2, "60020000", g_craftOffset_260_0.c_str() },
	// :0051A3EF 8A8C0760020000          mov cl, byte ptr [edi+eax+00000260]
	{ 0x001197F2, "60020000", g_craftOffset_260_0.c_str() },
	// :0051A4EB 8A9096020000            mov dl, byte ptr [eax+00000296]
	{ 0x001198ED, "96020000", g_craftOffset_292_4.c_str() },
	// :0051A512 C6809602000002          mov byte ptr [eax+00000296], 02
	{ 0x00119914, "96020000", g_craftOffset_292_4.c_str() },
	// :0051A521 888A32020000            mov byte ptr [edx+00000232], cl
	{ 0x00119923, "32020000", g_craftOffset_22E_4.c_str() },
	// :0051A6C7 8A9096020000            mov dl, byte ptr [eax+00000296]
	{ 0x00119AC9, "96020000", g_craftOffset_292_4.c_str() },
	// :0051A6F2 C6809602000002          mov byte ptr [eax+00000296], 02
	{ 0x00119AF4, "96020000", g_craftOffset_292_4.c_str() },
	// :0051A701 888A32020000            mov byte ptr [edx+00000232], cl
	{ 0x00119B03, "32020000", g_craftOffset_22E_4.c_str() },
	// :0051A741 8A9096020000            mov dl, byte ptr [eax+00000296]
	{ 0x00119B43, "96020000", g_craftOffset_292_4.c_str() },
	// :0051A766 C6809602000002          mov byte ptr [eax+00000296], 02
	{ 0x00119B68, "96020000", g_craftOffset_292_4.c_str() },
	// :0051A775 C6813202000000          mov byte ptr [ecx+00000232], 00
	{ 0x00119B77, "32020000", g_craftOffset_22E_4.c_str() },
};

static const HookPatchItem g_meshesCountPatch[] =
{
	{ 0x084605, "7E05", "EB05" },
	{ 0x08784B, "7E05", "EB05" },
	{ 0x087DEF, "7E05", "EB05" },
	{ 0x10DF5E, "7E0C", "EB0C" },
};

static const HookPatchItem g_meshesInfoPatch[] =
{
	{ 0x056E2A, "53E860D702008BE88D049B", "53E8F010150083C404EB50" },
	{ 0x087856, "33F685DB899834FFFFFF7E2A", "5355E8C306120083C408EB2A" },
	{ 0x087D76, "8B048D60978D00", "53E8B40112005B" },
	{ 0x087E0F, "85D27D0333C0", "E81C01120090" },
	{ 0x087F16, "85D27C318BC1C1E00403C18D0440", "5251E81300120083C4085F5EC390" },
};

static const HookPatchItem g_cockpitOptInfoPatch[] =
{
	{ 0x030AD5, "85DB7E36BF6C089100", "E846741700EB339090" },
	{ 0x087EAD, "8B048D6C089100", "51E87D00120059" },
};

static const HookPatchItem g_exteriorOptInfoPatch[] =
{
	{ 0x030F00, "85ED7E36BF2C828000", "E81B701700EB339090" },
	{ 0x087F00, "8B048D2C828000", "51E82A00120059" },
};

static const HookPatchItem g_escapePodCraftInitPatch[] =
{
	{ 0x004084, "8D8660020000B932000000", "56E8963E1A005EEB0F9090" },
};

static const HookPatchItem g_currentCraftInitPatch[] =
{
	{ 0x113FAB, "8B15FC0D910040", "E8703F0900EB25" },
	{ 0x01BC28, "8B15FC0D910040", "E8F3C21800EB25" },
	{ 0x055057, "8B0DFC0D910040", "E8C42E1500EB27" },
	{ 0x0560FD, "889C082E020000", "E81E1E1500EB20" },
};

static const HookPatchItem g_craftMeshesFilterPatch[] =
{
	{ 0x08691A, "83E07F80BC109202000000", "5052E8FF1512005A5885C0" },
	{ 0x087288, "83E07F80BC389202000000", "5057E8910C12005F5885C0" },
	{ 0x087341, "83E07F80BC189202000000", "5053E8D80B12005B5885C0" },
};

static const HookPatchItem g_craftZeroMemoryPatch[] =
{
	{ 0x004FF2, "8BBADD000000F3ABAA", "52E8282F1A0083C404" },
	{ 0x01DC58, "B9EA00000033C0F3ABAA", "51E8C2A2180083C40490" },
};

static const HookPatchItem g_craftEnginesPatch[] =
{
	{ 0x01C106 + 3, "CF020000", g_craftOffset_2CF_0.c_str() },
	{ 0x07A08C + 3, "CF020000", g_craftOffset_2CF_0.c_str() },
	{ 0x0BA0FB + 3, "CF020000", g_craftOffset_2CF_0.c_str() },
	{ 0x0E0285 + 3, "CF020000", g_craftOffset_2CF_0.c_str() },
	{ 0x0E03B7 + 3, "CF020000", g_craftOffset_2CF_0.c_str() },
	{ 0x0E0417 + 3, "CF020000", g_craftOffset_2CF_0.c_str() },
	{ 0x0E20CF + 3, "CF020000", g_craftOffset_2CF_0.c_str() },
	{ 0x02CA75, "8DB862B65B00", "E8A6B4170090" },
	{ 0x02CE2E, "8DB862B65B00", "E8EDB0170090" },
	{ 0x031F49, "89849662B65B00", "E8D25F17009090" },
	{ 0x0E027A, "8D9F62B65B00", "E8A17C0C0090" },
	{ 0x0E940B, "8B9C9A62B65B00", "E810EB0B009090" },
	{ 0x0F1FC4, "8D8562B65B00", "E8575F0B0090" },
	{ 0x00A4E9, "660FB6BC01A2B65B00", "E832DA190090909090"},
	{ 0x00A5BD, "660FB6BC01A2B65B00", "E85ED9190090909090"},
	{ 0x02CA7B, "8D80A2B65B00", "E8A0B4170090"},
	{ 0x02CE34, "8D98A2B65B00", "E8E7B0170090"},
	{ 0x031F60, "88940FA1B65B00", "E8BB5F17009090"},
	{ 0x07A054, "660FB6AC01A2B65B00", "E8C7DE120090909090"},
	{ 0x0E0294, "660FB68C3AA2B65B00", "E8877C0C0090909090"},
	{ 0x0E20AC, "8A9401A2B65B00", "E86F5E0C009090"},
	{ 0x0F1FDA, "8A8429A2B65B00", "E8415F0B009090"},
	{ 0x02CA11 + 1, "60578D00", g_spaceBombEnginesArray_0.c_str() },
	{ 0x031D71 + 3, "60578D00", g_spaceBombEnginesArray_0.c_str() },
	{ 0x02CA16 + 4, "A0578D00", g_spaceBombEngineMeshIdsArray_0.c_str() },
	{ 0x031D86 + 3, "9F578D00", g_spaceBombEngineMeshIdsArray__1.c_str() },
	{ 0x02CACE + 1, "80FB7F00", g_cockpitEnginesArray_0.c_str() },
	{ 0x032A1A + 3, "80FB7F00", g_cockpitEnginesArray_0.c_str() },
	{ 0x02CAD3 + 4, "C0FB7F00", g_cockpitEngineMeshIdsArray_0.c_str() },
	{ 0x032A2D + 3, "BFFB7F00", g_cockpitEngineMeshIdsArray__1.c_str() },
	{ 0x02CAEA + 1, "60099100", g_exteriorEnginesArray_0.c_str() },
	{ 0x032BE9 + 3, "60099100", g_exteriorEnginesArray_0.c_str() },
	{ 0x02CAEF + 4, "A0099100", g_exteriorEngineMeshIdsArray_0.c_str() },
	{ 0x032BF9 + 3, "9F099100", g_exteriorEngineMeshIdsArray__1.c_str() },
	{ 0x031F11, "E86A5B0500", "E81A601700" },
	{ 0x031D39, "E8425D0500", "E8F2611700" },
	{ 0x0329E2, "E899500500", "E849551700" },
	{ 0x032BB5, "E8C64E0500", "E876531700" },
};

static const HookPatchItem g_craftEngineMeshPatch[] =
{
	{ 0x0E1EC0, "E88B5FFAFF", "E86B600C00" },
	{ 0x079F11, "8B9ADD000000", "E80AE0120090" },
	{ 0x0E0264, "660FB687B2B65B00", "E8B77C0C00909090" },
	{ 0x0E0422, "403BC172F0", "E8F97A0C00" },
	{ 0x0F2028, "8B81DD000000", "E8F35E0B0090" },
	{ 0x02CC84, "0F8598000000", "E897B2170090" },
	{ 0x02CF9C, "0F84CD070000", "E87FAF170090" },
};

static const HookPatchItem g_selectLodVersionPatch[] =
{
	{ 0x0819B9, "A1C4277800", "E862651200" },
};

static const HookPatchItem g_computeChecksumPatch[] =
{
	{ 0x0F5010, "83EC14A178AE910053C1F8045589442410A148099100568B3570AE9100575033DB68BCFA5F006800000200895C241C", "33EDBA100000008D3CAD201277002BD533C0C1E2028BCA8D34ADD80E7700C1E902F3AB8BCA8BFEC1E902F3AB33C0C3" },
};

static const HookPatchItem g_meshesCollisionPatch[] =
{
	{ 0x0DE717, "E83497FAFF", "E814980C00" },
};

static const HookPatchItem g_weaponRacksOffsetsPatch[] =
{
	// :004DA5C0
	// :004DA5C0 8A8C5EE4020000          mov cl, byte ptr [esi+2*ebx+000002E4]
	{ 0x000D99C3, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004DA5DB
	// :004DA5DB 8A8C6EE4020000          mov cl, byte ptr [esi+2*ebp+000002E4]
	{ 0x000D99DE, "E4020000", g_weaponRack_000002E4.c_str() },
	// :00401DF0
	// :00401DF0 80BC19E102000004        cmp byte ptr [ecx+ebx+000002E1], 04
	{ 0x000011F3, "E1020000", g_weaponRack_000002E1.c_str() },
	// :00401E50
	// :00401E50 6681BC19E9020000FFFF    cmp word ptr [ecx+ebx+000002E9], FFFF
	{ 0x00001254, "E9020000", g_weaponRack_000002E9.c_str() },
	// :00401E7F
	// :00401E7F 668B8419E9020000        mov ax, word ptr [ecx+ebx+000002E9]
	{ 0x00001283, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004E17B4
	// :004E17B4 66C78450E9020000FFFF    mov word ptr [eax+2*edx+000002E9], FFFF
	{ 0x000E0BB8, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004E1880
	// :004E1880 66019448E5020000        add word ptr [eax+2*ecx+000002E5], dx
	{ 0x000E0C84, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004E1888
	// :004E1888 8D8448E5020000          lea eax, dword ptr [eax+2*ecx+000002E5]
	{ 0x000E0C8B, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004E18A5
	// :004E18A5 66FF8470E5020000        inc word ptr [eax+2*esi+000002E5]
	{ 0x000E0CA9, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004E18AD
	// :004E18AD 8D8470E5020000          lea eax, dword ptr [eax+2*esi+000002E5]
	{ 0x000E0CB0, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004E18DB
	// :004E18DB 66019448E5020000        add word ptr [eax+2*ecx+000002E5], dx
	{ 0x000E0CDF, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004E18E3
	// :004E18E3 8D8448E5020000          lea eax, dword ptr [eax+2*ecx+000002E5]
	{ 0x000E0CE6, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004E1900
	// :004E1900 66838470E502000002      add word ptr [eax+2*esi+000002E5], 0002
	{ 0x000E0D04, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004E1909
	// :004E1909 8D8470E5020000          lea eax, dword ptr [eax+2*esi+000002E5]
	{ 0x000E0D0C, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004E192C
	// :004E192C 6683BC30E502000000      cmp word ptr [eax+esi+000002E5], 0000
	{ 0x000E0D30, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004E19B6
	// :004E19B6 8A9430E2020000          mov dl, byte ptr [eax+esi+000002E2]
	{ 0x000E0DB9, "E2020000", g_weaponRack_000002E2.c_str() },
	// :004E19D0
	// :004E19D0 8A9C30E1020000          mov bl, byte ptr [eax+esi+000002E1]
	{ 0x000E0DD3, "E1020000", g_weaponRack_000002E1.c_str() },
	// :004E1A16
	// :004E1A16 66899C30E5020000        mov word ptr [eax+esi+000002E5], bx
	{ 0x000E0E1A, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004E1BD9
	// :004E1BD9 8D8AE8020000            lea ecx, dword ptr [edx+000002E8]
	{ 0x000E0FDB, "E8020000", g_weaponRack_000002E8.c_str() },
	// :004E1CCB
	// :004E1CCB 81C1E8020000            add ecx, 000002E8
	{ 0x000E10CD, "E8020000", g_weaponRack_000002E8.c_str() },
	// :004E2107
	// :004E2107 8A8C10E2020000          mov cl, byte ptr [eax+edx+000002E2]
	{ 0x000E150A, "E2020000", g_weaponRack_000002E2.c_str() },
	// :004E2122
	// :004E2122 8A9408E1020000          mov dl, byte ptr [eax+ecx+000002E1]
	{ 0x000E1525, "E1020000", g_weaponRack_000002E1.c_str() },
	// :004E2168
	// :004E2168 80BC31E102000005        cmp byte ptr [ecx+esi+000002E1], 05
	{ 0x000E156B, "E1020000", g_weaponRack_000002E1.c_str() },
	// :004E2218
	// :004E2218 8A9430E4020000          mov dl, byte ptr [eax+esi+000002E4]
	{ 0x000E161B, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004E2283
	// :004E2283 668BB410DF020000        mov si, word ptr [eax+edx+000002DF]
	{ 0x000E1687, "DF020000", g_weaponRack_000002DF.c_str() },
	// :004E28BE
	// :004E28BE 888C02E7020000          mov byte ptr [edx+eax+000002E7], cl
	{ 0x000E1CC1, "E7020000", g_weaponRack_000002E7.c_str() },
	// :004E28CE
	// :004E28CE 888C02E8020000          mov byte ptr [edx+eax+000002E8], cl
	{ 0x000E1CD1, "E8020000", g_weaponRack_000002E8.c_str() },
	// :004A1E68
	// :004A1E68 8D85E9020000            lea eax, dword ptr [ebp+000002E9]
	{ 0x000A126A, "E9020000", g_weaponRack_000002E9.c_str() },
	// :0050630F
	// :0050630F 8D8FE9020000            lea ecx, dword ptr [edi+000002E9]
	{ 0x00105711, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004FBD27
	// :004FBD27 8D83E3020000            lea eax, dword ptr [ebx+000002E3]
	{ 0x000FB129, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004FBD6B
	// :004FBD6B 8A8453E3020000          mov al, byte ptr [ebx+2*edx+000002E3]
	{ 0x000FB16E, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004FBD7D
	// :004FBD7D 80B9E102000004          cmp byte ptr [ecx+000002E1], 04
	{ 0x000FB17F, "E1020000", g_weaponRack_000002E1.c_str() },
	// :004FBD8C
	// :004FBD8C 8881E3020000            mov byte ptr [ecx+000002E3], al
	{ 0x000FB18E, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004FC5DB
	// :004FC5DB 8A90E4020000            mov dl, byte ptr [eax+000002E4]
	{ 0x000FB9DD, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004FC5E1
	// :004FC5E1 8D88F2020000            lea ecx, dword ptr [eax+000002F2]
	{ 0x000FB9E3, "F2020000", g_weaponRack_000002F2.c_str() },
	// :004FC5E7
	// :004FC5E7 05E4020000              add eax, 000002E4
	{ 0x000FB9E8, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004FC614
	// :004FC614 8A90E4020000            mov dl, byte ptr [eax+000002E4]
	{ 0x000FBA16, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004FC61A
	// :004FC61A 8D88F2020000            lea ecx, dword ptr [eax+000002F2]
	{ 0x000FBA1C, "F2020000", g_weaponRack_000002F2.c_str() },
	// :004FC620
	// :004FC620 05E4020000              add eax, 000002E4
	{ 0x000FBA21, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004FCBB7
	// :004FCBB7 8A9453E1020000          mov dl, byte ptr [ebx+2*edx+000002E1]
	{ 0x000FBFBA, "E1020000", g_weaponRack_000002E1.c_str() },
	// :004FCBC3
	// :004FCBC3 66C780E9020000FFFF      mov word ptr [ebx+000002E9], FFFF
	{ 0x000FBFC6, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004FCD8F
	// :004FCD8F 8D8BE3020000            lea ecx, dword ptr [ebx+000002E3]
	{ 0x000FC191, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004FCEC9
	// :004FCEC9 8A8C43E3020000          mov cl, byte ptr [ebx+2*eax+000002E3]
	{ 0x000FC2CC, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004FCED8
	// :004FCED8 80B8E102000004          cmp byte ptr [eax+000002E1], 04
	{ 0x000FC2DA, "E1020000", g_weaponRack_000002E1.c_str() },
	// :004FCEE3
	// :004FCEE3 8888E3020000            mov byte ptr [eax+000002E3], cl
	{ 0x000FC2E5, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004FD350
	// :004FD350 8A844BE3020000          mov al, byte ptr [ebx+2*ecx+000002E3]
	{ 0x000FC753, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004FD362
	// :004FD362 80B9E102000004          cmp byte ptr [ecx+000002E1], 04
	{ 0x000FC764, "E1020000", g_weaponRack_000002E1.c_str() },
	// :004FD371
	// :004FD371 8881E3020000            mov byte ptr [ecx+000002E3], al
	{ 0x000FC773, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004FF771
	// :004FF771 8D87E9020000            lea eax, dword ptr [edi+000002E9]
	{ 0x000FEB73, "E9020000", g_weaponRack_000002E9.c_str() },
	// :005000C3
	// :005000C3 80BC08E102000004        cmp byte ptr [eax+ecx+000002E1], 04
	{ 0x000FF4C6, "E1020000", g_weaponRack_000002E1.c_str() },
	// :005000CD
	// :005000CD 66899408E9020000        mov word ptr [eax+ecx+000002E9], dx
	{ 0x000FF4D1, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004D7CC1
	// :004D7CC1 660FB69C5EE4020000      movzx bx, byte ptr [esi+2*ebx+000002E4]
	{ 0x000D70C6, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004D7CD5
	// :004D7CD5 660FB68C6EE4020000      movzx cx, byte ptr [esi+2*ebp+000002E4]
	{ 0x000D70DA, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004BAFC4
	// :004BAFC4 8A845AE4020000          mov al, byte ptr [edx+2*ebx+000002E4]
	{ 0x000BA3C7, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004F695F
	// :004F695F 668B9408EB020000        mov dx, word ptr [eax+ecx+000002EB]
	{ 0x000F5D63, "EB020000", g_weaponRack_000002EB.c_str() },
	// :004F6973
	// :004F6973 66899408EB020000        mov word ptr [eax+ecx+000002EB], dx
	{ 0x000F5D77, "EB020000", g_weaponRack_000002EB.c_str() },
	// :004F6980
	// :004F6980 668B9408E5020000        mov dx, word ptr [eax+ecx+000002E5]
	{ 0x000F5D84, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004F6994
	// :004F6994 66899408E5020000        mov word ptr [eax+ecx+000002E5], dx
	{ 0x000F5D98, "E5020000", g_weaponRack_000002E5.c_str() },
	// :0043E196
	// :0043E196 80BC5EE402000000        cmp byte ptr [esi+2*ebx+000002E4], 00
	{ 0x0003D599, "E4020000", g_weaponRack_000002E4.c_str() },
	// :0043E329
	// :0043E329 80BC56E402000000        cmp byte ptr [esi+2*edx+000002E4], 00
	{ 0x0003D72C, "E4020000", g_weaponRack_000002E4.c_str() },
	// :0046D49F
	// :0046D49F 8DAFE3020000            lea ebp, dword ptr [edi+000002E3]
	{ 0x0006C8A1, "E3020000", g_weaponRack_000002E3.c_str() },
	// :0046A8BE
	// :0046A8BE 8A847EE1020000          mov al, byte ptr [esi+2*edi+000002E1]
	{ 0x00069CC1, "E1020000", g_weaponRack_000002E1.c_str() },
	// :00507C96
	// :00507C96 81C6E9020000            add esi, 000002E9
	{ 0x00107098, "E9020000", g_weaponRack_000002E9.c_str() },
	// :0040F4A8
	// :0040F4A8 8A844FE4020000          mov al, byte ptr [edi+2*ecx+000002E4]
	{ 0x0000E8AB, "E4020000", g_weaponRack_000002E4.c_str() },
	// :0040F4BF
	// :0040F4BF 8A845FE4020000          mov al, byte ptr [edi+2*ebx+000002E4]
	{ 0x0000E8C2, "E4020000", g_weaponRack_000002E4.c_str() },
	// :00410950
	// :00410950 8D83E9020000            lea eax, dword ptr [ebx+000002E9]
	{ 0x0000FD52, "E9020000", g_weaponRack_000002E9.c_str() },
	// :00501F94
	// :00501F94 8A9C5DE1020000          mov bl, byte ptr [ebp+2*ebx+000002E1]
	{ 0x00101397, "E1020000", g_weaponRack_000002E1.c_str() },
	// :00501F9F
	// :00501F9F 66C780E9020000FFFF      mov word ptr [ebx+000002E9], FFFF
	{ 0x001013A2, "E9020000", g_weaponRack_000002E9.c_str() },
	// :00491998
	// :00491998 8A8C50E4020000          mov cl, byte ptr [eax+2*edx+000002E4]
	{ 0x00090D9B, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004919F0
	// :004919F0 8A8C50E4020000          mov cl, byte ptr [eax+2*edx+000002E4]
	{ 0x00090DF3, "E4020000", g_weaponRack_000002E4.c_str() },
	// :00491A4A
	// :00491A4A 8A9448E4020000          mov dl, byte ptr [eax+2*ecx+000002E4]
	{ 0x00090E4D, "E4020000", g_weaponRack_000002E4.c_str() },
	// :00491A97
	// :00491A97 8A9448E4020000          mov dl, byte ptr [eax+2*ecx+000002E4]
	{ 0x00090E9A, "E4020000", g_weaponRack_000002E4.c_str() },
	// :0041AC92
	// :0041AC92 81C7DF020000            add edi, 000002DF
	{ 0x0001A094, "DF020000", g_weaponRack_000002DF.c_str() },
	// :0041B949
	// :0041B949 66898C07DF020000        mov word ptr [edi+eax+000002DF], cx
	{ 0x0001AD4D, "DF020000", g_weaponRack_000002DF.c_str() },
	// :0041B957
	// :0041B957 C68401E102000004        mov byte ptr [ecx+eax+000002E1], 04
	{ 0x0001AD5A, "E1020000", g_weaponRack_000002E1.c_str() },
	// :0041B965
	// :0041B965 66898C07DF020000        mov word ptr [edi+eax+000002DF], cx
	{ 0x0001AD69, "DF020000", g_weaponRack_000002DF.c_str() },
	// :0041B976
	// :0041B976 888C07E1020000          mov byte ptr [edi+eax+000002E1], cl
	{ 0x0001AD79, "E1020000", g_weaponRack_000002E1.c_str() },
	// :0041B986
	// :0041B986 C68401D50200007F        mov byte ptr [ecx+eax+000002D5], 7F
	{ 0x0001AD89, "D5020000", g_weaponRack_000002D5.c_str() },
	// :0041B994
	// :0041B994 889C01D6020000          mov byte ptr [ecx+eax+000002D6], bl
	{ 0x0001AD97, "D6020000", g_weaponRack_000002D6.c_str() },
	// :0041B9A1
	// :0041B9A1 C68401D9020000FF        mov byte ptr [ecx+eax+000002D9], FF
	{ 0x0001ADA4, "D9020000", g_weaponRack_000002D9.c_str() },
	// :0041B9AF
	// :0041B9AF C68401DA020000FF        mov byte ptr [ecx+eax+000002DA], FF
	{ 0x0001ADB2, "DA020000", g_weaponRack_000002DA.c_str() },
	// :0041B9BD
	// :0041B9BD 889401D4020000          mov byte ptr [ecx+eax+000002D4], dl
	{ 0x0001ADC0, "D4020000", g_weaponRack_000002D4.c_str() },
	// :0041B9CA
	// :0041B9CA 66C78401DB020000FFFF    mov word ptr [ecx+eax+000002DB], FFFF
	{ 0x0001ADCE, "DB020000", g_weaponRack_000002DB.c_str() },
	// :0041BAC5
	// :0041BAC5 389C06E1020000          cmp byte ptr [esi+eax+000002E1], bl
	{ 0x0001AEC8, "E1020000", g_weaponRack_000002E1.c_str() },
	// :0041BADE
	// :0041BADE 66898C06DF020000        mov word ptr [esi+eax+000002DF], cx
	{ 0x0001AEE2, "DF020000", g_weaponRack_000002DF.c_str() },
	// :0041BAE8
	// :0041BAE8 66C78406DF0200001901    mov word ptr [esi+eax+000002DF], 0119
	{ 0x0001AEEC, "DF020000", g_weaponRack_000002DF.c_str() },
	// :0041BAFA
	// :0041BAFA 888C06E1020000          mov byte ptr [esi+eax+000002E1], cl
	{ 0x0001AEFD, "E1020000", g_weaponRack_000002E1.c_str() },
	// :0041BB07
	// :0041BB07 C68401E30200007F        mov byte ptr [ecx+eax+000002E3], 7F
	{ 0x0001AF0A, "E3020000", g_weaponRack_000002E3.c_str() },
	// :0041BB15
	// :0041BB15 889C01E4020000          mov byte ptr [ecx+eax+000002E4], bl
	{ 0x0001AF18, "E4020000", g_weaponRack_000002E4.c_str() },
	// :0041BB22
	// :0041BB22 C68401E7020000FF        mov byte ptr [ecx+eax+000002E7], FF
	{ 0x0001AF25, "E7020000", g_weaponRack_000002E7.c_str() },
	// :0041BB30
	// :0041BB30 C68401E8020000FF        mov byte ptr [ecx+eax+000002E8], FF
	{ 0x0001AF33, "E8020000", g_weaponRack_000002E8.c_str() },
	// :0041BB3E
	// :0041BB3E 889401E2020000          mov byte ptr [ecx+eax+000002E2], dl
	{ 0x0001AF41, "E2020000", g_weaponRack_000002E2.c_str() },
	// :0041BB4B
	// :0041BB4B 66C78401E9020000FFFF    mov word ptr [ecx+eax+000002E9], FFFF
	{ 0x0001AF4F, "E9020000", g_weaponRack_000002E9.c_str() },
	// :0041BD1E
	// :0041BD1E 66899C02D1020000        mov word ptr [edx+eax+000002D1], bx
	{ 0x0001B122, "D1020000", g_weaponRack_000002D1.c_str() },
	// :0041BD2C
	// :0041BD2C C68402D302000003        mov byte ptr [edx+eax+000002D3], 03
	{ 0x0001B12F, "D3020000", g_weaponRack_000002D3.c_str() },
	// :0041BD3A
	// :0041BD3A 889C02D5020000          mov byte ptr [edx+eax+000002D5], bl
	{ 0x0001B13D, "D5020000", g_weaponRack_000002D5.c_str() },
	// :0041BD47
	// :0041BD47 6689B402DB020000        mov word ptr [edx+eax+000002DB], si
	{ 0x0001B14B, "DB020000", g_weaponRack_000002DB.c_str() },
	// :0041BD55
	// :0041BD55 889C02D6020000          mov byte ptr [edx+eax+000002D6], bl
	{ 0x0001B158, "D6020000", g_weaponRack_000002D6.c_str() },
	// :0041BDD2
	// :0041BDD2 66899430DF020000        mov word ptr [eax+esi+000002DF], dx
	{ 0x0001B1D6, "DF020000", g_weaponRack_000002DF.c_str() },
	// :0041BDDF
	// :0041BDDF C68430E102000003        mov byte ptr [eax+esi+000002E1], 03
	{ 0x0001B1E2, "E1020000", g_weaponRack_000002E1.c_str() },
	// :0041BDED
	// :0041BDED C68431E30200007F        mov byte ptr [ecx+esi+000002E3], 7F
	{ 0x0001B1F0, "E3020000", g_weaponRack_000002E3.c_str() },
	// :0041BDFB
	// :0041BDFB 66C78432E9020000FFFF    mov word ptr [edx+esi+000002E9], FFFF
	{ 0x0001B1FF, "E9020000", g_weaponRack_000002E9.c_str() },
	// :0041BEB1
	// :0041BEB1 888431D6020000          mov byte ptr [ecx+esi+000002D6], al
	{ 0x0001B2B4, "D6020000", g_weaponRack_000002D6.c_str() },
	// :004A9A92
	// :004A9A92 80BC05E102000004        cmp byte ptr [ebp+eax+000002E1], 04
	{ 0x000A8E95, "E1020000", g_weaponRack_000002E1.c_str() },
	// :004A9AA0
	// :004A9AA0 6681BC05E9020000FFFF    cmp word ptr [ebp+eax+000002E9], FFFF
	{ 0x000A8EA4, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004A9AB2
	// :004A9AB2 8D840DEB020000          lea eax, dword ptr [ebp+ecx+000002EB]
	{ 0x000A8EB5, "EB020000", g_weaponRack_000002EB.c_str() },
	// :004A9AB9
	// :004A9AB9 668B8C0DEB020000        mov cx, word ptr [ebp+ecx+000002EB]
	{ 0x000A8EBD, "EB020000", g_weaponRack_000002EB.c_str() },
	// :004A9ADD
	// :004A9ADD 80BC31E102000004        cmp byte ptr [ecx+esi+000002E1], 04
	{ 0x000A8EE0, "E1020000", g_weaponRack_000002E1.c_str() },
	// :004A9B06
	// :004A9B06 8A9431E2020000          mov dl, byte ptr [ecx+esi+000002E2]
	{ 0x000A8F09, "E2020000", g_weaponRack_000002E2.c_str() },
	// :004A9D4D
	// :004A9D4D C68411E402000000        mov byte ptr [ecx+edx+000002E4], 00
	{ 0x000A9150, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004A9E68
	// :004A9E68 6689840AE9020000        mov word ptr [edx+ecx+000002E9], ax
	{ 0x000A926C, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004A9E75
	// :004A9E75 66818408EB020000D801    add word ptr [eax+ecx+000002EB], 01D8
	{ 0x000A9279, "EB020000", g_weaponRack_000002EB.c_str() },
	// :004A9E85
	// :004A9E85 6683BC0DE502000000      cmp word ptr [ebp+ecx+000002E5], 0000
	{ 0x000A9289, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004A9E99
	// :004A9E99 6689940DE5020000        mov word ptr [ebp+ecx+000002E5], dx
	{ 0x000A929D, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004A9FBD
	// :004A9FBD 6689840AE9020000        mov word ptr [edx+ecx+000002E9], ax
	{ 0x000A93C1, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004A9FCA
	// :004A9FCA 66818408EB020000D801    add word ptr [eax+ecx+000002EB], 01D8
	{ 0x000A93CE, "EB020000", g_weaponRack_000002EB.c_str() },
	// :004A9FDA
	// :004A9FDA 6683BC0DE502000000      cmp word ptr [ebp+ecx+000002E5], 0000
	{ 0x000A93DE, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004A9FEE
	// :004A9FEE 6689940DE5020000        mov word ptr [ebp+ecx+000002E5], dx
	{ 0x000A93F2, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004AA088
	// :004AA088 C68405E402000001        mov byte ptr [ebp+eax+000002E4], 01
	{ 0x000A948B, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004AA0BD
	// :004AA0BD 6689840AE9020000        mov word ptr [edx+ecx+000002E9], ax
	{ 0x000A94C1, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004AA0CA
	// :004AA0CA 66818408EB020000D801    add word ptr [eax+ecx+000002EB], 01D8
	{ 0x000A94CE, "EB020000", g_weaponRack_000002EB.c_str() },
	// :004AA0DA
	// :004AA0DA 6683BC0DE502000000      cmp word ptr [ebp+ecx+000002E5], 0000
	{ 0x000A94DE, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004AA0EE
	// :004AA0EE 6689940DE5020000        mov word ptr [ebp+ecx+000002E5], dx
	{ 0x000A94F2, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004AA236
	// :004AA236 8DB0E9020000            lea esi, dword ptr [eax+000002E9]
	{ 0x000A9638, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004AA415
	// :004AA415 8D8FE9020000            lea ecx, dword ptr [edi+000002E9]
	{ 0x000A9817, "E9020000", g_weaponRack_000002E9.c_str() },
	// :00491BD9
	// :00491BD9 80BC31E102000003        cmp byte ptr [ecx+esi+000002E1], 03
	{ 0x00090FDC, "E1020000", g_weaponRack_000002E1.c_str() },
	// :00491BE7
	// :00491BE7 8A9431E4020000          mov dl, byte ptr [ecx+esi+000002E4]
	{ 0x00090FEA, "E4020000", g_weaponRack_000002E4.c_str() },
	// :00491C81
	// :00491C81 FE8C30E4020000          dec byte ptr [eax+esi+000002E4]
	{ 0x00091084, "E4020000", g_weaponRack_000002E4.c_str() },
	// :00491E68
	// :00491E68 8A9C51F2020000          mov bl, byte ptr [ecx+2*edx+000002F2]
	{ 0x0009126B, "F2020000", g_weaponRack_000002F2.c_str() },
	// :00491E72
	// :00491E72 8A9451E4020000          mov dl, byte ptr [ecx+2*edx+000002E4]
	{ 0x00091275, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004F4704
	// :004F4704 8DAEE9020000            lea ebp, dword ptr [esi+000002E9]
	{ 0x000F3B06, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004653DE
	// :004653DE 8DB2E1020000            lea esi, dword ptr [edx+000002E1]
	{ 0x000647E0, "E1020000", g_weaponRack_000002E1.c_str() },
	// :00469F8F
	// :00469F8F 8D8AE1020000            lea ecx, dword ptr [edx+000002E1]
	{ 0x00069391, "E1020000", g_weaponRack_000002E1.c_str() },
	// :0042822E
	// :0042822E C68451E402000001        mov byte ptr [ecx+2*edx+000002E4], 01
	{ 0x00027631, "E4020000", g_weaponRack_000002E4.c_str() },
	// :00428253
	// :00428253 C68451E402000001        mov byte ptr [ecx+2*edx+000002E4], 01
	{ 0x00027656, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004D60C3
	// :004D60C3 660FB6B471E4020000      movzx si, byte ptr [ecx+2*esi+000002E4]
	{ 0x000D54C8, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004D60D3
	// :004D60D3 660FB69459E4020000      movzx dx, byte ptr [ecx+2*ebx+000002E4]
	{ 0x000D54D8, "E4020000", g_weaponRack_000002E4.c_str() },
	// :0045CB81
	// :0045CB81 66898C30DF020000        mov word ptr [eax+esi+000002DF], cx
	{ 0x0005BF85, "DF020000", g_weaponRack_000002DF.c_str() },
	// :0045CB8F
	// :0045CB8F C68432E102000003        mov byte ptr [edx+esi+000002E1], 03
	{ 0x0005BF92, "E1020000", g_weaponRack_000002E1.c_str() },
	// :0045CBA0
	// :0045CBA0 C68430E30200007F        mov byte ptr [eax+esi+000002E3], 7F
	{ 0x0005BFA3, "E3020000", g_weaponRack_000002E3.c_str() },
	// :0045CBAE
	// :0045CBAE 66C78431E9020000FFFF    mov word ptr [ecx+esi+000002E9], FFFF
	{ 0x0005BFB2, "E9020000", g_weaponRack_000002E9.c_str() },
	// :0045CC4D
	// :0045CC4D 888431D6020000          mov byte ptr [ecx+esi+000002D6], al
	{ 0x0005C050, "D6020000", g_weaponRack_000002D6.c_str() },
	// :00459DAC
	// :00459DAC 8D9457E4020000          lea edx, dword ptr [edi+2*edx+000002E4]
	{ 0x000591AF, "E4020000", g_weaponRack_000002E4.c_str() },
	// :00459EC0
	// :00459EC0 8D8C4FE4020000          lea ecx, dword ptr [edi+2*ecx+000002E4]
	{ 0x000592C3, "E4020000", g_weaponRack_000002E4.c_str() },
	// :0046ABF0
	// :0046ABF0 8D9455E3020000          lea edx, dword ptr [ebp+2*edx+000002E3]
	{ 0x00069FF3, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004A6357
	// :004A6357 81C1E9020000            add ecx, 000002E9
	{ 0x000A5759, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004BA004
	// :004BA004 8D9456E4020000          lea edx, dword ptr [esi+2*edx+000002E4]
	{ 0x000B9407, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004A85FF
	// :004A85FF 668B8419DF020000        mov ax, word ptr [ecx+ebx+000002DF]
	{ 0x000A7A03, "DF020000", g_weaponRack_000002DF.c_str() },
	// :004A8617
	// :004A8617 8A8419E4020000          mov al, byte ptr [ecx+ebx+000002E4]
	{ 0x000A7A1A, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004A8626
	// :004A8626 6683BC19E502000000      cmp word ptr [ecx+ebx+000002E5], 0000
	{ 0x000A7A2A, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004A8635
	// :004A8635 66C78419E9020000FFFF    mov word ptr [ecx+ebx+000002E9], FFFF
	{ 0x000A7A39, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004A897D
	// :004A897D 6689841AE9020000        mov word ptr [edx+ebx+000002E9], ax
	{ 0x000A7D81, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004A898B
	// :004A898B 668B8419E9020000        mov ax, word ptr [ecx+ebx+000002E9]
	{ 0x000A7D8F, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004A89D3
	// :004A89D3 668B8419DF020000        mov ax, word ptr [ecx+ebx+000002DF]
	{ 0x000A7DD7, "DF020000", g_weaponRack_000002DF.c_str() },
	// :004A8A23
	// :004A8A23 66C78418E50200007012    mov word ptr [eax+ebx+000002E5], 1270
	{ 0x000A7E27, "E5020000", g_weaponRack_000002E5.c_str() },
	// :00491238
	// :00491238 8A9C51F2020000          mov bl, byte ptr [ecx+2*edx+000002F2]
	{ 0x0009063B, "F2020000", g_weaponRack_000002F2.c_str() },
	// :00491244
	// :00491244 8A90E4020000            mov dl, byte ptr [eax+000002E4]
	{ 0x00090646, "E4020000", g_weaponRack_000002E4.c_str() },
	// :0046DFB6
	// :0046DFB6 8D8AE1020000            lea ecx, dword ptr [edx+000002E1]
	{ 0x0006D3B8, "E1020000", g_weaponRack_000002E1.c_str() },
	// :0040D52E
	// :0040D52E 80BC4AE302000040        cmp byte ptr [edx+2*ecx+000002E3], 40
	{ 0x0000C931, "E3020000", g_weaponRack_000002E3.c_str() },
	// :00476DF4
	// :00476DF4 660FB6AC72E4020000      movzx bp, byte ptr [edx+2*esi+000002E4]
	{ 0x000761F9, "E4020000", g_weaponRack_000002E4.c_str() },
	// :0047484F
	// :0047484F 660FB6944BE4020000      movzx dx, byte ptr [ebx+2*ecx+000002E4]
	{ 0x00073C54, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004748BB
	// :004748BB 660FB6944BE4020000      movzx dx, byte ptr [ebx+2*ecx+000002E4]
	{ 0x00073CC0, "E4020000", g_weaponRack_000002E4.c_str() },
	// :00474995
	// :00474995 660FB6944BE4020000      movzx dx, byte ptr [ebx+2*ecx+000002E4]
	{ 0x00073D9A, "E4020000", g_weaponRack_000002E4.c_str() },
	// :00474A07
	// :00474A07 660FB6944BE4020000      movzx dx, byte ptr [ebx+2*ecx+000002E4]
	{ 0x00073E0C, "E4020000", g_weaponRack_000002E4.c_str() },
	// :00474A67
	// :00474A67 660FB6944BE4020000      movzx dx, byte ptr [ebx+2*ecx+000002E4]
	{ 0x00073E6C, "E4020000", g_weaponRack_000002E4.c_str() },
	// :00474AD7
	// :00474AD7 660FB6944BE4020000      movzx dx, byte ptr [ebx+2*ecx+000002E4]
	{ 0x00073EDC, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004B13D0
	// :004B13D0 8DBC56E4020000          lea edi, dword ptr [esi+2*edx+000002E4]
	{ 0x000B07D3, "E4020000", g_weaponRack_000002E4.c_str() },
	// :00508D99
	// :00508D99 389C7AE1020000          cmp byte ptr [edx+2*edi+000002E1], bl
	{ 0x0010819C, "E1020000", g_weaponRack_000002E1.c_str() },
	// :00508DA5
	// :00508DA5 66C780E9020000FFFF      mov word ptr [ebx+000002E9], FFFF
	{ 0x001081A8, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004A8B1F
	// :004A8B1F 8A8405E1020000          mov al, byte ptr [ebp+eax+000002E1]
	{ 0x000A7F22, "E1020000", g_weaponRack_000002E1.c_str() },
	// :004A8B36
	// :004A8B36 6683BC15EB02000000      cmp word ptr [ebp+edx+000002EB], 0000
	{ 0x000A7F3A, "EB020000", g_weaponRack_000002EB.c_str() },
	// :004A8B4C
	// :004A8B4C 6689B41DE9020000        mov word ptr [ebp+ebx+000002E9], si
	{ 0x000A7F50, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004A8B59
	// :004A8B59 C68418E402000000        mov byte ptr [eax+ebx+000002E4], 00
	{ 0x000A7F5C, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004A8B67
	// :004A8B67 C68419E7020000FF        mov byte ptr [ecx+ebx+000002E7], FF
	{ 0x000A7F6A, "E7020000", g_weaponRack_000002E7.c_str() },
	// :004A8B75
	// :004A8B75 C6841AE8020000FF        mov byte ptr [edx+ebx+000002E8], FF
	{ 0x000A7F78, "E8020000", g_weaponRack_000002E8.c_str() },
	// :004A8D22
	// :004A8D22 8A8C1DE2020000          mov cl, byte ptr [ebp+ebx+000002E2]
	{ 0x000A8125, "E2020000", g_weaponRack_000002E2.c_str() },
	// :004A8DCB
	// :004A8DCB 8D95E9020000            lea edx, dword ptr [ebp+000002E9]
	{ 0x000A81CD, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004A8F3B
	// :004A8F3B 6689940DE9020000        mov word ptr [ebp+ecx+000002E9], dx
	{ 0x000A833F, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004A8F48
	// :004A8F48 66818408EB020000D801    add word ptr [eax+ecx+000002EB], 01D8
	{ 0x000A834C, "EB020000", g_weaponRack_000002EB.c_str() },
	// :004A8F58
	// :004A8F58 6683BC0DE502000000      cmp word ptr [ebp+ecx+000002E5], 0000
	{ 0x000A835C, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004A8F98
	// :004A8F98 8A9405E2020000          mov dl, byte ptr [ebp+eax+000002E2]
	{ 0x000A839B, "E2020000", g_weaponRack_000002E2.c_str() },
	// :004A90C0
	// :004A90C0 8D8DE9020000            lea ecx, dword ptr [ebp+000002E9]
	{ 0x000A84C2, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004A9178
	// :004A9178 6689940DE9020000        mov word ptr [ebp+ecx+000002E9], dx
	{ 0x000A857C, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004A9190
	// :004A9190 8A8C05E2020000          mov cl, byte ptr [ebp+eax+000002E2]
	{ 0x000A8593, "E2020000", g_weaponRack_000002E2.c_str() },
	// :004A92EE
	// :004A92EE 8D85E1020000            lea eax, dword ptr [ebp+000002E1]
	{ 0x000A86F0, "E1020000", g_weaponRack_000002E1.c_str() },
	// :004A93A9
	// :004A93A9 8DB5E9020000            lea esi, dword ptr [ebp+000002E9]
	{ 0x000A87AB, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004A95EF
	// :004A95EF 6689BC0DE9020000        mov word ptr [ebp+ecx+000002E9], di
	{ 0x000A89F3, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004A95FC
	// :004A95FC 66818408EB020000D801    add word ptr [eax+ecx+000002EB], 01D8
	{ 0x000A8A00, "EB020000", g_weaponRack_000002EB.c_str() },
	// :004A960C
	// :004A960C 6683BC0DE502000000      cmp word ptr [ebp+ecx+000002E5], 0000
	{ 0x000A8A10, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004A9628
	// :004A9628 66899405E5020000        mov word ptr [ebp+eax+000002E5], dx
	{ 0x000A8A2C, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004A963D
	// :004A963D 6689BC0DE9020000        mov word ptr [ebp+ecx+000002E9], di
	{ 0x000A8A41, "E9020000", g_weaponRack_000002E9.c_str() },
	// :004A964A
	// :004A964A 66818408EB020000D801    add word ptr [eax+ecx+000002EB], 01D8
	{ 0x000A8A4E, "EB020000", g_weaponRack_000002EB.c_str() },
	// :004A965A
	// :004A965A 6683BC0DE502000000      cmp word ptr [ebp+ecx+000002E5], 0000
	{ 0x000A8A5E, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004A9672
	// :004A9672 6689840DE5020000        mov word ptr [ebp+ecx+000002E5], ax
	{ 0x000A8A76, "E5020000", g_weaponRack_000002E5.c_str() },
	// :004915A1
	// :004915A1 80BC31E102000000        cmp byte ptr [ecx+esi+000002E1], 00
	{ 0x000909A4, "E1020000", g_weaponRack_000002E1.c_str() },
	// :004915AF
	// :004915AF 8A9431E3020000          mov dl, byte ptr [ecx+esi+000002E3]
	{ 0x000909B2, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004915BE
	// :004915BE 668BAC31DF020000        mov bp, word ptr [ecx+esi+000002DF]
	{ 0x000909C2, "DF020000", g_weaponRack_000002DF.c_str() },
	// :00491663
	// :00491663 80BC30E102000004        cmp byte ptr [eax+esi+000002E1], 04
	{ 0x00090A66, "E1020000", g_weaponRack_000002E1.c_str() },
	// :0049166D
	// :0049166D 8A8C30E3020000          mov cl, byte ptr [eax+esi+000002E3]
	{ 0x00090A70, "E3020000", g_weaponRack_000002E3.c_str() },
	// :00491674
	// :00491674 8D8430E3020000          lea eax, dword ptr [eax+esi+000002E3]
	{ 0x00090A77, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004916A9
	// :004916A9 8A8C30E3020000          mov cl, byte ptr [eax+esi+000002E3]
	{ 0x00090AAC, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004916BA
	// :004916BA 8A8C30E3020000          mov cl, byte ptr [eax+esi+000002E3]
	{ 0x00090ABD, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004916CB
	// :004916CB 8A8C30E3020000          mov cl, byte ptr [eax+esi+000002E3]
	{ 0x00090ACE, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004916D4
	// :004916D4 888C30E3020000          mov byte ptr [eax+esi+000002E3], cl
	{ 0x00090AD7, "E3020000", g_weaponRack_000002E3.c_str() },
	// :00491713
	// :00491713 8A8C30E3020000          mov cl, byte ptr [eax+esi+000002E3]
	{ 0x00090B16, "E3020000", g_weaponRack_000002E3.c_str() },
	// :0049171E
	// :0049171E C68430E302000000        mov byte ptr [eax+esi+000002E3], 00
	{ 0x00090B21, "E3020000", g_weaponRack_000002E3.c_str() },
	// :00477237
	// :00477237 8DBDE9020000            lea edi, dword ptr [ebp+000002E9]
	{ 0x00076639, "E9020000", g_weaponRack_000002E9.c_str() },
	// :00519D1C
	// :00519D1C 8888E7020000            mov byte ptr [eax+000002E7], cl
	{ 0x0011911E, "E7020000", g_weaponRack_000002E7.c_str() },
	// :00519D28
	// :00519D28 8888E8020000            mov byte ptr [eax+000002E8], cl
	{ 0x0011912A, "E8020000", g_weaponRack_000002E8.c_str() },
	// :00470222
	// :00470222 660FBE8C4AE3020000      movsx cx, byte ptr [edx+2*ecx+000002E3]
	{ 0x0006F627, "E3020000", g_weaponRack_000002E3.c_str() },
	// :0046C02E
	// :0046C02E 8D85E3020000            lea eax, dword ptr [ebp+000002E3]
	{ 0x0006B430, "E3020000", g_weaponRack_000002E3.c_str() },
	// :00514EC1
	// :00514EC1 66898C06DF020000        mov word ptr [esi+eax+000002DF], cx
	{ 0x001142C5, "DF020000", g_weaponRack_000002DF.c_str() },
	// :00514ECF
	// :00514ECF C68401E102000004        mov byte ptr [ecx+eax+000002E1], 04
	{ 0x001142D2, "E1020000", g_weaponRack_000002E1.c_str() },
	// :00514EDD
	// :00514EDD 66898C06DF020000        mov word ptr [esi+eax+000002DF], cx
	{ 0x001142E1, "DF020000", g_weaponRack_000002DF.c_str() },
	// :00514EEE
	// :00514EEE 888C06E1020000          mov byte ptr [esi+eax+000002E1], cl
	{ 0x001142F1, "E1020000", g_weaponRack_000002E1.c_str() },
	// :00514EFF
	// :00514EFF C68401D50200007F        mov byte ptr [ecx+eax+000002D5], 7F
	{ 0x00114302, "D5020000", g_weaponRack_000002D5.c_str() },
	// :00514F0D
	// :00514F0D 889C01D6020000          mov byte ptr [ecx+eax+000002D6], bl
	{ 0x00114310, "D6020000", g_weaponRack_000002D6.c_str() },
	// :00514F1A
	// :00514F1A C68401D9020000FF        mov byte ptr [ecx+eax+000002D9], FF
	{ 0x0011431D, "D9020000", g_weaponRack_000002D9.c_str() },
	// :00514F28
	// :00514F28 C68401DA020000FF        mov byte ptr [ecx+eax+000002DA], FF
	{ 0x0011432B, "DA020000", g_weaponRack_000002DA.c_str() },
	// :00514F3A
	// :00514F3A 888C06D4020000          mov byte ptr [esi+eax+000002D4], cl
	{ 0x0011433D, "D4020000", g_weaponRack_000002D4.c_str() },
	// :00514F47
	// :00514F47 66C78401DB020000FFFF    mov word ptr [ecx+eax+000002DB], FFFF
	{ 0x0011434B, "DB020000", g_weaponRack_000002DB.c_str() },
	// :004A7C71
	// :004A7C71 80BC58E402000000        cmp byte ptr [eax+2*ebx+000002E4], 00
	{ 0x000A7074, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004A825E
	// :004A825E 8A9451E4020000          mov dl, byte ptr [ecx+2*edx+000002E4]
	{ 0x000A7661, "E4020000", g_weaponRack_000002E4.c_str() },
	// :004A8265
	// :004A8265 3A90F2020000            cmp dl, byte ptr [eax+000002F2]
	{ 0x000A7667, "F2020000", g_weaponRack_000002F2.c_str() },
	// :0040EA00
	// :0040EA00 C68473E302000000        mov byte ptr [ebx+2*esi+000002E3], 00
	{ 0x0000DE03, "E3020000", g_weaponRack_000002E3.c_str() },
	// :00470B11
	// :00470B11 05E3020000              add eax, 000002E3
	{ 0x0006FF12, "E3020000", g_weaponRack_000002E3.c_str() },
	// :0048FD81
	// :0048FD81 660FB688F2020000        movzx cx, byte ptr [eax+000002F2]
	{ 0x0008F185, "F2020000", g_weaponRack_000002F2.c_str() },
	// :0048FD89
	// :0048FD89 660FB680E4020000        movzx ax, byte ptr [eax+000002E4]
	{ 0x0008F18D, "E4020000", g_weaponRack_000002E4.c_str() },
	// :00490556
	// :00490556 8D96E3020000            lea edx, dword ptr [esi+000002E3]
	{ 0x0008F958, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004905B1
	// :004905B1 8A8C56E3020000          mov cl, byte ptr [esi+2*edx+000002E3]
	{ 0x0008F9B4, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004905BA
	// :004905BA 8D9456E3020000          lea edx, dword ptr [esi+2*edx+000002E3]
	{ 0x0008F9BD, "E3020000", g_weaponRack_000002E3.c_str() },
	// :00490628
	// :00490628 8D8EE3020000            lea ecx, dword ptr [esi+000002E3]
	{ 0x0008FA2A, "E3020000", g_weaponRack_000002E3.c_str() },
	// :00490894
	// :00490894 8A9401E1020000          mov dl, byte ptr [ecx+eax+000002E1]
	{ 0x0008FC97, "E1020000", g_weaponRack_000002E1.c_str() },
	// :004908E0
	// :004908E0 009401E3020000          add byte ptr [ecx+eax+000002E3], dl
	{ 0x0008FCE3, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004908E7
	// :004908E7 8D8C01E3020000          lea ecx, dword ptr [ecx+eax+000002E3]
	{ 0x0008FCEA, "E3020000", g_weaponRack_000002E3.c_str() },
	// :004908F9
	// :004908F9 80BC01E302000000        cmp byte ptr [ecx+eax+000002E3], 00
	{ 0x0008FCFC, "E3020000", g_weaponRack_000002E3.c_str() },
	// :00490903
	// :00490903 C68401E302000000        mov byte ptr [ecx+eax+000002E3], 00
	{ 0x0008FD06, "E3020000", g_weaponRack_000002E3.c_str() },
	// :00490916
	// :00490916 8A9401E3020000          mov dl, byte ptr [ecx+eax+000002E3]
	{ 0x0008FD19, "E3020000", g_weaponRack_000002E3.c_str() },
	// :00490921
	// :00490921 C68401E30200007F        mov byte ptr [ecx+eax+000002E3], 7F
	{ 0x0008FD24, "E3020000", g_weaponRack_000002E3.c_str() },
	// :00490959
	// :00490959 8D91E3020000            lea edx, dword ptr [ecx+000002E3]
	{ 0x0008FD5B, "E3020000", g_weaponRack_000002E3.c_str() },
	// :00490C76
	// :00490C76 8A9450E1020000          mov dl, byte ptr [eax+2*edx+000002E1]
	{ 0x00090079, "E1020000", g_weaponRack_000002E1.c_str() },
	// :00490C82
	// :00490C82 668B89E9020000          mov cx, word ptr [ecx+000002E9]
	{ 0x00090085, "E9020000", g_weaponRack_000002E9.c_str() },

	{ 0x01A075, "B938000000", "B9C0010000" },
	{ 0x01AF71, "6683FD10", "6683FD80" },

	// :00401E13
	// :00401E13 660FB6B4D0E9B55B00      movzx si, byte ptr [eax+8*edx+005BB5E9]
	{ 0x00001218, "E9B55B00", g_weaponSlots_005BB5E9.c_str() },
	// :0040D5B2
	// :0040D5B2 668B94F9E2B55B00        mov dx, word ptr [ecx+8*edi+005BB5E2]
	{ 0x0000C9B6, "E2B55B00", g_weaponSlots_005BB5E2.c_str() },
	// :0040D5BA
	// :0040D5BA 668B84F9E4B55B00        mov ax, word ptr [ecx+8*edi+005BB5E4]
	{ 0x0000C9BE, "E4B55B00", g_weaponSlots_005BB5E4.c_str() },
	// :0040D5C2
	// :0040D5C2 668B8CF9E6B55B00        mov cx, word ptr [ecx+8*edi+005BB5E6]
	{ 0x0000C9C6, "E6B55B00", g_weaponSlots_005BB5E6.c_str() },
	// :0040D68B
	// :0040D68B 668B91E2B55B00          mov dx, word ptr [ecx+005BB5E2]
	{ 0x0000CA8E, "E2B55B00", g_weaponSlots_005BB5E2.c_str() },
	// :0040D692
	// :0040D692 668B81E4B55B00          mov ax, word ptr [ecx+005BB5E4]
	{ 0x0000CA95, "E4B55B00", g_weaponSlots_005BB5E4.c_str() },
	// :0040D699
	// :0040D699 668B89E6B55B00          mov cx, word ptr [ecx+005BB5E6]
	{ 0x0000CA9C, "E6B55B00", g_weaponSlots_005BB5E6.c_str() },
	// :004331DC
	// :004331DC 668988E2B55B00          mov word ptr [eax+005BB5E2], cx
	{ 0x000325DF, "E2B55B00", g_weaponSlots_005BB5E2.c_str() },
	// :004331E8
	// :004331E8 668990E6B55B00          mov word ptr [eax+005BB5E6], dx
	{ 0x000325EB, "E6B55B00", g_weaponSlots_005BB5E6.c_str() },
	// :004331F3
	// :004331F3 668988E4B55B00          mov word ptr [eax+005BB5E4], cx
	{ 0x000325F6, "E4B55B00", g_weaponSlots_005BB5E4.c_str() },
	// :004331FA
	// :004331FA 8890E9B55B00            mov byte ptr [eax+005BB5E9], dl
	{ 0x000325FC, "E9B55B00", g_weaponSlots_005BB5E9.c_str() },
	// :00433200
	// :00433200 C680E8B55B00FF          mov byte ptr [eax+005BB5E8], FF
	{ 0x00032602, "E8B55B00", g_weaponSlots_005BB5E8.c_str() },
	// :00433242
	// :00433242 8884DEE8B55B00          mov byte ptr [esi+8*ebx+005BB5E8], al
	{ 0x00032645, "E8B55B00", g_weaponSlots_005BB5E8.c_str() },
	// :004333E7
	// :004333E7 668988E2B55B00          mov word ptr [eax+005BB5E2], cx
	{ 0x000327EA, "E2B55B00", g_weaponSlots_005BB5E2.c_str() },
	// :004333F3
	// :004333F3 668990E6B55B00          mov word ptr [eax+005BB5E6], dx
	{ 0x000327F6, "E6B55B00", g_weaponSlots_005BB5E6.c_str() },
	// :004333FE
	// :004333FE 668988E4B55B00          mov word ptr [eax+005BB5E4], cx
	{ 0x00032801, "E4B55B00", g_weaponSlots_005BB5E4.c_str() },
	// :00433405
	// :00433405 8890E9B55B00            mov byte ptr [eax+005BB5E9], dl
	{ 0x00032807, "E9B55B00", g_weaponSlots_005BB5E9.c_str() },
	// :00465464
	// :00465464 0FBF88E6B55B00          movsx ecx, word ptr [eax+005BB5E6]
	{ 0x00064867, "E6B55B00", g_weaponSlots_005BB5E6.c_str() },
	// :00465474
	// :00465474 0FBF90E4B55B00          movsx edx, word ptr [eax+005BB5E4]
	{ 0x00064877, "E4B55B00", g_weaponSlots_005BB5E4.c_str() },
	// :00465481
	// :00465481 0FBF80E2B55B00          movsx eax, word ptr [eax+005BB5E2]
	{ 0x00064884, "E2B55B00", g_weaponSlots_005BB5E2.c_str() },
	// :0047730E
	// :0047730E 8A94F0E9B55B00          mov dl, byte ptr [eax+8*esi+005BB5E9]
	{ 0x00076711, "E9B55B00", g_weaponSlots_005BB5E9.c_str() },
	// :00491462
	// :00491462 6683BCD0E4B55B00C0      cmp word ptr [eax+8*edx+005BB5E4], FFC0
	{ 0x00090866, "E4B55B00", g_weaponSlots_005BB5E4.c_str() },
	// :00492340
	// :00492340 668B90E2B55B00          mov dx, word ptr [eax+005BB5E2]
	{ 0x00091743, "E2B55B00", g_weaponSlots_005BB5E2.c_str() },
	// :00492347
	// :00492347 668B88E4B55B00          mov cx, word ptr [eax+005BB5E4]
	{ 0x0009174A, "E4B55B00", g_weaponSlots_005BB5E4.c_str() },
	// :00492352
	// :00492352 668B90E6B55B00          mov dx, word ptr [eax+005BB5E6]
	{ 0x00091755, "E6B55B00", g_weaponSlots_005BB5E6.c_str() },
	// :00492478
	// :00492478 668B90E2B55B00          mov dx, word ptr [eax+005BB5E2]
	{ 0x0009187B, "E2B55B00", g_weaponSlots_005BB5E2.c_str() },
	// :0049247F
	// :0049247F 668B88E4B55B00          mov cx, word ptr [eax+005BB5E4]
	{ 0x00091882, "E4B55B00", g_weaponSlots_005BB5E4.c_str() },
	// :0049248A
	// :0049248A 668B90E6B55B00          mov dx, word ptr [eax+005BB5E6]
	{ 0x0009188D, "E6B55B00", g_weaponSlots_005BB5E6.c_str() },
	// :004A85DA
	// :004A85DA 8A9406E9B55B00          mov dl, byte ptr [esi+eax+005BB5E9]
	{ 0x000A79DD, "E9B55B00", g_weaponSlots_005BB5E9.c_str() },
	// :004A867B
	// :004A867B 668B88E6B55B00          mov cx, word ptr [eax+005BB5E6]
	{ 0x000A7A7E, "E6B55B00", g_weaponSlots_005BB5E6.c_str() },
	// :004A8682
	// :004A8682 668B90E4B55B00          mov dx, word ptr [eax+005BB5E4]
	{ 0x000A7A85, "E4B55B00", g_weaponSlots_005BB5E4.c_str() },
	// :004A8689
	// :004A8689 668B80E2B55B00          mov ax, word ptr [eax+005BB5E2]
	{ 0x000A7A8C, "E2B55B00", g_weaponSlots_005BB5E2.c_str() },
	// :004A8BF7
	// :004A8BF7 668B90E6B55B00          mov dx, word ptr [eax+005BB5E6]
	{ 0x000A7FFA, "E6B55B00", g_weaponSlots_005BB5E6.c_str() },
	// :004A8BFE
	// :004A8BFE 668B88E4B55B00          mov cx, word ptr [eax+005BB5E4]
	{ 0x000A8001, "E4B55B00", g_weaponSlots_005BB5E4.c_str() },
	// :004A8C06
	// :004A8C06 668B90E2B55B00          mov dx, word ptr [eax+005BB5E2]
	{ 0x000A8009, "E2B55B00", g_weaponSlots_005BB5E2.c_str() },
	// :004A9C6E
	// :004A9C6E 668B88E6B55B00          mov cx, word ptr [eax+005BB5E6]
	{ 0x000A9071, "E6B55B00", g_weaponSlots_005BB5E6.c_str() },
	// :004A9C75
	// :004A9C75 668B90E4B55B00          mov dx, word ptr [eax+005BB5E4]
	{ 0x000A9078, "E4B55B00", g_weaponSlots_005BB5E4.c_str() },
	// :004A9C7C
	// :004A9C7C 668B80E2B55B00          mov ax, word ptr [eax+005BB5E2]
	{ 0x000A907F, "E2B55B00", g_weaponSlots_005BB5E2.c_str() },
	// :004E181B
	// :004E181B 660FB68EE9B55B00        movzx cx, byte ptr [esi+005BB5E9]
	{ 0x000E0C1F, "E9B55B00", g_weaponSlots_005BB5E9.c_str() },
	// :004E1827
	// :004E1827 8A8EE8B55B00            mov cl, byte ptr [esi+005BB5E8]
	{ 0x000E0C29, "E8B55B00", g_weaponSlots_005BB5E8.c_str() },
	// :004E196C
	// :004E196C 6683BCD1E4B55B00C0      cmp word ptr [ecx+8*edx+005BB5E4], FFC0
	{ 0x000E0D70, "E4B55B00", g_weaponSlots_005BB5E4.c_str() },
	// :004E1F75
	// :004E1F75 668B94C8E4B55B00        mov dx, word ptr [eax+8*ecx+005BB5E4]
	{ 0x000E1379, "E4B55B00", g_weaponSlots_005BB5E4.c_str() },
	// :004E1F7D
	// :004E1F7D 668B9CC8E2B55B00        mov bx, word ptr [eax+8*ecx+005BB5E2]
	{ 0x000E1381, "E2B55B00", g_weaponSlots_005BB5E2.c_str() },
	// :004E1F85
	// :004E1F85 668BB4C8E6B55B00        mov si, word ptr [eax+8*ecx+005BB5E6]
	{ 0x000E1389, "E6B55B00", g_weaponSlots_005BB5E6.c_str() },
	// :004F4732
	// :004F4732 8A8CF8E9B55B00          mov cl, byte ptr [eax+8*edi+005BB5E9]
	{ 0x000F3B35, "E9B55B00", g_weaponSlots_005BB5E9.c_str() },
	// :00519818
	// :00519818 0FBF88E4B55B00          movsx ecx, word ptr [eax+005BB5E4]
	{ 0x00118C1B, "E4B55B00", g_weaponSlots_005BB5E4.c_str() },
	// :0051981F
	// :0051981F 0FBF90E6B55B00          movsx edx, word ptr [eax+005BB5E6]
	{ 0x00118C22, "E6B55B00", g_weaponSlots_005BB5E6.c_str() },
	// :00519826
	// :00519826 0FBF80E2B55B00          movsx eax, word ptr [eax+005BB5E2]
	{ 0x00118C29, "E2B55B00", g_weaponSlots_005BB5E2.c_str() },
	// :00519D22
	// :00519D22 8A8AE8B55B00            mov cl, byte ptr [edx+005BB5E8]
	{ 0x00119124, "E8B55B00", g_weaponSlots_005BB5E8.c_str() },

	{ 0x0324D2, "80F910", "80F978" },
	{ 0x032623, "3C10", "3C78" },
	{ 0x032669, "807C242010", "807C242078" },
	{ 0x0326F2, "80FB10", "80FB78" },
	{ 0x0327D8, "80FB10", "80FB78" },
	{ 0x032821, "80FB10", "80FB78" },

	{ 0x0000C9B2, "66", "90" },
	{ 0x0000C9BA, "66", "90" },
	{ 0x0000C9C2, "66", "90" },
	{ 0x0000C9CA, "66", "90" },
	{ 0x0000C9CE, "66", "90" },
	{ 0x0000CA8B, "66", "90" },
	{ 0x0000CA92, "66", "90" },
	{ 0x0000CA99, "66", "90" },
	{ 0x0000CAA3, "66", "90" },
	{ 0x0000CAC4, "0FBFD2", "909090" },
	{ 0x0000CAFA, "0FBFC0", "909090" },
	{ 0x0000CAD0, "0FBF", "908B" },
	{ 0x000325C4, "66", "90" },
	{ 0x000325DC, "66", "90" },
	{ 0x000325D7, "66", "90" },
	{ 0x000325E8, "66", "90" },
	{ 0x000325E3, "66", "90" },
	{ 0x000325F3, "66", "90" },
	{ 0x000327CB, "66", "90" },
	{ 0x000327E7, "66", "90" },
	{ 0x000327E2, "66", "90" },
	{ 0x000327F3, "66", "90" },
	{ 0x000327EE, "66", "90" },
	{ 0x000327FE, "66", "90" },
	{ 0x00064864, "0FBF", "908B" },
	{ 0x00064874, "0FBF", "908B" },
	{ 0x00064881, "0FBF", "908B" },
	{ 0x00090862, "66", "90" },
	{ 0x00091740, "66", "90" },
	{ 0x0009174E, "66", "90" },
	{ 0x00091747, "66", "90" },
	{ 0x00091759, "66", "90" },
	{ 0x00091752, "66", "90" },
	{ 0x0009175D, "66", "90" },
	{ 0x00091878, "66", "90" },
	{ 0x00091886, "66", "90" },
	{ 0x0009187F, "66", "90" },
	{ 0x00091891, "66", "90" },
	{ 0x0009188A, "66", "90" },
	{ 0x00091895, "66", "90" },
	{ 0x000918C5, "0FBF", "908B" },
	{ 0x000918D2, "0FBF", "908B" },
	{ 0x000918D9, "0FBF", "908B" },
	{ 0x000A7A7B, "66", "90" },
	{ 0x000A7A82, "66", "90" },
	{ 0x000A7A89, "66", "90" },
	{ 0x000A7FF7, "66", "90" },
	{ 0x000A7FFE, "66", "90" },
	{ 0x000A8006, "66", "90" },
	{ 0x000A906E, "66", "90" },
	{ 0x000A9075, "66", "90" },
	{ 0x000A907C, "66", "90" },
	{ 0x000E0D6C, "66", "90" },
	{ 0x000E1375, "66", "90" },
	{ 0x000E137D, "66", "90" },
	{ 0x000E1385, "66", "90" },
	{ 0x000E138D, "66", "90" },
	{ 0x000E13B5, "0FBF", "908B" },
	{ 0x000E13B9, "0FBFC3", "908BC3" },
	{ 0x000E13BC, "0FBFCE", "908BCE" },
	{ 0x00118C18, "0FBF", "908B" },
	{ 0x00118C1F, "0FBF", "908B" },
	{ 0x00118C26, "0FBF", "908B" },
	{ 0x000173D1, "66", "90" },
	{ 0x000173D8, "66", "90" },
	{ 0x000173DF, "66", "90" },
	{ 0x0001742B, "66", "90" },
	{ 0x00017432, "66", "90" },
	{ 0x0001743A, "66", "90" },
	{ 0x000A220F, "0FBF", "908B" },
	{ 0x000A2232, "0FBF", "908B" },
	{ 0x000A2264, "0FBF", "908B" },

	{ 0x000A21DC, "8A88C6000000", "E83F5D100090" },

	{ 0x001BA9E2, "0000000000000000", "FF7FFF7FFF7FFFFF" },
	{ 0x001BA9EA, "00000000", g_weaponSlots_Offset1.c_str() },
	{ 0x001BA9EE, "00000000", g_weaponSlots_Offset2.c_str() },
};

static const HookPatch g_patches[] =
{
	MAKE_HOOK_PATCH("'triangling to infinity' patch", g_trianglingPatch),
	MAKE_HOOK_PATCH("'disable briefing wireframe overlay' patch", g_briefingWireframeOverlayPatch),
	MAKE_HOOK_PATCH("disable hitzone bounds check on render patch", g_disableHitzoneBoundsCheckOnRenderPatch),
	MAKE_HOOK_PATCH("'remove textures size limit' patch", g_texturesSizePatch),
	MAKE_HOOK_PATCH("'opt textures count limit' patch", g_texturesCountPatch),
	MAKE_HOOK_PATCH("To call the hook that removes the opt limit of 512 vertices per mesh", g_optLimitPatch),
	MAKE_HOOK_PATCH("'GlowMarkUVArray vertices count' patch", g_glowMarkUVArrayPatch),
	MAKE_HOOK_PATCH("To call the hook that generates spec.rci", g_generateSpecRciPatch),
	MAKE_HOOK_PATCH("'fix depth buffer bug' patch", g_fixDepthBufferBugPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the craft size", g_craftSizePatch),
	MAKE_HOOK_PATCH("To call the hook that sets the craft offsets", g_craftOffsetsPatch),
	MAKE_HOOK_PATCH("To call the hook that compares the meshes count", g_meshesCountPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the meshes info", g_meshesInfoPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the cockpit opt meshes info", g_cockpitOptInfoPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the exterior opt meshes info", g_exteriorOptInfoPatch),
	MAKE_HOOK_PATCH("To call the hook that inits the escape pod craft", g_escapePodCraftInitPatch),
	MAKE_HOOK_PATCH("To call the hook that inits the current craft", g_currentCraftInitPatch),
	MAKE_HOOK_PATCH("To call the hook that filters craft meshes", g_craftMeshesFilterPatch),
	MAKE_HOOK_PATCH("To call the hook that zero inits the craft memory", g_craftZeroMemoryPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the craft engines count", g_craftEnginesPatch),
	MAKE_HOOK_PATCH("To call the hook that gets the craft engine mesh", g_craftEngineMeshPatch),
	MAKE_HOOK_PATCH("To call the hook that selects the lod version", g_selectLodVersionPatch),
	MAKE_HOOK_PATCH("To call the hook that computes the checksum", g_computeChecksumPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the opt meshes collision", g_meshesCollisionPatch),
	MAKE_HOOK_PATCH("To call the hook that sets the weapon racks offsets", g_weaponRacksOffsetsPatch),
};
