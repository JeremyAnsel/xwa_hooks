#pragma once

int LoadingScreenStepHook(int* params);
int ConcoursePresentHook(int* params);
int InFlightPresentHook(int* params);

int OptLoadHook(int* params);
int DatLoadHook(int* params);

int MainGameInitCallbackHook(int* params);
int LoadSfxLstHook(int* params);
