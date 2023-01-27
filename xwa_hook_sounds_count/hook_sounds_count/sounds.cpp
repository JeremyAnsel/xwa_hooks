#include "targetver.h"
#include "sounds.h"
#include "config.h"
#include <fstream>
#include <filesystem>
#include <tuple>

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

std::string GetPathFileName(const std::string& str)
{
	auto a = str.find_last_of('\\');

	return a == -1 ? str : str.substr(a + 1, -1);
}

std::string to_lower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), tolower);
	return str;
}

class Config
{
public:
	const int VanillaSoundsCount = 2872;

	Config()
	{
		//auto lines = GetFileLines("hook_sounds_count.cfg");

		//if (lines.empty())
		//{
		//	lines = GetFileLines("hooks.ini", "hook_sounds_count");
		//}

		//this->SoundsCount = GetFileKeyValueInt(lines, "SoundsCount", VanillaSoundsCount);

		LoadSoundEffectsLists();
	}

private:
	void LoadSoundEffectsLists()
	{
		std::vector<std::string> filenames = GetListFilenames();
		BuildListsConfig(filenames);
	}

	std::vector<std::string> GetListFilenames()
	{
		std::vector<std::string> filenames;

		for (const auto& file : std::filesystem::directory_iterator(".\\Wave"))
		{
			if (!std::filesystem::is_regular_file(file))
			{
				continue;
			}

			if (to_lower(file.path().extension().string()) != ".lst")
			{
				continue;
			}

			const auto& filename = file.path().filename().string();

			if (to_lower(filename).rfind("sfx_", 0) != 0)
			{
				continue;
			}

			filenames.push_back(file.path().string());
		}

		return filenames;
	}

	void BuildListsConfig(const std::vector<std::string>& filenames)
	{
		std::ofstream file("Hook_Sounds_Count.txt");

		if (!file)
		{
			return;
		}

		int currentIndex = VanillaSoundsCount;

		for (const std::string& filename : filenames)
		{
			int count = GetListLinesCount(filename);

			if (count)
			{
				std::string baseName = to_lower(GetStringWithoutExtension(GetPathFileName(filename)));

				file << "; " << filename << std::endl;
				file << baseName << "_index = " << currentIndex << std::endl;
				file << baseName << "_count = " << count << std::endl;

				this->SoundLists.push_back(std::make_pair(currentIndex, filename));
				currentIndex += count;
			}
		}

		file << "; SoundsCount = " << currentIndex << std::endl;

		this->SoundsCount = currentIndex;
	}

	int GetListLinesCount(const std::string& filename)
	{
		std::ifstream file(filename);

		if (!file)
		{
			return 0;
		}

		int count = 0;
		std::string line;

		while (std::getline(file, line))
		{
			line = Trim(line);

			if (!line.empty())
			{
				count++;
			}
		}

		return count;
	}

public:
	int SoundsCount;
	std::vector<std::tuple<int, std::string>> SoundLists;
};

Config& GetConfig()
{
	static Config g_config;
	return g_config;
}

//typedef char TSoundEffectName[32];
typedef char TSoundEffectName[64];

class BuffersArray
{
public:
	BuffersArray()
	{
		this->_soundEffectNamesBuffer.reserve(GetConfig().SoundsCount * sizeof(TSoundEffectName));
		this->_sfxVolumesBuffer.reserve(GetConfig().SoundsCount);
		this->_sfxDistancesBuffer.reserve(GetConfig().SoundsCount);
		this->_soundEffectIdsBuffer.reserve(GetConfig().SoundsCount);
		this->_soundEffectBuffer.reserve(GetConfig().SoundsCount * 201);

		*(int*)0x009D23E0 = this->SoundEffectNamesData();
		*(int*)0x005B3DA0 = this->SfxVolumesData();
		*(int*)0x005B3C10 = this->SfxDistancesData();
		*(int*)0x00917E80 = this->SoundEffectIdsData();
		*(int*)0x0091B850 = this->SoundEffectBufferData();
	}

	int SoundEffectNamesData()
	{
		return (int)this->_soundEffectNamesBuffer.data();
	}

