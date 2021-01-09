#pragma once

int GetHitDataArrayPtr();

int GetCraftSize();
int GetCraftMeshesCount();
int GetCraftOffset_22E();
int GetCraftOffset_260();
int GetCraftOffset_292();
int GetOptModelMeshesInfoSize();
int GetOptModelMeshesInfoPtr();
int GetOptModelMeshesInfoOffsetType();
int GetOptModelMeshesInfoOffsetDescriptor();

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
