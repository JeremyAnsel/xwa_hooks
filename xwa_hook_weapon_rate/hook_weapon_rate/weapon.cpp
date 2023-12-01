#include "targetver.h"
#include "weapon.h"
#include "config.h"
#include <fstream>
#include <map>
#include <utility>

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

class FlightModelsList
{
public:
	FlightModelsList()
	{
		for (const auto& line : GetFileLines("FlightModels\\Spacecraft0.LST"))
		{
			this->_spacecraftList.push_back(GetStringWithoutExtension(line));
		}

		for (const auto& line : GetFileLines("FlightModels\\Equipment0.LST"))
		{
			this->_equipmentList.push_back(GetStringWithoutExtension(line));
		}
	}

	std::string GetLstLine(int modelIndex)
	{
		const int xwaObjectStats = 0x05FB240;
		const int dataIndex1 = *(short*)(xwaObjectStats + modelIndex * 0x18 + 0x14);
		const int dataIndex2 = *(short*)(xwaObjectStats + modelIndex * 0x18 + 0x16);

		switch (dataIndex1)
		{
		case 0:
			if ((unsigned int)dataIndex2 < this->_spacecraftList.size())
			{
				return this->_spacecraftList[dataIndex2];
			}

			break;

		case 1:
			if ((unsigned int)dataIndex2 < this->_equipmentList.size())
			{
				return this->_equipmentList[dataIndex2];
			}

			break;
		}

		return std::string();
	}

private:
	std::vector<std::string> _spacecraftList;
	std::vector<std::string> _equipmentList;
};

FlightModelsList g_flightModelsList;

class CraftConfig
{
public:
	CraftConfig()
	{
		auto lines = GetFileLines("hook_opt_limit.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_opt_limit");
		}

		this->MeshesCount = GetFileKeyValueInt(lines, "MeshesCount", 0);
		this->Craft_Size = 0x3F9 + GetFileKeyValueInt(lines, "Craft_ExtraSize", 0);
		this->Craft_Offset_22E = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_22E", 0);
		this->Craft_Offset_260 = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_260", 0);
		this->Craft_Offset_292 = 0x3F9 + GetFileKeyValueInt(lines, "Craft_Offset_292", 0);
	}

	int MeshesCount;
	int Craft_Size;
	int Craft_Offset_22E;
	int Craft_Offset_260;
	int Craft_Offset_292;
};

CraftConfig g_craftConfig;

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hook_weapon_rate.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_weapon_rate");
		}

		this->EnableFireMeshFilter = GetFileKeyValueInt(lines, "EnableFireMeshFilter", 1) != 0;
	}

	bool EnableFireMeshFilter;
};

Config g_config;

#pragma pack(push, 1)

enum ShipCategoryEnum : unsigned char
{
	ShipCategory_Starfighter = 0,
	ShipCategory_Transport = 1,
	ShipCategory_UtilityVehicle = 2,
	ShipCategory_Freighter = 3,
	ShipCategory_Starship = 4,
	ShipCategory_Platform = 5,
	ShipCategory_PlayerProjectile = 6,
	ShipCategory_OtherProjectile = 7,
	ShipCategory_Mine = 8,
	ShipCategory_Satellite = 9,
	ShipCategory_NormalDebris = 10,
	ShipCategory_SmallDebris = 11,
	ShipCategory_Backdrop = 12,
	ShipCategory_Explosion = 13,
	ShipCategory_Obstacle = 14,
	ShipCategory_DeathStarII = 15,
	ShipCategory_People = 16,
	ShipCategory_Container = 17,
	ShipCategory_Droid = 18,
	ShipCategory_Armament = 19,
	ShipCategory_LargeDebris = 20,
	ShipCategory_SalvageYard = 21,
};

