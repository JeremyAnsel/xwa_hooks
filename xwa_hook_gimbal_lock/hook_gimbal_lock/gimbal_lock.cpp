#include "targetver.h"
#include "gimbal_lock.h"
#include "config.h"

//#define _XM_NO_INTRINSICS_
#define _XM_SSE_INTRINSICS_
#include <DirectXMath.h>

enum ParamsEnum
{
	Params_ReturnAddress = -1,
	Params_EAX = -3,
	Params_ECX = -4,
	Params_EDX = -5,
	Params_EBX = -6,
	Params_EBP = -8,
	Params_ESI = -9,
	Params_EDI = -10,
};

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hook_gimbal_lock.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_gimbal_lock");
		}

		this->GimbalLockFixEnabled = GetFileKeyValueInt(lines, "GimbalLockFixEnabled", 1) != 0;
	}

	bool GimbalLockFixEnabled;
};

Config g_config;

#pragma pack(push, 1)

struct XwaMobileObject
{
	char Unk0000[191];
	bool RecalculateForwardVector;
	short ForwardX;
	short ForwardY;
	short ForwardZ;
	bool RecalculateTransformMatrix;
	short TransformMatrixFrontX;
	short TransformMatrixFrontY;
	short TransformMatrixFrontZ;
	short TransformMatrixRightX;
	short TransformMatrixRightY;
	short TransformMatrixRightZ;
	short TransformMatrixUpX;
	short TransformMatrixUpY;
	short TransformMatrixUpZ;
	char Unk00D9[12];
};

static_assert(sizeof(XwaMobileObject) == 229, "size of XwaMobileObject must be 229");

