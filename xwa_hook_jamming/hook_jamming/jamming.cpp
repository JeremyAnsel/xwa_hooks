#include "targetver.h"
#include "jamming.h"
#include "config.h"
#include <fstream>
#include <map>

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

class Config
{
public:
	Config()
	{
		auto lines = GetFileLines("hook_jamming.cfg");

		if (lines.empty())
		{
			lines = GetFileLines("hooks.ini", "hook_jamming");
		}

		this->IsJammingEnabled = GetFileKeyValueInt(lines, "IsJammingEnabled", 1) != 0;
	}

	bool IsJammingEnabled;
};

Config g_config;

#pragma pack(push, 1)

struct ExeEnableEntry
{
	char unk000[4];
	unsigned int ObjectSize;
	char unk008[16];
};

static_assert(sizeof(ExeEnableEntry) == 24, "size of ExeEnableEntry must be 24");

struct ExeCraftJammingPoint
{
	int PositionX;
	int PositionY;
	int PositionZ;
};

static_assert(sizeof(ExeCraftJammingPoint) == 12, "size of ExeCraftJammingPoint must be 12");

struct ExeCraftEntry
{
	char unk0000[626];
	int InsideHangarX;
	int InsideHangarZ;
	int InsideHangarY;
	char unk027E[252];
	unsigned char NumOfJammingPoints;
	ExeCraftJammingPoint JammingPoints[8];
};

static_assert(sizeof(ExeCraftEntry) == 987, "size of ExeCraftEntry must be 987");

struct XwaCraft
{
	char unk000[4];
	unsigned short CraftIndex;
	char unk006[13];
	int m013[5];
	char unk027[344];
	unsigned short m17F;
	unsigned short m181;
	unsigned short m183;
	unsigned short m185;
	char unk187[98];
	short m1E9;
	char unk1EB[526];
};

static_assert(sizeof(XwaCraft) == 1017, "size of XwaCraft must be 1017");

struct XwaMobileObject
{
	char unk00[152];
	unsigned char Team;
	char unk99[45];
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
	char unkD9[4];
	XwaCraft* pCraft;
	char unkE1[4];
};

static_assert(sizeof(XwaMobileObject) == 229, "size of XwaMobileObject must be 229");

struct XwaObject
{
	short m00;
	unsigned short ModelIndex;
	unsigned char ShipCategory;
	unsigned char TieFlightGroupIndex;
	unsigned char Region;
	int PositionX;
	int PositionY;
	int PositionZ;
	short HeadingXY;
	short HeadingZ;
	short m17;
	short m19;
	short m1B;
	unsigned char m1D[2];
	int PlayerIndex;
	XwaMobileObject* pMobileObject;
};

static_assert(sizeof(XwaObject) == 39, "size of XwaObject must be 39");

#pragma pack(pop)

enum class JammingDirectionEnum
{
	All,
	Up,
	Down
};

enum class JammingBehaviorEnum
{
	None,
	HangarMeshCenter,
	InsideHangarHardpoint,
	InsideHangarHardpointAndJammingPoints
};

struct CraftJamming
{
	bool HasJamming;
	JammingDirectionEnum JammingDirection;
	int JammingDistance;
	JammingBehaviorEnum JammingBehavior;
};

