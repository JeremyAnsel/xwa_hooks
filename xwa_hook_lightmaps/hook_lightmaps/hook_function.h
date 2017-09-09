#pragma once

struct HookFunction
{
	int from;
	int(*function)(int* params);
};

struct HookPatch
{
	int offset;
	int size;
	const void *original;
	const void *original_alternative; // in case of already patched binary
	const void *patched;
	int calladdr_pos;
	unsigned calladdr;
};