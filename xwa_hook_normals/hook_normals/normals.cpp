#include "targetver.h"
#include "normals.h"
#include "config.h"
#include <vector>

#pragma pack(push, 1)

struct XwaVector3
{
	float x;
	float y;
	float z;
};

static_assert(sizeof(XwaVector3) == 12, "size of XwaVector3 must be 12");

struct XwaMatrix3x3
{
	float _11;
	float _12;
	float _13;
	float _21;
	float _22;
	float _23;
	float _31;
	float _32;
	float _33;
};

static_assert(sizeof(XwaMatrix3x3) == 36, "size of XwaMatrix3x3 must be 36");

struct XwaRotationAxis
{
	float x;
	float y;
	float z;
	float angle;
};

static_assert(sizeof(XwaRotationAxis) == 16, "size of XwaRotationAxis must be 16");

struct SceneVertList
{
	XwaVector3 Position;
	float ColorIntensity;
	float ColorA;
	float ColorR;
	float ColorG;
	float ColorB;
	float TextureU;
	float TextureV;
	float TextureVertices[32];
	int TextureVerticesCount;
};

static_assert(sizeof(SceneVertList) == 172, "size of SceneVertList must be 172");

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

struct XwaDataMesh
{
	XwaD3dVertex Vertices[384];
	int VerticesCount;
	char Triangles[7680];
	int TrianglesCount;
	XwaDataMesh* Next;
};

static_assert(sizeof(XwaDataMesh) == 19980, "size of XwaDataMesh must be 19980");

struct XwaTransform
{
	XwaVector3 Position;
	XwaMatrix3x3 Rotation;
};

static_assert(sizeof(XwaTransform) == 48, "size of XwaTransform must be 48");

struct SceneCompData
{
	char m000[8];
	XwaTransform ViewTransform;
	char m038[228];
};

static_assert(sizeof(SceneCompData) == 284, "size of SceneCompData must be 284");

#pragma pack(pop)

class Config
{
public:
	Config()
	{
		const auto lines = GetFileLines("hook_normals.cfg");

		this->Output = GetFileKeyValueInt(lines, "Output");
	}

	int Output;
};

Config g_config;

std::vector<XwaVector3> g_vertexNormals;

unsigned int ClampColor(float v)
{
	int c = (int)v;

	if (c < 0)
	{
		c = 0;
	}
	else if (c > 255)
	{
		c = 255;
	}

	return (unsigned int)c;
}

unsigned int VectorToColor(XwaVector3& vector)
{
	unsigned int r = ClampColor(vector.z);
	unsigned int g = ClampColor(vector.y);
	unsigned int b = ClampColor(vector.x);

	return 0xff000000 | (r << 16) | (g << 8) | b;
}

int SetSceneVertListsHook(int* params)
{
	const int s_XwaSceneVerticesCount = *(int*)0x009B6D06;
	int& s_XwaSceneVertListsCount = *(int*)0x009B6CC6;

	s_XwaSceneVertListsCount = s_XwaSceneVerticesCount * 4;

	if (g_vertexNormals.capacity() < (unsigned int)s_XwaSceneVertListsCount)
	{
		g_vertexNormals.reserve(s_XwaSceneVertListsCount);
	}

	return 0;
}

int SetVertexNormalHook(int* params)
{
	const auto L00442480 = (void(*)(SceneCompData*, SceneVertList*, const XwaVector3*, const XwaVector3*, const XwaVector3*))0x00442480;
	const auto XwaVector3Transform = (void(*)(XwaVector3*, XwaMatrix3x3*))0x00439B30;

	SceneCompData* A4 = (SceneCompData*)params[0];
	SceneVertList* A8 = (SceneVertList*)params[1];
	const XwaVector3* AC = (XwaVector3*)params[2];
	const XwaVector3* A10 = (XwaVector3*)params[3];
	const XwaVector3* A14 = (XwaVector3*)params[4];

	const SceneVertList* s_XwaSceneVertLists = *(SceneVertList**)0x009B6CBC;

	L00442480(A4, A8, AC, A10, A14);

	int index = A8 - s_XwaSceneVertLists;

	XwaVector3 n = *AC;

	XwaVector3Transform(&n, &A4->ViewTransform.Rotation);

	float length = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
	if (length != 0)
	{
		n.x /= length;
		n.y /= length;
		n.z /= length;
	}

	switch (g_config.Output)
	{
	case 0:
	default:
		break;

	case 1:
		n.y = n.z = n.x;
		break;

	case 2:
		n.x = n.z = n.y;
		break;

	case 3:
		n.x = n.y = n.z;
		break;
	}

	g_vertexNormals.data()[index] = n;

	//A8->ColorA = 0.0f;
	//A8->ColorR = (AC->z * 0.5f + 0.5f);
	//A8->ColorG = (AC->y * 0.5f + 0.5f);
	//A8->ColorB = (AC->x * 0.5f + 0.5f);

	return 0;
}