CraftJamming GetCraftJamming(int modelIndex)
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

	std::string shipPath = g_flightModelsList.GetLstLine(modelIndex);

	const std::string objectValue = GetFileKeyValue(lines, shipPath + ".opt");

	if (!objectValue.empty() && std::ifstream(objectValue))
	{
		shipPath = GetStringWithoutExtension(objectValue);
	}

	lines = GetFileLines(shipPath + "Jamming.txt");

	if (!lines.size())
	{
		lines = GetFileLines(shipPath + ".ini", "Jamming");
	}

	CraftJamming jamming{};

	if (lines.size())
	{
		jamming.HasJamming = GetFileKeyValueInt(lines, "HasJamming", 1) != 0;

		std::string direction = GetFileKeyValue(lines, "JammingDirection");

		if (direction == "All")
		{
			jamming.JammingDirection = JammingDirectionEnum::All;
		}
		else if (direction == "Up")
		{
			jamming.JammingDirection = JammingDirectionEnum::Up;
		}
		else if (direction == "Down")
		{
			jamming.JammingDirection = JammingDirectionEnum::Down;
		}
		else
		{
			jamming.JammingDirection = JammingDirectionEnum::All;
		}

		jamming.JammingDistance = GetFileKeyValueInt(lines, "JammingDistance", -1);

		std::string behavior = GetFileKeyValue(lines, "JammingBehavior");

		if (behavior == "None")
		{
			jamming.JammingBehavior = JammingBehaviorEnum::None;
		}
		else if (behavior == "HangarMeshCenter")
		{
			jamming.JammingBehavior = JammingBehaviorEnum::HangarMeshCenter;
		}
		else if (behavior == "InsideHangarHardpoint")
		{
			jamming.JammingBehavior = JammingBehaviorEnum::InsideHangarHardpoint;
		}
		else if (behavior == "InsideHangarHardpointAndJammingPoints")
		{
			jamming.JammingBehavior = JammingBehaviorEnum::InsideHangarHardpointAndJammingPoints;
		}
		else
		{
			jamming.JammingBehavior = JammingBehaviorEnum::InsideHangarHardpointAndJammingPoints;
		}
	}
	else
	{
		switch (modelIndex)
		{
		case 178: // ModelIndex_178_0_127_ImpResearchCenter
			jamming.HasJamming = false;
			jamming.JammingDirection = JammingDirectionEnum::All;
			jamming.JammingDistance = -1;
			jamming.JammingBehavior = JammingBehaviorEnum::None;
			break;

		case 160: // ModelIndex_160_0_109_Factory
			jamming.HasJamming = true;
			jamming.JammingDirection = JammingDirectionEnum::All;
			jamming.JammingDistance = 4096;
			jamming.JammingBehavior = JammingBehaviorEnum::None;
			break;

		case 137: // ModelIndex_137_0_91_Interdictor2
		case 138: // ModelIndex_138_0_92_VictoryStarDestroyer2
		case 139: // ModelIndex_139_0_93_ImperialStarDestroyer2
			jamming.HasJamming = true;
			jamming.JammingDirection = JammingDirectionEnum::Down;
			jamming.JammingDistance = -1;
			jamming.JammingBehavior = JammingBehaviorEnum::InsideHangarHardpointAndJammingPoints;
			break;

		case 140: // ModelIndex_140_0_94_SuperStarDestroyer
			jamming.HasJamming = true;
			jamming.JammingDirection = JammingDirectionEnum::All;
			jamming.JammingDistance = -1;
			jamming.JammingBehavior = JammingBehaviorEnum::InsideHangarHardpoint;
			break;

		case 162: // ModelIndex_162_0_111_RepairYard
			jamming.HasJamming = true;
			jamming.JammingDirection = JammingDirectionEnum::All;
			jamming.JammingDistance = -1;
			jamming.JammingBehavior = JammingBehaviorEnum::HangarMeshCenter;
			break;

		default:
			jamming.HasJamming = true;
			jamming.JammingDirection = JammingDirectionEnum::All;
			jamming.JammingDistance = -1;
			jamming.JammingBehavior = JammingBehaviorEnum::InsideHangarHardpointAndJammingPoints;
			break;
		}
	}

	return jamming;
}

class ModelIndexJammings
{
public:
	CraftJamming GetJamming(int modelIndex)
	{
		this->Update();

		auto it = this->_jammings.find(modelIndex);

		if (it != this->_jammings.end())
		{
			return it->second;
		}
		else
		{
			auto value = GetCraftJamming(modelIndex);
			this->_jammings.insert(std::make_pair(modelIndex, value));
			return value;
		}
	}

private:
	void Update()
	{
		static std::string _mission;

		const char* xwaMissionFileName = (const char*)0x06002E8;

		if (_mission != xwaMissionFileName)
		{
			_mission = xwaMissionFileName;

			this->_jammings.clear();
		}
	}

	std::map<int, CraftJamming> _jammings;
};

ModelIndexJammings g_modelIndexJammings;

