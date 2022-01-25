#include "SharedMem.h"

SharedMem::SharedMem(bool OpenCreate) 
{
	InitMemory(OpenCreate);
}

bool SharedMem::InitMemory(bool OpenCreate) 
{
	pSharedMemPtr = NULL;

	if (OpenCreate) {
		hMapFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,    // use paging file
			NULL,                    // default security
			PAGE_READWRITE,          // read/write access
			0,                       // maximum object size (high-order DWORD)
			SHARED_MEM_SIZE,         // maximum object size (low-order DWORD)
			SHARED_MEM_NAME);        // name of mapping object

		if (hMapFile == NULL)
		{
			return false;
		}
	}
	else {
		hMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			SHARED_MEM_NAME);      // name of mapping object

		if (hMapFile == NULL)
		{
			return false;
		}
	}

	pSharedMemPtr = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		SHARED_MEM_SIZE);

	if (pSharedMemPtr == NULL) {
		return false;
	}

	return true;
}

SharedMem::~SharedMem()
{
	UnmapViewOfFile(pSharedMemPtr);

	CloseHandle(hMapFile);
}

void *SharedMem::GetMemoryPtr()
{
	return pSharedMemPtr;
}