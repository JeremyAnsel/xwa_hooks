#pragma once

int DockReadPositionHook(int* params);

int AIDockPickUpHook(int* params);
int AIDockPickedUpHook(int* params);

int PlayerDockHook(int* params);
int PLayerDockElevationHook(int* params);
int PLayerDockOrientationHook(int* params);
