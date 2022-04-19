#pragma once

int GetHitDataArrayPtr();

int GetCraftSize();
int GetCraftMeshesCount();
int GetCraftEnginesCount();
int GetCraftOffset_22E();
int GetCraftOffset_260();
int GetCraftOffset_292();
int GetCraftOffset_2CF();
int GetOptModelMeshesInfoSize();
int GetOptModelMeshesInfoPtr();
int GetOptModelMeshesInfoOffsetType();
int GetOptModelMeshesInfoOffsetDescriptor();

int GetExeCraftEnginesArrayPtr();
int GetExeCraftEngineMeshIdsArrayPtr();
int GetSpaceBombEnginesArrayPtr();
int GetSpaceBombEngineMeshIdsArrayPtr();
int GetCockpitEnginesArrayPtr();
int GetCockpitEngineMeshIdsArrayPtr();
int GetExteriorEnginesArrayPtr();
int GetExteriorEngineMeshIdsArrayPtr();

int GenerateSpecRciHook(int* params);

int CraftInitHook(int* params);
int Set0x07821E8Hook(int* params);
int OptModelMeshesInfoHook1(int* params);
int OptModelMeshesInfoHook2(int* params);
int OptModelMeshesInfoHook3(int* params);
int OptModelMeshesInfoHook4(int* params);
int OptModelMeshesInfoHook5(int* params);
int CockpitOptInfoHook(int* params);
int CockpitOptInfoGetDescriptorHook(int* params);
int ExteriorOptInfoHook(int* params);
int ExteriorOptInfoGetDescriptorHook(int* params);
int EscapePodCraftInitHook(int* params);
int CurrentCraftInitHook(int* params);
int CraftMeshesFilterHook(int* params);
int CraftZeroMemoryHook(int* params);

int ExeCraftEnginesArrayHook1(int* params);
int ExeCraftEnginesArrayHook2(int* params);
int ExeCraftEnginesArrayHook3(int* params);
int ExeCraftEnginesArrayHook4(int* params);
int ExeCraftEnginesArrayHook5(int* params);
int ExeCraftEnginesArrayHook6(int* params);

int ExeCraftEngineMeshIdsArrayHook1(int* params);
int ExeCraftEngineMeshIdsArrayHook2(int* params);
int ExeCraftEngineMeshIdsArrayHook3(int* params);
int ExeCraftEngineMeshIdsArrayHook4(int* params);
int ExeCraftEngineMeshIdsArrayHook5(int* params);
int ExeCraftEngineMeshIdsArrayHook6(int* params);
int ExeCraftEngineMeshIdsArrayHook7(int* params);
int ExeCraftEngineMeshIdsArrayHook8(int* params);
int ExeCraftEngineMeshIdsArrayHook9(int* params);

int OptGetMeshEngineGlowCountExeCraftHook(int* params);
int OptGetMeshEngineGlowCountSpaceBombHook(int* params);
int OptGetMeshEngineGlowCountCockpitHook(int* params);
int OptGetMeshEngineGlowCountExteriorHook(int* params);

int L004E28F0_GetMeshHook(int* params);
int L0047AAA0_SetCraftHook(int* params);
int L004E0E10_GetEnginesCountHook(int* params);
int L004E0FA0_ComputePercentOfActiveEnginesHook(int* params);
int L004F22B0_EngineGlowIsDisabledHook(int* params);
int L0042D590_EngineGlowIsDisabledHook(int* params);
int L0042DB60_EngineGlowHook(int* params);
