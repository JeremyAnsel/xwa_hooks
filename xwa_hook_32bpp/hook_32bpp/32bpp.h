#pragma once

int SetOptNameHook(int* params);

int SetAlphaMaskHook(int* params);

int CreateLightMapHook(int* params);

int ConvertColorMapHook(int* params);

int ComputeGlobalLightsHook(int* params);

int DatImage32Hook(int* params);

int DatImageSetPixelFormatHook(int* params);

int OptCreateD3DTexturesFromTexturesHook(int* params);
int OptCreateD3DTexturesFromTexturesLoopHook(int* params);
int OptTransformTexturesToD3DTexturesHook(int* params);
int OptTransformTexturesToD3DTexturesLoopHook(int* params);