	int SfxVolumesData()
	{
		return (int)this->_sfxVolumesBuffer.data();
	}

	int SfxDistancesData()
	{
		return (int)this->_sfxDistancesBuffer.data();
	}

	int SoundEffectIdsData()
	{
		return (int)this->_soundEffectIdsBuffer.data();
	}

	int SoundEffectBufferData()
	{
		return (int)this->_soundEffectBuffer.data();
	}

private:
	std::vector<unsigned char> _soundEffectNamesBuffer;
	std::vector<unsigned char> _sfxVolumesBuffer;
	std::vector<unsigned short> _sfxDistancesBuffer;
	std::vector<int> _soundEffectIdsBuffer;
	std::vector<unsigned char> _soundEffectBuffer;
};

BuffersArray& GetBuffersArray()
{
	static BuffersArray g_buffersArray;
	return g_buffersArray;
}

int GetSoundEffectNamesArrayCount()
{
	return GetConfig().SoundsCount;
}

int GetSoundEffectNamesArraySize()
{
	return GetConfig().SoundsCount * sizeof(TSoundEffectName);
}

int GetSoundEffectNamesArrayPtr()
{
	return GetBuffersArray().SoundEffectNamesData();
}

int GetSfxVolumesArrayCount()
{
	return GetConfig().SoundsCount;
}

int GetSfxVolumesArraySize()
{
	return GetConfig().SoundsCount;
}

int GetSfxVolumesArrayPtr()
{
	return GetBuffersArray().SfxVolumesData();
}

int GetSfxDistancesArrayCount()
{
	return GetConfig().SoundsCount;
}

int GetSfxDistancesArraySize()
{
	return GetConfig().SoundsCount * sizeof(unsigned short);
}

int GetSfxDistancesArrayPtr()
{
	return GetBuffersArray().SfxDistancesData();
}

int GetSoundEffectIdsArrayCount()
{
	return GetConfig().SoundsCount;
}

int GetSoundEffectIdsArraySize()
{
	return GetConfig().SoundsCount * sizeof(int);
}

int GetSoundEffectIdsArrayPtr()
{
	return GetBuffersArray().SoundEffectIdsData();
}

int GetSoundEffectBufferArrayCount()
{
	return GetConfig().SoundsCount;
}

int GetSoundEffectBufferArraySize()
{
	return GetConfig().SoundsCount * 201;
}

int GetSoundEffectBufferArrayPtr()
{
	return GetBuffersArray().SoundEffectBufferData();
}

int ResetSoundEffectNamesHook(int* params)
{
	int ptr = GetSoundEffectNamesArrayPtr();
	int size = GetSoundEffectNamesArraySize();

	memset((void*)ptr, 0, size);

	return 0;
}

int ResetSoundEffectIdsHook(int* params)
{
	int ptr = GetSoundEffectIdsArrayPtr();
	int size = GetSoundEffectIdsArraySize();

	memset((void*)ptr, 0xff, size);

	return 0;
}

int LoadEffectSoundsHook(int* params)
{
	const char* A4 = (const char*)params[0];
	const short A8 = (short)params[1];
	const char* AC = (const char*)params[2];

	const auto XwaLoadSfxLst = (short(*)(const char*, short, const char*))0x0043A150;

	XwaLoadSfxLst(A4, A8, AC);

	for (const auto& sound : GetConfig().SoundLists)
	{
		int index = std::get<0>(sound);
		std::string filename = std::get<1>(sound);
		std::string basename = GetStringWithoutExtension(GetPathFileName(filename));
		std::string directory = "Wave\\" + basename + "\\";

		XwaLoadSfxLst(filename.c_str(), (short)index, directory.c_str());
	}

	return 0;
}

int LoadEffectSoundHook(int* params)
{
	char* esp2C = (char*)params + 0x2C;

	params[Params_ESI] = (int)esp2C;
	params[Params_EAX] = *esp2C;

	for (; *esp2C; esp2C++)
	{
		if (*esp2C == ';')
		{
			*esp2C = 0;
			break;
		}
	}

	return 0;
}