int SetSpecularSolidHook(int* params)
{
	const auto L004487E0 = (int(*)(int, const SceneVertList*))0x004487E0;

	const int vertIndex = params[0];
	const SceneVertList* vertArray = (SceneVertList*)params[1];

	const SceneVertList* s_XwaSceneVertLists = *(SceneVertList**)0x009B6CBC;
	XwaD3dVertex* buffer = *(XwaD3dVertex**)0x0064D1A8;
	int bufferIndex = L004487E0(vertIndex, vertArray);

	int index = vertArray + vertIndex - s_XwaSceneVertLists;
	XwaVector3 normal = g_vertexNormals.data()[index];

	normal.x = (normal.x * 0.5f + 0.5f) * 255.0f;
	normal.y = (normal.y * 0.5f + 0.5f) * 255.0f;
	normal.z = (normal.z * 0.5f + 0.5f) * 255.0f;

	buffer[bufferIndex].specular = VectorToColor(normal);

	return bufferIndex;
}

int SetSpecularTransparentHook(int* params)
{
	const auto L004489C0 = (int(*)(XwaDataMesh*, int, const SceneVertList*))0x004489C0;

	XwaDataMesh* dataMesh = (XwaDataMesh*)params[0];
	const int vertIndex = params[1];
	const SceneVertList* vertArray = (SceneVertList*)params[2];

	const SceneVertList* s_XwaSceneVertLists = *(SceneVertList**)0x009B6CBC;
	int bufferIndex = L004489C0(dataMesh, vertIndex, vertArray);

	int index = vertArray + vertIndex - s_XwaSceneVertLists;
	XwaVector3 normal = g_vertexNormals.data()[index];

	normal.x = (normal.x * 0.5f + 0.5f) * 255.0f;
	normal.y = (normal.y * 0.5f + 0.5f) * 255.0f;
	normal.z = (normal.z * 0.5f + 0.5f) * 255.0f;

	dataMesh->Vertices[bufferIndex].specular = VectorToColor(normal);

	return bufferIndex;
}

int CopyVertHook(int* params)
{
	SceneVertList* eax = (SceneVertList*)params[0];
	SceneVertList* ecx = (SceneVertList*)params[1];

	const SceneVertList* s_XwaSceneVertLists = *(SceneVertList**)0x009B6CBC;

	eax->ColorA = ecx->ColorA;
	eax->ColorR = ecx->ColorR;
	eax->ColorG = ecx->ColorG;
	eax->ColorB = ecx->ColorB;

	int eaxIndex = eax - s_XwaSceneVertLists;
	int ecxIndex = ecx - s_XwaSceneVertLists;
	g_vertexNormals.data()[eaxIndex] = g_vertexNormals.data()[ecxIndex];

	return 0;
}

