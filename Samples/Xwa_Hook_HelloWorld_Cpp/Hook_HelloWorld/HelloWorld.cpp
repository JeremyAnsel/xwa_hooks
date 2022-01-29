#include "targetver.h"
#include "HelloWorld.h"
#include <string>

int HelloWorldHook(int* params)
{
	const auto XwaGetFrontTxtString = (const char* (*)(int))0x55CB50;

	char* bufferPtr = (char*)0xABD680;
	const int bufferSize = 256;

	const int FrontTxt_STR_CONFIG_VERSION = 1168;

	strcpy_s(bufferPtr, bufferSize, "Hello, World! ");
	strcat_s(bufferPtr, bufferSize, XwaGetFrontTxtString(FrontTxt_STR_CONFIG_VERSION));
	strcat_s(bufferPtr, bufferSize, "2.02");

	return 0;
}
