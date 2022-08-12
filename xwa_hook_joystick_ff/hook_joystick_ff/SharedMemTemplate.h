#pragma once

#include <string>

template<class T>
class SharedMem
{
public:
	SharedMem(LPCWSTR name, bool createIfNotExists, bool availableOnCreate);
	~SharedMem();

	T* GetMemoryPointer();
	bool IsDataReady();
	void SetDataReady();

private:
	HANDLE hMapFile;
	HANDLE hSemaphore;
	T* pSharedData;
};

template<class T>
SharedMem<T>::SharedMem(LPCWSTR name, bool createIfNotExists, bool availableOnCreate)
{
	pSharedData = nullptr;
	hSemaphore = NULL;

	if (createIfNotExists)
	{
		hMapFile = CreateFileMappingW(
			INVALID_HANDLE_VALUE,    // use paging file
			NULL,                    // default security
			PAGE_READWRITE,          // read/write access
			0,                       // maximum object size (high-order DWORD)
			sizeof(T),         // maximum object size (low-order DWORD)
			name);        // name of mapping object
	}
	else
	{
		hMapFile = OpenFileMappingW(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			name);      // name of mapping object
	}

	if (!hMapFile)
	{
		return;
	}

	pSharedData = (T*)MapViewOfFile(
		hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		sizeof(T));

	LONG initialCount = availableOnCreate ? 1 : 0;
	initialCount = 1;
	std::wstring semaphoreName = name;
	semaphoreName.append(L"Semaphore");
	hSemaphore = CreateSemaphoreW(
		NULL,					// default security attributes
		initialCount,			// initial count
		1,						// maximum count
		semaphoreName.c_str()	// semaphore name);
	);
}

template<class T>
SharedMem<T>::~SharedMem()
{
	UnmapViewOfFile(pSharedData);
	CloseHandle(hMapFile);
	CloseHandle(hSemaphore);
}

template<class T>
T* SharedMem<T>::GetMemoryPointer()
{
	return pSharedData;
}

template<class T>
bool SharedMem<T>::IsDataReady()
{
	if (hSemaphore == NULL) return false;
	DWORD dwWaitResult;
	dwWaitResult = WaitForSingleObject(hSemaphore, 0);
	//log_debug("[DBG][SharedMem] IsDataReady::dwWaitResult = %d",  dwWaitResult);
	switch (dwWaitResult) {
		// The semaphore was signaled, the data is ready
	case WAIT_OBJECT_0:
		// We release it again immediately since we don't need exclusive or atomic access
		// (there are no concurrent threads)
		ReleaseSemaphore(hSemaphore, 1, NULL);
		return true;
		break;

		// The semaphore was nonsignaled, so a time-out occurred.
		// This should only happen when the data is not ready to be accessed
		// (missing/disabled hook, not initialized or not generating data.
	case WAIT_TIMEOUT:
		return false;
		break;
	}
	return false;
}

template<class T>
void SharedMem<T>::SetDataReady()
{
	ReleaseSemaphore(hSemaphore, 1, NULL);
}