int L004477E0Hook(int* params)
{
	const auto L004477E0 = (void(*)(int, int, SceneVertList*))0x004477E0;

	const int index0 = params[0];
	const int index1 = params[1];
	SceneVertList* vertList = (SceneVertList*)params[2];
	SceneVertList* vert0 = &vertList[index0];
	SceneVertList* vert1 = &vertList[index1];

	const SceneVertList* s_XwaSceneVertLists = *(SceneVertList**)0x009B6CBC;
	int sceneIndex0 = vertList + index0 - s_XwaSceneVertLists;
	int sceneIndex1 = vertList + index1 - s_XwaSceneVertLists;

	L004477E0(index0, index1, vertList);

	float z0 = vert0->Position.z;
	float z1 = vert1->Position.z;

	if (z0 < 0.0f)
	{
		if (z1 >= 0.0f)
		{
			int esp18 = *(int*)0x0686AB4 - 1;
			SceneVertList* vert18 = &vertList[esp18];
			int sceneIndex18 = vertList + esp18 - s_XwaSceneVertLists;

			XwaVector3 vec0 = g_vertexNormals.data()[sceneIndex0];
			XwaVector3 vec1 = g_vertexNormals.data()[sceneIndex1];
			XwaVector3& vec18 = g_vertexNormals.data()[sceneIndex18];

			// AC[esp18].ColorA = AC[A4].ColorA - AC[A4].Position.z / (- AC[A4].Position.z + s_V0x08B94CC / AC[A8].Position.z - 1.0f) * (AC[A8].ColorA - AC[A4].ColorA
			float s_V0x08B94CC = *(float*)0x08B94CC;
			float t = z0 / (-z0 + s_V0x08B94CC / z1 - 1.0f);
			vec18.x = vec0.x - t * (vec1.x - vec0.x);
			vec18.y = vec0.y - t * (vec1.y - vec0.y);
			vec18.z = vec0.z - t * (vec1.z - vec0.z);
		}
	}
	else if (z1 < 0.0f)
	{
		int esp18 = *(int*)0x0686AB4 - 1;
		SceneVertList* vert18 = &vertList[esp18];
		int sceneIndex18 = vertList + esp18 - s_XwaSceneVertLists;

		XwaVector3 vec0 = g_vertexNormals.data()[sceneIndex0];
		XwaVector3 vec1 = g_vertexNormals.data()[sceneIndex1];
		XwaVector3& vec18 = g_vertexNormals.data()[sceneIndex18];

		// AC[esp18].ColorA = AC[A8].ColorA - AC[A8].Position.z / (AC[A8].Position.z - s_V0x08B94CC / AC[A4].Position.z + 1.0f) * (AC[A8].ColorA - AC[A4].ColorA);
		float s_V0x08B94CC = *(float*)0x08B94CC;
		float t = z1 / (z1 - s_V0x08B94CC / z0 + 1.0f);
		vec18.x = vec1.x - t * (vec1.x - vec0.x);
		vec18.y = vec1.y - t * (vec1.y - vec0.y);
		vec18.z = vec1.z - t * (vec1.z - vec0.z);
	}

	return 0;
}

int L00444030Hook(int* params)
{
	const auto L00444030 = (void(*)(int, int, SceneVertList*))0x00444030;

	const int index0 = params[0];
	const int index1 = params[1];
	SceneVertList* vertList = (SceneVertList*)params[2];
	SceneVertList* vert0 = &vertList[index0];
	SceneVertList* vert1 = &vertList[index1];

	const SceneVertList* s_XwaSceneVertLists = *(SceneVertList**)0x009B6CBC;
	int sceneIndex0 = vertList + index0 - s_XwaSceneVertLists;
	int sceneIndex1 = vertList + index1 - s_XwaSceneVertLists;

	L00444030(index0, index1, vertList);

	float y0 = vert0->Position.y;
	float y1 = vert1->Position.y;

	if (y0 < 0.0f)
	{
		if (y1 >= 0.0f)
		{
			int esp18 = *(int*)0x0686AB4 - 1;
			SceneVertList* vert18 = &vertList[esp18];
			int sceneIndex18 = vertList + esp18 - s_XwaSceneVertLists;

			XwaVector3 vec0 = g_vertexNormals.data()[sceneIndex0];
			XwaVector3 vec1 = g_vertexNormals.data()[sceneIndex1];
			XwaVector3& vec18 = g_vertexNormals.data()[sceneIndex18];

			if (-y0 < y1)
			{
				// AC[esp18].ColorA = AC[A4].ColorA - AC[A4].Position.y / (AC[A8].Position.y - AC[A4].Position.y) * (AC[A8].ColorA - AC[A4].ColorA);
				float t = y0 / (y1 - y0);
				vec18.x = vec0.x - t * (vec1.x - vec0.x);
				vec18.y = vec0.y - t * (vec1.y - vec0.y);
				vec18.z = vec0.z - t * (vec1.z - vec0.z);
			}
			else
			{
				// AC[esp18].ColorA = AC[A8].ColorA - AC[A8].Position.y / (AC[A8].Position.y - AC[A4].Position.y) * (AC[A8].ColorA - AC[A4].ColorA);
				float t = y1 / (y1 - y0);
				vec18.x = vec1.x - t * (vec1.x - vec0.x);
				vec18.y = vec1.y - t * (vec1.y - vec0.y);
				vec18.z = vec1.z - t * (vec1.z - vec0.z);
			}
		}
	}
	else if (y1 < 0.0f)
	{
		int esp18 = *(int*)0x0686AB4 - 1;
		SceneVertList* vert18 = &vertList[esp18];
		int sceneIndex18 = vertList + esp18 - s_XwaSceneVertLists;

		XwaVector3 vec0 = g_vertexNormals.data()[sceneIndex0];
		XwaVector3 vec1 = g_vertexNormals.data()[sceneIndex1];
		XwaVector3& vec18 = g_vertexNormals.data()[sceneIndex18];

		if (y0 < -y1)
		{
			// AC[esp18].ColorA = AC[A4].ColorA - AC[A4].Position.y / (AC[A8].Position.y - AC[A4].Position.y) * (AC[A8].ColorA - AC[A4].ColorA);
			float t = y0 / (y1 - y0);
			vec18.x = vec0.x - t * (vec1.x - vec0.x);
			vec18.y = vec0.y - t * (vec1.y - vec0.y);
			vec18.z = vec0.z - t * (vec1.z - vec0.z);
		}
		else
		{
			// AC[esp18].ColorA = AC[A8].ColorA - AC[A8].Position.y / (AC[A8].Position.y - AC[A4].Position.y) * (AC[A8].ColorA - AC[A4].ColorA);
			float t = y1 / (y1 - y0);
			vec18.x = vec1.x - t * (vec1.x - vec0.x);
			vec18.y = vec1.y - t * (vec1.y - vec0.y);
			vec18.z = vec1.z - t * (vec1.z - vec0.z);
		}
	}

	return 0;
}

