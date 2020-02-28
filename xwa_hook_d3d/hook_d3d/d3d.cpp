#include "targetver.h"
#include "d3d.h"
#include <Windows.h>

#include <ddraw.h>
#include <d3d.h>
#include <string>
#include <vector>

#pragma pack(push, 1)

struct XwaD3dVertex
{
	float x;
	float y;
	float z;
	float rhw;
	unsigned int color;
	unsigned int specular;
	float tu;
	float tv;
};

static_assert(sizeof(XwaD3dVertex) == 32, "size of XwaD3dVertex must be 32");

struct XwaD3dTriangle
{
	int v1;
	int v2;
	int v3;
	int RenderStatesFlags;
	int pTexture;
};

static_assert(sizeof(XwaD3dTriangle) == 20, "size of XwaD3dTriangle must be 20");

#pragma pack(pop)

bool g_isInScene;
int g_lastVertexCount;
std::vector<XwaD3dVertex> g_vertexBuffer;
std::vector<XwaD3dTriangle> g_triangleBuffer;

int BeginSceneHook(int* params)
{
	g_isInScene = true;
	g_lastVertexCount = 0;
	g_vertexBuffer.clear();
	g_triangleBuffer.clear();

	return *(int*)0x007B1180;
}

int EndSceneHook(int* params)
{
	g_isInScene = false;

	const auto XwaD3dExecuteBufferLock = (char(*)())0x00595006;
	const auto XwaD3dExecuteBufferAddVertices = (char(*)(XwaD3dVertex*, int))0x00595095;
	const auto XwaD3dExecuteBufferProcessVertices = (char(*)())0x00595106;
	const auto XwaD3dExecuteBufferAddTriangles = (char(*)(XwaD3dTriangle*, int))0x00595191;
	const auto XwaD3dExecuteBufferUnlockAndExecute = (char(*)())0x005954D6;

	int& s_XwaD3dExecuteBufferSize = *(int*)0x007B1D24;
	D3DEXECUTEBUFFERDESC& s_XwaD3dExecuteBufferDesc = *(D3DEXECUTEBUFFERDESC*)0x007B1390;
	LPDIRECT3DEXECUTEBUFFER& s_XwaD3D_ExecuteBuffer = *(LPDIRECT3DEXECUTEBUFFER*)0x007B1D20;
	LPDIRECT3DDEVICE s_XwaD3D_D3DDevice = *(LPDIRECT3DDEVICE*)0x007B1180;

	int requiredExecuteBufferSize = g_vertexBuffer.size() * sizeof(XwaD3dVertex) + 4;

	if (requiredExecuteBufferSize > s_XwaD3dExecuteBufferSize || requiredExecuteBufferSize < s_XwaD3dExecuteBufferSize / 4)
	{
		s_XwaD3dExecuteBufferSize = requiredExecuteBufferSize;

		memset(&s_XwaD3dExecuteBufferDesc, 0, sizeof(D3DEXECUTEBUFFERDESC));
		s_XwaD3dExecuteBufferDesc.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
		s_XwaD3dExecuteBufferDesc.dwFlags = D3DDEB_BUFSIZE;
		s_XwaD3dExecuteBufferDesc.dwBufferSize = s_XwaD3dExecuteBufferSize;

		s_XwaD3D_ExecuteBuffer->Release();
		s_XwaD3D_ExecuteBuffer = nullptr;
		s_XwaD3D_D3DDevice->CreateExecuteBuffer(&s_XwaD3dExecuteBufferDesc, &s_XwaD3D_ExecuteBuffer, nullptr);
	}

	XwaD3dExecuteBufferLock();
	XwaD3dExecuteBufferAddVertices(g_vertexBuffer.data(), g_vertexBuffer.size());
	XwaD3dExecuteBufferProcessVertices();
	XwaD3dExecuteBufferAddTriangles(g_triangleBuffer.data(), g_triangleBuffer.size());
	XwaD3dExecuteBufferUnlockAndExecute();

	return *(int*)0x007B1180;
}

int ExecuteBufferLockHook(int* params)
{
	if (g_isInScene)
	{
		return 0;
	}

	*(int*)0x007B1D28 = 0;
	return 1;
}