enum MeshTypeEnum : unsigned int
{
	MeshType_Default = 0,
	MeshType_MainHull = 1,
	MeshType_Wing = 2,
	MeshType_Fuselage = 3,
	MeshType_GunTurret = 4,
	MeshType_SmallGun = 5,
	MeshType_Engine = 6,
	MeshType_Bridge = 7,
	MeshType_ShieldGen = 8,
	MeshType_EnergyGen = 9,
	MeshType_Launcher = 10,
	MeshType_CommSys = 11,
	MeshType_BeamSys = 12,
	MeshType_CommandSys = 13,
	MeshType_DockingPlat = 14,
	MeshType_LandingPlat = 15,
	MeshType_Hangar = 16,
	MeshType_CargoPod = 17,
	MeshType_MiscHull = 18,
	MeshType_Antenna = 19,
	MeshType_RotaryWing = 20,
	MeshType_RotaryGunTurret = 21,
	MeshType_RotaryLauncher = 22,
	MeshType_RotaryCommSys = 23,
	MeshType_RotaryBeamSys = 24,
	MeshType_RotaryCommandSys = 25,
	MeshType_Hatch = 26,
	MeshType_Custom = 27,
	MeshType_WeaponSys1 = 28,
	MeshType_WeaponSys2 = 29,
	MeshType_PowerReg = 30,
	MeshType_Reactor = 31,
};

struct OptRotationScaleNode_17_Data
{
	float PivotX;
	float PivotY;
	float PivotZ;
	float LookX;
	float LookY;
	float LookZ;
	float UpX;
	float UpY;
	float UpZ;
	float RightX;
	float RightY;
	float RightZ;
};

static_assert(sizeof(OptRotationScaleNode_17_Data) == 48, "size of OptRotationScaleNode_17_Data must be 48");

struct OptMeshDescriptorNode_19_Data
{
	MeshTypeEnum MeshType;
	unsigned int ExplosionType; // flags
	float SpanX;
	float SpanY;
	float SpanZ;
	float CenterX;
	float CenterY;
	float CenterZ;
	float MinX;
	float MinY;
	float MinZ;
	float MaxX;
	float MaxY;
	float MaxZ;
	int TargetId;
	float TargetX;
	float TargetY;
	float TargetZ;
};

static_assert(sizeof(OptMeshDescriptorNode_19_Data) == 72, "size of OptMeshDescriptorNode_19_Data must be 72");

struct ExeCraftWeaponSlot
{
	short PositionX;
	short PositionZ;
	short PositionY;
	unsigned char HardpointId;
	unsigned char MeshId;
};

static_assert(sizeof(ExeCraftWeaponSlot) == 8, "size of ExeCraftWeaponSlot must be 8");

struct ExeCraftEntry
{
	char Unk0000[354];
	ExeCraftWeaponSlot WeaponSlots[16];
	char Unk01E2[310];
	unsigned char NumOfFloatHardPts;
	char Unk0319[194];
};

static_assert(sizeof(ExeCraftEntry) == 987, "size of ExeCraftEntry must be 987");

struct XwaFloatHardPt
{
	int Position[3];
	unsigned char MeshId;
};

static_assert(sizeof(XwaFloatHardPt) == 13, "size of XwaFloatHardPt must be 13");

struct XwaCraftWeaponRack
{
	char Unk0000[2];
	char Sequence;
	char Unk0003[1];
	char Charge;
	char Unk0005[3];
	unsigned char MeshId;
	unsigned char HardpointId;
	char Unk000A[4];
};

static_assert(sizeof(XwaCraftWeaponRack) == 14, "size of XwaCraftWeaponRack must be 14");

