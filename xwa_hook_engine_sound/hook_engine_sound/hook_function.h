#pragma once

struct HookFunction
{
	int from;
	int(*function)(int* params);
};