int L00444DF0Hook(int* params)
{
	const auto L00444DF0 = (void(*)(int, int, SceneVertList*))0x00444DF0;

	const int index0 = params[0];
	const int index1 = params[1];
	SceneVertList* vertList = (SceneVertList*)params[2];
	SceneVertList* vert0 = &vertList[index0];
	SceneVertList* vert1 = &vertList[index1];

	const SceneVertList* s_XwaSceneVertLists = *(SceneVertList**)0x009B6CBC;
	int sceneIndex0 = vertList + index0 - s_XwaSceneVertLists;
	int sceneIndex1 = vertList + index1 - s_XwaSceneVertLists;

	L00444DF0(index0, index1, vertList);

	float esp10 = (float)*(unsigned short*)0x07FFB60;
	float y0 = vert0->Position.y;
	float y1 = vert1->Position.y;

	if (y0 > esp10)
	{
		if (y1 <= esp10)
		{
			int esp18 = *(int*)0x0686AB4 - 1;
			SceneVertList* vert18 = &vertList[esp18];
			int sceneIndex18 = vertList + esp18 - s_XwaSceneVertLists;

			XwaVector3 vec0 = g_vertexNormals.data()[sceneIndex0];
			XwaVector3 vec1 = g_vertexNormals.data()[sceneIndex1];
			XwaVector3& vec18 = g_vertexNormals.data()[sceneIndex18];

			if (y0 - esp10 < esp10 - y1)
			{
				// AC[esp18].ColorA = AC[A4].ColorA - (AC[A4].Position.y - esp10) / (AC[A8].Position.y - AC[A4].Position.y) * (AC[A8].ColorA - AC[A4].ColorA);
				float t = (y0 - esp10) / (y1 - y0);
				vec18.x = vec0.x - t * (vec1.x - vec0.x);
				vec18.y = vec0.y - t * (vec1.y - vec0.y);
				vec18.z = vec0.z - t * (vec1.z - vec0.z);
			}
			else
			{
				// AC[esp18].ColorA = AC[A8].ColorA - (AC[A8].Position.y - esp10) / (AC[A8].Position.y - AC[A4].Position.y) * (AC[A8].ColorA - AC[A4].ColorA);
				float t = (y1 - esp10) / (y1 - y0);
				vec18.x = vec1.x - t * (vec1.x - vec0.x);
				vec18.y = vec1.y - t * (vec1.y - vec0.y);
				vec18.z = vec1.z - t * (vec1.z - vec0.z);
			}
		}
	}
	else if (y1 > esp10)
	{
		int esp18 = *(int*)0x0686AB4 - 1;
		SceneVertList* vert18 = &vertList[esp18];
		int sceneIndex18 = vertList + esp18 - s_XwaSceneVertLists;

		XwaVector3 vec0 = g_vertexNormals.data()[sceneIndex0];
		XwaVector3 vec1 = g_vertexNormals.data()[sceneIndex1];
		XwaVector3& vec18 = g_vertexNormals.data()[sceneIndex18];

		if (esp10 - y0 < y1 - esp10)
		{
			// AC[esp18].ColorA = AC[A4].ColorA - (AC[A4].Position.y - esp10) / (AC[A8].Position.y - AC[A4].Position.y) * (AC[A8].ColorA - AC[A4].ColorA);
			float t = (y0 - esp10) / (y1 - y0);
			vec18.x = vec0.x - t * (vec1.x - vec0.x);
			vec18.y = vec0.y - t * (vec1.y - vec0.y);
			vec18.z = vec0.z - t * (vec1.z - vec0.z);
		}
		else
		{
			// AC[esp18].ColorA = AC[A8].ColorA - (AC[A8].Position.y - esp10) / (AC[A8].Position.y - AC[A4].Position.y) * (AC[A8].ColorA - AC[A4].ColorA);
			float t = (y1 - esp10) / (y1 - y0);
			vec18.x = vec1.x - t * (vec1.x - vec0.x);
			vec18.y = vec1.y - t * (vec1.y - vec0.y);
			vec18.z = vec1.z - t * (vec1.z - vec0.z);
		}
	}

	return 0;
}