struct XwaCraft
{
	char Unk0000[4];
	short CraftIndex;
	char Unk0006[424];
	char WeaponRacksCount;
	char Unk01AF[16];
	short m1BF[3];
	int m1C5[3];
	char Unk01D1[93];
	char XwaCraft_m22E[50];
	char XwaCraft_m260[50];
	char XwaCraft_m292[50];
	char Unk02C4[27];
	XwaCraftWeaponRack WeaponRacks[16];
	char Unk03BF[58];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

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
	ShipCategoryEnum ShipCategory;
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

std::vector<std::string> GetShipLines(int modelIndex)
{
	const char* xwaMissionFileName = (const char*)0x06002E8;

	const std::string mission = GetStringWithoutExtension(xwaMissionFileName);
	std::vector<std::string> lines = GetFileLines(mission + "_Objects.txt");

	if (!lines.size())
	{
		lines = GetFileLines(mission + ".ini", "Objects");
	}

	if (!lines.size())
	{
		lines = GetFileLines("FlightModels\\Objects.txt");
	}

	if (!lines.size())
	{
		lines = GetFileLines("FlightModels\\default.ini", "Objects");
	}

	std::string ship = g_flightModelsList.GetLstLine(modelIndex);

	const std::string objectValue = GetFileKeyValue(lines, ship + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		ship = GetStringWithoutExtension(objectValue);
	}

	lines = GetFileLines("FlightModels\\WeaponRate.txt");

	if (!lines.size())
	{
		lines = GetFileLines("FlightModels\\default.ini", "WeaponRate");
	}

	if (!lines.size())
	{
		lines = GetFileLines(ship + "WeaponRate.txt");
	}

	if (!lines.size())
	{
		lines = GetFileLines(ship + ".ini", "WeaponRate");
	}

	return lines;
}

int GetWeaponDechargeRate(int modelIndex)
{
	auto lines = GetShipLines(modelIndex);

	int rate = GetFileKeyValueInt(lines, "DechargeRate", -1);

	if (rate == -1)
	{
		const int xwaObjectStats = 0x05FB240;
		const char category = *(char*)(xwaObjectStats + modelIndex * 0x18 + 0x03);

		// ShipCategory_Starfighter
		if (category == 0)
		{
			switch (modelIndex)
			{
			case 5: // ModelIndex_005_0_4_TieFighter
			case 7: // ModelIndex_007_0_6_TieBomber
				rate = 3;
				break;

			default:
				rate = 4;
				break;
			}
		}
		else
		{
			rate = 3;
		}
	}

	return rate;
}

int GetWeaponRechargeRate(int modelIndex)
{
	auto lines = GetShipLines(modelIndex);

	int rate = GetFileKeyValueInt(lines, "RechargeRate", -1);

	if (rate == -1)
	{
		switch (modelIndex)
		{
		case 5: // ModelIndex_005_0_4_TieFighter
		case 7: // ModelIndex_007_0_6_TieBomber
			rate = 3;
			break;

		default:
			rate = 2;
			break;
		}
	}

	return rate;
}

int GetWeaponCooldownTime(int modelIndex)
{
	auto lines = GetShipLines(modelIndex);

	int rate = GetFileKeyValueInt(lines, "CooldownTimeFactor", 0x2F);

	return rate;
}

class ModelIndexWeapon
{
public:
	int GetDechargeRate(int modelIndex)
	{
		this->Update();

		auto it = this->_weaponDechargeRate.find(modelIndex);

		if (it != this->_weaponDechargeRate.end())
		{
			return it->second;
		}
		else
		{
			int value = GetWeaponDechargeRate(modelIndex);
			this->_weaponDechargeRate.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	int GetRechargeRate(int modelIndex)
	{
		this->Update();

		auto it = this->_weaponRechargeRate.find(modelIndex);

		if (it != this->_weaponRechargeRate.end())
		{
			return it->second;
		}
		else
		{
			int value = GetWeaponRechargeRate(modelIndex);
			this->_weaponRechargeRate.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

	int GetCooldownTime(int modelIndex)
	{
		this->Update();

		auto it = this->_weaponCooldownTime.find(modelIndex);

		if (it != this->_weaponCooldownTime.end())
		{
			return it->second;
		}
		else
		{
			int value = GetWeaponCooldownTime(modelIndex);
			this->_weaponCooldownTime.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

private:
	void Update()
	{
		static std::string _mission;
		static int _missionIndex = 0;

		const char* xwaMissionFileName = (const char*)0x06002E8;
		const int missionFileNameIndex = *(int*)0x06002E4;

		if (missionFileNameIndex == 0 ? (_mission != xwaMissionFileName) : (_missionIndex != missionFileNameIndex))
		{
			_mission = xwaMissionFileName;
			_missionIndex = missionFileNameIndex;

			this->_weaponDechargeRate.clear();
			this->_weaponRechargeRate.clear();
			this->_weaponCooldownTime.clear();
		}
	}

	std::map<int, int> _weaponDechargeRate;
	std::map<int, int> _weaponRechargeRate;
	std::map<int, int> _weaponCooldownTime;
};

ModelIndexWeapon g_modelIndexWeapon;

int WeaponDechargeHook(int* params)
{
	const int objectIndex = params[12];
	const int weaponRackIndex = (unsigned short)params[14];

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	if (XwaCurrentCraft->WeaponRacks[weaponRackIndex].Sequence < 4)
	{
		unsigned short modelIndex = XwaObjects[objectIndex].ModelIndex;
		int dechargeRate = g_modelIndexWeapon.GetDechargeRate(modelIndex);

		XwaCurrentCraft->WeaponRacks[weaponRackIndex].Charge -= dechargeRate;
	}

	return 0;
}

int WeaponRechargeHook(int* params)
{
	unsigned short modelIndex = (unsigned short)params[0];

	int rechargeRate = g_modelIndexWeapon.GetRechargeRate(modelIndex);
	params[0] = rechargeRate;

	return 0;
}

int WeaponCooldownTimeHook(int* params)
{
	const int esp10 = params[4];
	const int A4 = params[12];
	const int A8 = params[13];
	const int AC = params[Params_EBX];

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	unsigned short modelIndex = XwaObjects[A4].ModelIndex;
	int rate = g_modelIndexWeapon.GetCooldownTime(modelIndex);

	if (AC != -1)
	{
		XwaCurrentCraft->m1BF[0] += esp10 * rate + 0x02;
		XwaCurrentCraft->m1C5[0] += esp10 * rate + 0x02;
	}
	else
	{
		XwaCurrentCraft->m1BF[A8] += esp10 * rate + 0x02;
		XwaCurrentCraft->m1C5[A8] += esp10 * rate + 0x02;
	}

	return 0;
}

int WeaponRackHook(int* params)
{
	const auto L00491EB0 = (int(*)(int, int, int, int))0x00491EB0;

	const int A4 = params[0];
	const int A8 = params[1];
	const int AC = params[2];
	const int A10 = params[3];

	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	ShipCategoryEnum category = XwaObjects[A4].ShipCategory;

	if (category == ShipCategory_Freighter
		|| category == ShipCategory_Starship
		|| category == ShipCategory_Platform
		|| category == ShipCategory_Satellite)
	{
		char* m292;

		if (g_craftConfig.MeshesCount == 0)
		{
			m292 = XwaCurrentCraft->XwaCraft_m292;
		}
		else
		{
			m292 = (char*)((int)XwaCurrentCraft + g_craftConfig.Craft_Offset_292);
		}

		if (m292[XwaCurrentCraft->WeaponRacks[A8].MeshId] == 0)
		{
			return 0xffff;
		}
	}

	int result = L00491EB0(A4, A8, AC, A10);

	return result;
}

bool IsMeshTypeGunTurret(MeshTypeEnum meshType)
{
	switch (meshType)
	{
	case MeshType_GunTurret:
	case MeshType_RotaryGunTurret:
	case MeshType_SmallGun:
	case MeshType_WeaponSys1:
	case MeshType_WeaponSys2:
		return true;
	}

	return false;
}

bool IsMeshGunTurret(int modelIndex, int meshIndex)
{
	const auto XwaOptGetMeshType = (MeshTypeEnum(*)(int, int))0x00485A70;

	MeshTypeEnum meshType = XwaOptGetMeshType(modelIndex, meshIndex);
	return IsMeshTypeGunTurret(meshType);
}

bool HasOptGunTurrets(int modelIndex)
{
	const auto XwaOptGetMeshesCount = (int(*)(int))0x00485190;

	bool hasGunTurrets = false;
	int meshesCount = XwaOptGetMeshesCount(modelIndex);

	for (int i = 0; i < meshesCount; i++)
	{
		if (IsMeshGunTurret(modelIndex, i))
		{
			hasGunTurrets = true;
			break;
		}
	}

	return hasGunTurrets;
}

bool IsHardpointInFront(int modelIndex, XwaFloatHardPt* hardPts, int hardHpIndex)
{
	const auto XwaOptGetMeshRotationScale = (OptRotationScaleNode_17_Data * (*)(int, int))0x00486C30;

	float hpPositionX = hardPts[hardHpIndex].Position[0];
	float hpPositionY = hardPts[hardHpIndex].Position[2];
	float hpPositionZ = hardPts[hardHpIndex].Position[1];
	int hpMeshIndex = hardPts[hardHpIndex].MeshId;

	if (!IsMeshGunTurret(modelIndex, hpMeshIndex))
	{
		return true;
	}

	float targetPositionX = *(int*)0x080DA48;
	float targetPositionY = *(int*)0x080DA44;
	float targetPositionZ = *(int*)0x080DB64;

	float offsetX = targetPositionX - hpPositionX;
	float offsetY = targetPositionY - hpPositionY;
	float offsetZ = targetPositionZ - hpPositionZ;
	float offsetLength = sqrtf(offsetX * offsetX + offsetY * offsetY + offsetZ * offsetZ);
	if (offsetLength != 0.0f)
	{
		offsetX /= offsetLength;
		offsetY /= offsetLength;
		offsetZ /= offsetLength;
	}

	OptRotationScaleNode_17_Data* rotation = XwaOptGetMeshRotationScale(modelIndex, hpMeshIndex);
	float lookX = rotation->LookX;
	float lookY = rotation->LookY;
	float lookZ = rotation->LookZ;
	float lookLength = sqrtf(lookX * lookX + lookY * lookY + lookZ * lookZ);
	if (lookLength != 0.0f)
	{
		lookX /= lookLength;
		lookY /= lookLength;
		lookZ /= lookLength;
	}

	float dot = offsetX * lookX + offsetY * lookY + offsetZ * lookZ;
	bool inFront = dot > 0.0f;

	return inFront;
}

int WeaponHardpointHook(int* params)
{
	int A4 = params[0];
	int A8 = params[1];
	int AC = params[2];
	int A10 = params[3];
	int A14 = params[4];
	int* A18 = (int*)params[5];
	XwaFloatHardPt* A1C = (XwaFloatHardPt*)params[6];
	XwaCraft* A20 = (XwaCraft*)params[7];

	//const auto L00487FD0 = (int(*)(int, int, int, int, int, int*, XwaFloatHardPt*, XwaCraft*))0x00487FD0;

	//return L00487FD0(A4, A8, AC, A10, A14, A18, A1C, A20);

	const auto XwaOptGetMeshesCount = (int(*)(int))0x00488960;

	ExeCraftEntry* s_ExeCraftTable = (ExeCraftEntry*)0x005BB480;

	char* m292;

	if (g_craftConfig.MeshesCount == 0)
	{
		m292 = A20->XwaCraft_m292;
	}
	else
	{
		m292 = (char*)((int)A20 + g_craftConfig.Craft_Offset_292);
	}

	int esi = -1;

	float esp18 = (float)A8;
	float esp1C = (float)AC;
	float esp20 = (float)A10;

	int ebp = s_ExeCraftTable[A20->CraftIndex].NumOfFloatHardPts;

	int edx = A14;
	if (edx >= ebp)
	{
		edx = ebp - 1;
	}

	unsigned char esp24[256];

	memset(esp24, 0, ebp);

	for (int ecx = 0; ecx < edx; ecx++)
	{
		esp24[A18[ecx]] = 0x01;
	}

	esi = 0;

	for (; esi < ebp; esi++)
	{
		if (m292[A1C[esi].MeshId] != 0 && esp24[esi] == 0 && IsHardpointInFront(A4, A1C, esi))
			break;
	}

	if (esi == ebp)
	{
		return -1;
	}

	float esp14 =
		(A1C[esi].Position[0] - esp18) * (A1C[esi].Position[0] - esp18)
		+ (A1C[esi].Position[2] - esp1C) * (A1C[esi].Position[2] - esp1C)
		+ (A1C[esi].Position[1] - esp20) * (A1C[esi].Position[1] - esp20);

	for (int edx = esi + 1; edx < ebp; edx++)
	{
		if (m292[A1C[edx].MeshId] != 0 && esp24[edx] == 0 && IsHardpointInFront(A4, A1C, edx))
		{
			float esp10 =
				(A1C[edx].Position[0] - esp18) * (A1C[edx].Position[0] - esp18)
				+ (A1C[edx].Position[2] - esp1C) * (A1C[edx].Position[2] - esp1C)
				+ (A1C[edx].Position[1] - esp20) * (A1C[edx].Position[1] - esp20);

			if (esp10 < esp14)
			{
				esp14 = esp10;
				esi = edx;
			}
		}
	}

	return esi;
}

int GetNearestWeaponMeshHook(int* params)
{
	const int modelIndex = params[0];
	const int hardpointPositionX = params[1];
	const int hardpointPositionY = params[2];
	const int hardpointPositionZ = params[3];

	const auto XwaOptGetNearestWeaponSysOrMainHull = (int(*)(int, int, int, int))0x00487680;

	bool hasGunTurrets = HasOptGunTurrets(modelIndex);

	if (!hasGunTurrets)
	{
		return XwaOptGetNearestWeaponSysOrMainHull(modelIndex, hardpointPositionX, hardpointPositionY, hardpointPositionZ);
	}

	const auto XwaOptGetMeshesCount = (int(*)(int))0x00485190;
	const auto XwaOptGetMeshDescriptor = (OptMeshDescriptorNode_19_Data * (*)(int, int))0x004859B0;
	const auto XwaOptGetMeshHardpointsCount = (int(*)(int, int))0x00486DC0;
	const auto XwaOptRetrieveMeshHardpoint = (void(*)(int, int, int, int*, int*, int*, int*))0x00487180;

	float minDistance = 2.14748365E+09f;
	float distanceX = (float)hardpointPositionX;
	float distanceY = (float)hardpointPositionY;
	float distanceZ = (float)hardpointPositionZ;

	int selectedMeshIndex = -1;

	int meshesCount = XwaOptGetMeshesCount(modelIndex);

	for (int meshIndex = 0; meshIndex < meshesCount; meshIndex++)
	{
		if (!IsMeshGunTurret(modelIndex, meshIndex))
		{
			continue;
		}

		OptMeshDescriptorNode_19_Data* meshDescriptor = XwaOptGetMeshDescriptor(modelIndex, meshIndex);

		if (meshDescriptor == nullptr)
		{
			continue;
		}

		int hardpointsCount = XwaOptGetMeshHardpointsCount(modelIndex, meshIndex);

		for (int hardpointIndex = 0; hardpointIndex < hardpointsCount; hardpointIndex++)
		{
			int hpType = 0, hpPositionX = 0, hpPositionY = 0, hpPositionZ = 0;
			XwaOptRetrieveMeshHardpoint(modelIndex, meshIndex, hardpointIndex, &hpType, &hpPositionX, &hpPositionY, &hpPositionZ);
			hpPositionY = -hpPositionY;

			if (hardpointPositionX == hpPositionX
				&& hardpointPositionY == hpPositionY
				&& hardpointPositionZ == hpPositionZ)
			{
				return meshIndex;
			}
		}

		float offsetX;

		if (distanceX > meshDescriptor->MaxX)
		{
			offsetX = distanceX - meshDescriptor->MaxX;
		}
		else if (distanceX < meshDescriptor->MinX)
		{
			offsetX = meshDescriptor->MinX - distanceX;
		}
		else
		{
			offsetX = 0.0f;
		}

		float offsetY;

		if (distanceY > meshDescriptor->MaxY)
		{
			offsetY = distanceY - meshDescriptor->MaxY;
		}
		else if (distanceY < meshDescriptor->MinY)
		{
			offsetY = meshDescriptor->MinY - distanceY;
		}
		else
		{
			offsetY = 0.0f;
		}

		float offsetZ;

		if (distanceZ > meshDescriptor->MaxZ)
		{
			offsetZ = distanceZ - meshDescriptor->MaxZ;
		}
		else if (distanceZ < meshDescriptor->MinZ)
		{
			offsetZ = meshDescriptor->MinZ - distanceZ;
		}
		else
		{
			offsetZ = 0.0f;
		}

		float offsetDistance = offsetX * offsetX + offsetY * offsetY + offsetZ * offsetZ;

		if (offsetDistance < minDistance)
		{
			minDistance = offsetDistance;
			selectedMeshIndex = meshIndex;

			if (minDistance == 0.0f)
			{
				break;
			}
		}
	}

	return selectedMeshIndex;
}

int MirrorHardpointHook(int* params)
{
	int edi = params[Params_EDI];
	MeshTypeEnum meshType = (MeshTypeEnum)params[18];

	if (edi >= 0x100 || IsMeshTypeGunTurret(meshType))
	{
		params[Params_ReturnAddress] = 0x00433045;
	}

	return 0;
}

int PlayerFireHook(int* params)
{
	params[Params_EBX] = 0xffff;

	if (!g_config.EnableFireMeshFilter)
	{
		return 0;
	}

	const int objectIndex = params[54];
	const int weaponSlotIndex = params[55];

	ExeCraftEntry* s_ExeCraftTable = (ExeCraftEntry*)0x005BB480;
	XwaObject* XwaObjects = *(XwaObject**)0x007B33C4;
	XwaCraft* XwaCurrentCraft = *(XwaCraft**)0x00910DFC;

	char* m292;

	if (g_craftConfig.MeshesCount == 0)
	{
		m292 = XwaCurrentCraft->XwaCraft_m292;
	}
	else
	{
		m292 = (char*)((int)XwaCurrentCraft + g_craftConfig.Craft_Offset_292);
	}

	unsigned char meshId = s_ExeCraftTable[XwaCurrentCraft->CraftIndex].WeaponSlots[weaponSlotIndex].MeshId;

	if (m292[meshId] == 0)
	{
		params[Params_ReturnAddress] = 0x00492F12;
		return 0;
	}

	return 0;
}
