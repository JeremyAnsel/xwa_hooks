#include "SharedMem.h"

SharedMemDataJoystick* g_pSharedData = NULL;
SharedMem<SharedMemDataJoystick> g_SharedMem(SHARED_MEM_NAME, true, true);

void InitSharedMem() {
	g_pSharedData = g_SharedMem.GetMemoryPointer();
	g_SharedMem.SetDataReady();
}