int L00445C00Hook(int* params)
{
	const auto L00445C00 = (void(*)(int, int, SceneVertList*))0x00445C00;

	const int index0 = params[0];
	const int index1 = params[1];
	SceneVertList* vertList = (SceneVertList*)params[2];
	SceneVertList* vert0 = &vertList[index0];
	SceneVertList* vert1 = &vertList[index1];

	const SceneVertList* s_XwaSceneVertLists = *(SceneVertList**)0x009B6CBC;
	int sceneIndex0 = vertList + index0 - s_XwaSceneVertLists;
	int sceneIndex1 = vertList + index1 - s_XwaSceneVertLists;

	L00445C00(index0, index1, vertList);

	float x0 = vert0->Position.x;
	float x1 = vert1->Position.x;

	if (x0 < 0.0f)
	{
		if (x1 >= 0.0f)
		{
			int esp18 = *(int*)0x0686AB4 - 1;
			SceneVertList* vert18 = &vertList[esp18];
			int sceneIndex18 = vertList + esp18 - s_XwaSceneVertLists;

			XwaVector3 vec0 = g_vertexNormals.data()[sceneIndex0];
			XwaVector3 vec1 = g_vertexNormals.data()[sceneIndex1];
			XwaVector3& vec18 = g_vertexNormals.data()[sceneIndex18];

			if (-x0 < x1)
			{
				// AC[esp18].ColorA = AC[A4].ColorA - AC[A4].Position.x / (AC[A8].Position.x - AC[A4].Position.x) * (AC[A8].ColorA - AC[A4].ColorA);
				float t = x0 / (x1 - x0);
				vec18.x = vec0.x - t * (vec1.x - vec0.x);
				vec18.y = vec0.y - t * (vec1.y - vec0.y);
				vec18.z = vec0.z - t * (vec1.z - vec0.z);
			}
			else
			{
				// AC[esp18].ColorA = AC[A8].ColorA - AC[A8].Position.x / (AC[A8].Position.x - AC[A4].Position.x) * (AC[A8].ColorA - AC[A4].ColorA);
				float t = x1 / (x1 - x0);
				vec18.x = vec1.x - t * (vec1.x - vec0.x);
				vec18.y = vec1.y - t * (vec1.y - vec0.y);
				vec18.z = vec1.z - t * (vec1.z - vec0.z);
			}
		}
	}
	else if (x1 < 0.0f)
	{
		int esp18 = *(int*)0x0686AB4 - 1;
		SceneVertList* vert18 = &vertList[esp18];
		int sceneIndex18 = vertList + esp18 - s_XwaSceneVertLists;

		XwaVector3 vec0 = g_vertexNormals.data()[sceneIndex0];
		XwaVector3 vec1 = g_vertexNormals.data()[sceneIndex1];
		XwaVector3& vec18 = g_vertexNormals.data()[sceneIndex18];

		if (x0 < -x1)
		{
			// AC[esp18].ColorA = AC[A4].ColorA - AC[A4].Position.x / (AC[A8].Position.x - AC[A4].Position.x) * (AC[A8].ColorA - AC[A4].ColorA);
			float t = x0 / (x1 - x0);
			vec18.x = vec0.x - t * (vec1.x - vec0.x);
			vec18.y = vec0.y - t * (vec1.y - vec0.y);
			vec18.z = vec0.z - t * (vec1.z - vec0.z);
		}
		else
		{
			// AC[esp18].ColorA = AC[A8].ColorA - AC[A8].Position.x / (AC[A8].Position.x - AC[A4].Position.x) * (AC[A8].ColorA - AC[A4].ColorA);
			float t = x1 / (x1 - x0);
			vec18.x = vec1.x - t * (vec1.x - vec0.x);
			vec18.y = vec1.y - t * (vec1.y - vec0.y);
			vec18.z = vec1.z - t * (vec1.z - vec0.z);
		}
	}

	return 0;
}

