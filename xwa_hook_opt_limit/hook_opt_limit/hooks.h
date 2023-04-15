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
};