int ExecuteBufferAddVerticesHook(int* params)
{
	const XwaD3dVertex* buffer = (XwaD3dVertex*)params[2];
	const int count = params[3];

	if (g_isInScene)
	{
		*(int*)0x009EAA00 += count;

		g_lastVertexCount = g_vertexBuffer.size();
		g_vertexBuffer.reserve(count);

		for (int i = 0; i < count; i++)
		{
			g_vertexBuffer.push_back(buffer[i]);
		}

		return 0;
	}

	const D3DEXECUTEBUFFERDESC* s_XwaD3dExecuteBufferDesc = (D3DEXECUTEBUFFERDESC*)0x007B1390;
	void*& s_XwaD3dExecuteBufferDataPtr = *(void**)0x007B138C;
	void*& s_XwaD3dExecuteBufferCurrentPtr = *(void**)0x007B1CCC;

	memcpy(s_XwaD3dExecuteBufferDesc->lpData, buffer, count * sizeof(XwaD3dVertex));

	s_XwaD3dExecuteBufferDataPtr = (void*)s_XwaD3dExecuteBufferDesc->dwCaps;
	s_XwaD3dExecuteBufferCurrentPtr = s_XwaD3dExecuteBufferDataPtr;

	return 1;
}

int ExecuteBufferProcessVerticesHook(int* params)
{
	if (g_isInScene)
	{
		return 0;
	}

	//*(int*)0x007B1CD0 = *(int*)0x007B1CCC;
	//return 1;

	return 0;
}

int ExecuteBufferAddTrianglesHook(int* params)
{
	const XwaD3dTriangle* buffer = (XwaD3dTriangle*)params[10];
	const int count = params[11];

	if (g_isInScene)
	{
		*(int*)0x009EA96C += count;

		g_triangleBuffer.reserve(count);

		for (int i = 0; i < count; i++)
		{
			XwaD3dTriangle triangle = buffer[i];
			triangle.v1 += g_lastVertexCount;
			triangle.v2 += g_lastVertexCount;
			triangle.v3 += g_lastVertexCount;

			g_triangleBuffer.push_back(triangle);
		}

		return 0;
	}

	const int s_XwaD3dVerticesCount = *(int*)0x007B1D28;
	const D3DEXECUTEBUFFERDESC* s_XwaD3dExecuteBufferDesc = (D3DEXECUTEBUFFERDESC*)0x007B1390;
	void*& s_XwaD3dExecuteBufferCurrentPtr = *(void**)0x007B1CCC;

	int* indexBuffer = (int*)s_XwaD3dExecuteBufferDesc->dwFlags;

	for (int i = 0; i < count; i++)
	{
		const XwaD3dTriangle* triangle = buffer + i;

		*indexBuffer++ = triangle->v1;
		*indexBuffer++ = triangle->v2;
		*indexBuffer++ = triangle->v3;
	}

	s_XwaD3dExecuteBufferCurrentPtr = (void*)((int)s_XwaD3dExecuteBufferDesc->lpData + s_XwaD3dVerticesCount * sizeof(XwaD3dVertex));

	return 1;
}

int ExecuteBufferUnlockAndExecuteHook(int* params)
{
	if (g_isInScene)
	{
		return 0;
	}

	const int s_XwaD3dVerticesCount = *(int*)0x007B1D28;
	const D3DEXECUTEBUFFERDESC* s_XwaD3dExecuteBufferDesc = (D3DEXECUTEBUFFERDESC*)0x007B1390;
	void*& s_XwaD3dExecuteBufferDataPtr = *(void**)0x007B138C;
	void*& s_XwaD3dExecuteBufferCurrentPtr = *(void**)0x007B1CCC;
	void*& s_XwaD3dExecuteBufferInstructionsPtr = *(void**)0x007B1CD0;

	s_XwaD3dExecuteBufferDataPtr = s_XwaD3dExecuteBufferDesc->lpData;
	s_XwaD3dExecuteBufferInstructionsPtr = (void*)((int)s_XwaD3dExecuteBufferDataPtr + s_XwaD3dVerticesCount * sizeof(XwaD3dVertex));

	(*(char**)0x007B1CCC)[0] = 0x0B;
	(*(char**)0x007B1CCC)[1] = 0x00;
	return 1;
}