int L004469D0Hook(int* params)
{
	const auto L004469D0 = (void(*)(int, int, SceneVertList*))0x004469D0;

	const int index0 = params[0];
	const int index1 = params[1];
	SceneVertList* vertList = (SceneVertList*)params[2];
	SceneVertList* vert0 = &vertList[index0];
	SceneVertList* vert1 = &vertList[index1];

	const SceneVertList* s_XwaSceneVertLists = *(SceneVertList**)0x009B6CBC;
	int sceneIndex0 = vertList + index0 - s_XwaSceneVertLists;
	int sceneIndex1 = vertList + index1 - s_XwaSceneVertLists;

	L004469D0(index0, index1, vertList);

	float esp10 = (float)*(unsigned short*)0x07FBB72;
	float x0 = vert0->Position.x;
	float x1 = vert1->Position.x;

	if (x0 > esp10)
	{
		if (x1 <= esp10)
		{
			int esp18 = *(int*)0x0686AB4 - 1;
			SceneVertList* vert18 = &vertList[esp18];
			int sceneIndex18 = vertList + esp18 - s_XwaSceneVertLists;

			XwaVector3 vec0 = g_vertexNormals.data()[sceneIndex0];
			XwaVector3 vec1 = g_vertexNormals.data()[sceneIndex1];
			XwaVector3& vec18 = g_vertexNormals.data()[sceneIndex18];

			if (x0 - esp10 < esp10 - x1)
			{
				// AC[esp18].ColorA = AC[A4].ColorA - (AC[A4].Position.x - esp10) / (AC[A8].Position.x - AC[A4].Position.x) * (AC[A8].ColorA - AC[A4].ColorA);
				float t = (x0 - esp10) / (x1 - x0);
				vec18.x = vec0.x - t * (vec1.x - vec0.x);
				vec18.y = vec0.y - t * (vec1.y - vec0.y);
				vec18.z = vec0.z - t * (vec1.z - vec0.z);
			}
			else
			{
				// AC[esp18].ColorA = AC[A8].ColorA - (AC[A8].Position.x - esp10) / (AC[A8].Position.x - AC[A4].Position.x) * (AC[A8].ColorA - AC[A4].ColorA);
				float t = (x1 - esp10) / (x1 - x0);
				vec18.x = vec1.x - t * (vec1.x - vec0.x);
				vec18.y = vec1.y - t * (vec1.y - vec0.y);
				vec18.z = vec1.z - t * (vec1.z - vec0.z);
			}
		}
	}
	else if (x1 > esp10)
	{
		int esp18 = *(int*)0x0686AB4 - 1;
		SceneVertList* vert18 = &vertList[esp18];
		int sceneIndex18 = vertList + esp18 - s_XwaSceneVertLists;

		XwaVector3 vec0 = g_vertexNormals.data()[sceneIndex0];
		XwaVector3 vec1 = g_vertexNormals.data()[sceneIndex1];
		XwaVector3& vec18 = g_vertexNormals.data()[sceneIndex18];

		if (esp10 - x0 < x1 - esp10)
		{
			// AC[esp18].ColorA = AC[A4].ColorA - (AC[A4].Position.x - esp10) / (AC[A8].Position.x - AC[A4].Position.x) * (AC[A8].ColorA - AC[A4].ColorA);
			float t = (x0 - esp10) / (x1 - x0);
			vec18.x = vec0.x - t * (vec1.x - vec0.x);
			vec18.y = vec0.y - t * (vec1.y - vec0.y);
			vec18.z = vec0.z - t * (vec1.z - vec0.z);
		}
		else
		{
			// AC[esp18].ColorA = AC[A8].ColorA - (AC[A8].Position.x - esp10) / (AC[A8].Position.x - AC[A4].Position.x) * (AC[A8].ColorA - AC[A4].ColorA);
			float t = (x1 - esp10) / (x1 - x0);
			vec18.x = vec1.x - t * (vec1.x - vec0.x);
			vec18.y = vec1.y - t * (vec1.y - vec0.y);
			vec18.z = vec1.z - t * (vec1.z - vec0.z);
		}
	}

	return 0;
}