struct XwaObject
{
	char Unk0000[2];
	short ModelIndex;
	unsigned char ShipCategory;
	unsigned char TieFlightGroupIndex;
	unsigned char Region;
	int PositionX;
	int PositionY;
	int PositionZ;
	short HeadingXY;
	short HeadingZ;
	short HeadingRoll;
	short XwaObject_m19;
	char Unk001B[4];
	int PlayerIndex;
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

#pragma pack(pop)

inline DirectX::XMVECTOR XM_CALLCONV ClampAngles(DirectX::FXMVECTOR v)
{
	return DirectX::XMVectorATan2(DirectX::XMVectorSin(v), DirectX::XMVectorCos(v));
}

void XwaAnglesToRad(short headingXY, short headingZ, short headingRoll, float* pitch, float* yaw, float* roll)
{
	DirectX::XMVECTOR heading = DirectX::XMVectorSet(
		-headingXY * DirectX::XM_PI / 32767.0f,
		-DirectX::XM_PIDIV2 - headingZ * DirectX::XM_PI / 32767.0f,
		-headingRoll * DirectX::XM_PI / 32767.0f,
		0.0f);

	heading = ClampAngles(heading);

	DirectX::XMFLOAT4 v;
	DirectX::XMStoreFloat4(&v, heading);
	*yaw = v.x;
	*pitch = v.y;
	*roll = v.z;
}

short RoundIntShort(float f)
{
	int v = (int)roundf(f);

	if (v > 32767)
	{
		v = 32767;
	}
	else if (v < -32767)
	{
		v = -32767;
	}

	return (short)v;
}

void RadAnglesToXwa(float pitch, float yaw, float roll, short* headingXY, short* headingZ, short* headingRoll)
{
	DirectX::XMVECTOR heading = DirectX::XMVectorSet(
		-yaw,
		-DirectX::XM_PIDIV2 - pitch,
		-roll,
		0.0f);

	heading = ClampAngles(heading);
	heading = DirectX::XMVectorScale(heading, 32767.0f / DirectX::XM_PI);

	DirectX::XMFLOAT4 v;
	DirectX::XMStoreFloat4(&v, heading);

	//*headingXY = (short)roundf(v.x);
	//*headingZ = (short)roundf(v.y);
	//*headingRoll = (short)roundf(v.z);

	*headingXY = RoundIntShort(v.x);
	*headingZ = RoundIntShort(v.y);
	*headingRoll = RoundIntShort(v.z);
}

inline DirectX::XMVECTOR XM_CALLCONV QuaternionRotationMatrix(DirectX::FXMMATRIX m)
{
	DirectX::XMVECTOR q = DirectX::XMQuaternionRotationMatrix(m);

	//if (DirectX::XMVectorGetW(q) < 0.0f)
	//{
	//	q = DirectX::XMVectorNegate(q);
	//}

	return q;
}

DirectX::XMVECTOR XM_CALLCONV QuaternionToEuler(DirectX::FXMVECTOR quat)
{
	DirectX::XMFLOAT4 q;
	DirectX::XMStoreFloat4(&q, quat);

	const float xx = q.x * q.x;
	const float yy = q.y * q.y;
	const float zz = q.z * q.z;

	const float m31 = 2.f * q.x * q.z + 2.f * q.y * q.w;
	const float m32 = 2.f * q.y * q.z - 2.f * q.x * q.w;
	const float m33 = 1.f - 2.f * xx - 2.f * yy;

	const float cy = sqrtf(m33 * m33 + m31 * m31);
	const float cx = atan2f(-m32, cy);

	DirectX::XMVECTOR v;

	if (cy > 0.00001f)
	{
		const float m12 = 2.f * q.x * q.y + 2.f * q.z * q.w;
		const float m22 = 1.f - 2.f * xx - 2.f * zz;

		v = DirectX::XMVectorSet(cx, atan2f(m31, m33), atan2f(m12, m22), 0.0f);
	}
	else
	{
		const float m11 = 1.f - 2.f * yy - 2.f * zz;
		const float m21 = 2.f * q.x * q.y - 2.f * q.z * q.w;

		v = DirectX::XMVectorSet(cx, 0.f, atan2f(-m21, m11), 0.0f);
	}

	v = ClampAngles(v);
	return v;
}

void ExtractEulerFromMatrix(short* pitch, short* yaw, short* roll, const DirectX::XMMATRIX& m)
{
	DirectX::XMVECTOR q = QuaternionRotationMatrix(m);
	DirectX::XMVECTOR euler = QuaternionToEuler(q);
	DirectX::XMFLOAT4 eulerV;
	DirectX::XMStoreFloat4(&eulerV, euler);

	RadAnglesToXwa(eulerV.x, eulerV.y, eulerV.z, yaw, pitch, roll);
}

int GimbalLockUserInputHook(int* params)
{
	const int A4 = params[0];
	const int A8 = params[1];
	const short AC = (short)params[2];
	const int A10 = params[3];

	const auto L005042F0 = (void(*)(int, int, short, int))0x005042F0;
	const auto XwaRecalculateForwardVector = (void(*)(short, short, XwaObject*))0x0043FFB0;
	const auto XwaRecalculateTransformMatrix = (void(*)(short, short, XwaObject*))0x00440140;

	if (!g_config.GimbalLockFixEnabled)
	{
		L005042F0(A4, A8, AC, A10);
		return 0;
	}

	float A4_angle = A4 * DirectX::XM_PI / 32767.0f;
	float A8_angle = -A8 * DirectX::XM_PI / 32767.0f;

	int objectIndex = AC;
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaObject* object = &XwaObjects[objectIndex];
	XwaMobileObject* mobile = object->pMobileObject;

	float f_pitch, f_yaw, f_roll;
	XwaAnglesToRad(object->HeadingXY, object->HeadingZ, object->HeadingRoll, &f_pitch, &f_yaw, &f_roll);

	DirectX::XMMATRIX m = DirectX::XMMatrixIdentity();
	m = DirectX::XMMatrixMultiply(m, DirectX::XMMatrixRotationAxis(m.r[1], f_yaw));
	m = DirectX::XMMatrixMultiply(m, DirectX::XMMatrixRotationAxis(m.r[0], f_pitch));
	m = DirectX::XMMatrixMultiply(m, DirectX::XMMatrixRotationAxis(m.r[2], f_roll));

	m = DirectX::XMMatrixMultiply(m, DirectX::XMMatrixRotationAxis(m.r[1], A8_angle));
	m = DirectX::XMMatrixMultiply(m, DirectX::XMMatrixRotationAxis(m.r[0], A4_angle));

	short pitch, yaw, roll;
	ExtractEulerFromMatrix(&pitch, &yaw, &roll, m);

	pitch = -pitch;
	yaw += 0x8000;
	roll += 0x8000;

	object->HeadingXY = yaw;
	object->HeadingZ = pitch;
	object->HeadingRoll = roll;
	object->XwaObject_m19 = 0;

	XwaRecalculateForwardVector(object->HeadingZ, object->HeadingXY, object);
	XwaRecalculateTransformMatrix(object->HeadingRoll, object->XwaObject_m19, object);

	return 0;
}
