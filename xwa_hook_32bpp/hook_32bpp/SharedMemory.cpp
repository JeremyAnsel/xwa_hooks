#include "targetver.h"
#include "SharedMemory.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

SharedMemoryWriter::SharedMemoryWriter()
{
	_handle = nullptr;
	_lpData = nullptr;
	_cbData = 0;
}

SharedMemoryWriter::~SharedMemoryWriter()
{
	Close();
}

void SharedMemoryWriter::Close()
{
	_cbData = 0;

	if (_lpData)
	{
		UnmapViewOfFile(_lpData);
		_lpData = nullptr;
	}

	if (_handle)
	{
		CloseHandle(_handle);
		_handle = nullptr;
	}
}

void SharedMemoryWriter::Create(const wchar_t* name, unsigned int cbData)
{
	Close();

	HANDLE hMapFile = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, cbData, name);

	if (!hMapFile)
	{
		return;
	}

	void* pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, cbData);

	if (!pBuf)
	{
		CloseHandle(hMapFile);
		return;
	}

	_handle = hMapFile;
	_lpData = pBuf;
	_cbData = cbData;
}

SharedMemoryReader::SharedMemoryReader()
{
	_handle = nullptr;
	_lpData = nullptr;
	_cbData = 0;
}

SharedMemoryReader::~SharedMemoryReader()
{
	Close();
}

void SharedMemoryReader::Close()
{
	_cbData = 0;

	if (_lpData)
	{
		UnmapViewOfFile(_lpData);
		_lpData = nullptr;
	}

	if (_handle)
	{
		CloseHandle(_handle);
		_handle = nullptr;
	}
}

void SharedMemoryReader::Open(const wchar_t* name, unsigned int cbData)
{
	Close();

	HANDLE hMapFile = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, name);

	if (!hMapFile)
	{
		return;
	}

	void* pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, cbData);

	if (!pBuf)
	{
		CloseHandle(hMapFile);
		return;
	}

	_handle = hMapFile;
	_lpData = pBuf;
	_cbData = cbData;
}