int HangarJammingHook(int* params)
{
	if (!g_config.IsJammingEnabled)
	{
		return 0;
	}

	const int A4 = params[0];
	const int A8 = params[1];

	const int MeshType_Hangar = 16;

	const auto XwaGetLengthEstimate = (int(*)(int, int, int))0x004121C0;
	const auto XwaGetCraftIndex = (unsigned short(*)(unsigned short))0x004DCE30;
	const auto XwaOptGetMeshesCount = (int(*)(unsigned short))0x00488960;
	const auto XwaModelGetMeshType = (int(*)(unsigned short, int))0x00488A00;
	const auto XwaOptGetMeshCenterX = (int(*)(unsigned short, int))0x00485DC0;
	const auto XwaOptGetMeshCenterY = (int(*)(unsigned short, int))0x00485E90;
	const auto XwaOptGetMeshCenterZ = (int(*)(unsigned short, int))0x00485F60;
	const auto L004A2FB0 = (void(*)(XwaObject*, int, int, int))0x004A2FB0;
	int& s_V0x080DA48 = *(int*)0x080DA48;
	int& s_V0x080DA44 = *(int*)0x080DA44;
	int& s_V0x080DB64 = *(int*)0x080DB64;

	XwaObject* s_XwaObjects = *(XwaObject**)0x07B33C4;
	ExeEnableEntry* s_ExeEnableTable = (ExeEnableEntry*)0x005FB240;
	ExeCraftEntry* s_ExeCraftTable = (ExeCraftEntry*)0x005BB480;

	const unsigned short A4_modelIndex = s_XwaObjects[A4].ModelIndex;
	const unsigned short A8_modelIndex = s_XwaObjects[A8].ModelIndex;

	int A8_objectSize = s_ExeEnableTable[s_XwaObjects[A8].ModelIndex].ObjectSize;

	int directionX = s_XwaObjects[A4].PositionX - s_XwaObjects[A8].PositionX;
	int directionY = s_XwaObjects[A4].PositionY - s_XwaObjects[A8].PositionY;
	int directionZ = s_XwaObjects[A4].PositionZ - s_XwaObjects[A8].PositionZ;

	int directionLength = XwaGetLengthEstimate(directionX, directionY, directionZ);

	if (directionLength > A8_objectSize * 2)
	{
		return 0;
	}

	if (s_XwaObjects[A8].pMobileObject == 0 || s_XwaObjects[A8].pMobileObject->pCraft == 0)
	{
		return 0;
	}

	if (s_XwaObjects[A8].pMobileObject->pCraft->m185 == 0)
	{
		return 0;
	}

	if (s_XwaObjects[A8].pMobileObject->pCraft->CraftIndex == 0xFFFF)
	{
		return 0;
	}

	const CraftJamming& jamming = g_modelIndexJammings.GetJamming(A8_modelIndex);

	if (!jamming.HasJamming)
	{
		return 0;
	}

	unsigned short A8_craftIndex = XwaGetCraftIndex(A8_modelIndex);

	if (A8_craftIndex == 0xFFFF)
	{
		return 0;
	}

	bool isJamming = false;

	if (jamming.JammingDistance != -1)
	{
		if (directionLength <= jamming.JammingDistance)
		{
			isJamming = true;
		}
	}
	else
	{
		if (s_XwaObjects[A8].pMobileObject->RecalculateTransformMatrix != 0)
		{
			const auto L0043FFB0 = (void(*)(short, short, XwaObject*))0x0043FFB0;
			const auto L00440140 = (void(*)(short, short, XwaObject*))0x00440140;

			L0043FFB0(s_XwaObjects[A8].HeadingZ, s_XwaObjects[A8].HeadingXY, &s_XwaObjects[A8]);
			L00440140(s_XwaObjects[A8].m17, s_XwaObjects[A8].m19, &s_XwaObjects[A8]);
		}

		XwaMobileObject* A8_pMobileObject = s_XwaObjects[A8].pMobileObject;

		int transformedX = (int)((long long)A8_pMobileObject->TransformMatrixRightX * directionX / 32768) + (int)((long long)A8_pMobileObject->TransformMatrixRightY * directionY / 32768) + (int)((long long)A8_pMobileObject->TransformMatrixRightZ * directionZ / 32768);
		int transformedY = (int)((long long)A8_pMobileObject->TransformMatrixFrontX * directionX / 32768) + (int)((long long)A8_pMobileObject->TransformMatrixFrontY * directionY / 32768) + (int)((long long)A8_pMobileObject->TransformMatrixFrontZ * directionZ / 32768);
		int transformedZ = (int)((long long)A8_pMobileObject->TransformMatrixUpX * directionX / 32768) + (int)((long long)A8_pMobileObject->TransformMatrixUpY * directionY / 32768) + (int)((long long)A8_pMobileObject->TransformMatrixUpZ * directionZ / 32768);

		int A8_InsideHangarY = s_ExeCraftTable[A8_craftIndex].InsideHangarY;
		int A8_InsideHangarX = s_ExeCraftTable[A8_craftIndex].InsideHangarX;
		int A8_InsideHangarZ = s_ExeCraftTable[A8_craftIndex].InsideHangarZ;

		switch (jamming.JammingDirection)
		{
		case JammingDirectionEnum::Up:
			if (transformedZ < 0)
			{
				return 0;
			}

			break;

		case JammingDirectionEnum::Down:
			if (transformedZ > 0)
			{
				return 0;
			}

			break;
		}

		switch (jamming.JammingBehavior)
		{
		case JammingBehaviorEnum::HangarMeshCenter:
		{
			int ecx = XwaGetLengthEstimate(transformedX - A8_InsideHangarX, transformedY - A8_InsideHangarY, transformedZ - A8_InsideHangarZ);
			int ebp18 = A8_objectSize / 6;

			if (ecx > ebp18)
			{
				int ebp1C = XwaOptGetMeshesCount(A8_modelIndex);

				for (int ebx = 0; ebx < ebp1C; ebx++)
				{
					if (XwaModelGetMeshType(A8_modelIndex, ebx) != MeshType_Hangar)
						continue;

					int ebp20 = XwaOptGetMeshCenterX(A8_modelIndex, ebx);
					int ebp04a = XwaOptGetMeshCenterZ(A8_modelIndex, ebx);
					int eax1 = -XwaOptGetMeshCenterY(A8_modelIndex, ebx);

					L004A2FB0(&s_XwaObjects[A8], ebp20, ebp04a, eax1);

					if (XwaGetLengthEstimate(transformedX - s_V0x080DA48, transformedY - s_V0x080DA44, transformedZ - s_V0x080DB64) < ebp18)
					{
						isJamming = true;
						break;
					}
				}
			}

			break;
		}

		case JammingBehaviorEnum::InsideHangarHardpoint:
		{
			int edi = std::abs(transformedZ - A8_InsideHangarZ);

			if (edi > std::abs(A8_InsideHangarZ) * 3 / 4)
			{
				return 0;
			}

			if (XwaGetLengthEstimate(transformedX - A8_InsideHangarX, transformedY - A8_InsideHangarY, edi) > A8_objectSize / 8)
			{
				return 0;
			}

			// TODO
			isJamming = true;
			break;
		}

		case JammingBehaviorEnum::InsideHangarHardpointAndJammingPoints:
		{
			if (s_ExeCraftTable[A8_craftIndex].NumOfJammingPoints == 0)
			{
				if (XwaGetLengthEstimate(transformedX - A8_InsideHangarX, transformedY - A8_InsideHangarY, transformedZ - A8_InsideHangarZ) > A8_objectSize / 6)
				{
					return 0;
				}

				isJamming = true;
			}
			else
			{
				int ecx1 = XwaGetLengthEstimate(transformedX - A8_InsideHangarX, transformedY - A8_InsideHangarY, transformedZ - A8_InsideHangarZ);
				int ebp18 = A8_objectSize / 6;

				if (ecx1 < ebp18)
				{
					isJamming = true;
					break;
				}

				for (int ebp14 = 0; ebp14 < s_ExeCraftTable[A8_craftIndex].NumOfJammingPoints; ebp14++)
				{
					L004A2FB0(
						&s_XwaObjects[A8],
						s_ExeCraftTable[A8_craftIndex].JammingPoints[ebp14].PositionX,
						s_ExeCraftTable[A8_craftIndex].JammingPoints[ebp14].PositionZ,
						s_ExeCraftTable[A8_craftIndex].JammingPoints[ebp14].PositionY);

					if (XwaGetLengthEstimate(transformedX - s_V0x080DA48, transformedY - s_V0x080DA44, transformedZ - s_V0x080DB64) < ebp18)
					{
						isJamming = true;
						break;
					}
				}
			}

			break;
		}
		}
	}

	if (isJamming)
	{
		if (s_XwaObjects[A4].pMobileObject != 0)
		{
			XwaCraft* pCraft = s_XwaObjects[A4].pMobileObject->pCraft;

			if (pCraft != 0)
			{
				pCraft->m013[2] = 0x28000;
				pCraft->m1E9 = 0;
			}
		}
	}

	return 0;
}
