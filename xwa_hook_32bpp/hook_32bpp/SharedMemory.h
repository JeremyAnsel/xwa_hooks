#pragma once

class SharedMemoryWriter
{
public:
	SharedMemoryWriter();
	~SharedMemoryWriter();
	void Close();
	void Create(const wchar_t* name, unsigned int cbData);

	void* _handle;
	void* _lpData;
	unsigned int _cbData;
};

class SharedMemoryReader
{
public:
	SharedMemoryReader();
	~SharedMemoryReader();
	void Close();
	void Open(const wchar_t* name, unsigned int cbData);

	void* _handle;
	void* _lpData;
	unsigned int _cbData;
};
