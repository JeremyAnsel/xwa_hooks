#include "targetver.h"
#include "music.h"
#include "xaudio2.h"
#include "ComPtr.h"

struct StreamingVoiceContext : public IXAudio2VoiceCallback
{
	STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32) override
	{
	}

	STDMETHOD_(void, OnVoiceProcessingPassEnd)() override
	{
	}

	STDMETHOD_(void, OnStreamEnd)() override
	{
	}

	STDMETHOD_(void, OnBufferStart)(void*) override
	{
	}

	STDMETHOD_(void, OnBufferEnd)(void*) override
	{
		SetEvent(hBufferEndEvent);
	}

	STDMETHOD_(void, OnLoopEnd)(void*) override
	{
	}

	STDMETHOD_(void, OnVoiceError)(void*, HRESULT) override
	{
	}

	HANDLE hBufferEndEvent;

	StreamingVoiceContext() :
		hBufferEndEvent(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE))
	{
	}

	virtual ~StreamingVoiceContext()
	{
		CloseHandle(hBufferEndEvent);
	}
};

//struct ComInit
//{
//	HRESULT hr;
//
//	ComInit()
//	{
//		hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
//	}
//
//	~ComInit()
//	{
//		if (SUCCEEDED(hr))
//		{
//			CoUninitialize();
//		}
//	}
//};

//ComInit g_comInit;
ComPtr<IXAudio2> g_pXAudio2;
IXAudio2MasteringVoice* g_pMasteringVoice = nullptr;
IXAudio2SourceVoice* g_pSourceVoice = nullptr;
WAVEFORMATEX g_wfx;
StreamingVoiceContext g_voiceContext;

#define STREAMING_BUFFER_SIZE 8192
#define MAX_BUFFER_COUNT 5

BYTE g_buffers[MAX_BUFFER_COUNT][STREAMING_BUFFER_SIZE];
DWORD g_currentBuffer;

int MusicPlayHook(int* params)
{
	//LPDIRECTSOUNDBUFFER& s_V0x07B0F04 = *(LPDIRECTSOUNDBUFFER*)0x07B0F04;

	//s_V0x07B0F04->Play(0, 0, DSBPLAY_LOOPING);

	//if (FAILED(g_comInit.hr))
	//{
	//	return 0;
	//}

	if (FAILED(XAudio2Create(&g_pXAudio2)))
	{
		return 0;
	}

	if (FAILED(g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice)))
	{
		return 0;
	}

	g_wfx.wFormatTag = WAVE_FORMAT_PCM;
	g_wfx.nChannels = 2;
	g_wfx.nSamplesPerSec = 22050;
	g_wfx.nAvgBytesPerSec = 4 * 22050;
	g_wfx.nBlockAlign = 4;
	g_wfx.wBitsPerSample = 16;
	g_wfx.cbSize = 0;

	if (FAILED(g_pXAudio2->CreateSourceVoice(&g_pSourceVoice, &g_wfx, 0, 1.0f, &g_voiceContext)))
	{
		return 0;
	}

	memset(g_buffers, 0, sizeof(g_buffers));
	g_currentBuffer = 0;

	g_pSourceVoice->Start();

	return 0;
}

int MusicReleaseHook(int* params)
{
	*(int*)0x007B0F08 = 1;

	if (g_pSourceVoice)
	{
		XAUDIO2_VOICE_STATE state;
		for (; ; )
		{
			g_pSourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
			if (!state.BuffersQueued)
			{
				break;
			}

			WaitForSingleObject(g_voiceContext.hBufferEndEvent, INFINITE);
		}

		g_pSourceVoice->Stop();
		g_pSourceVoice->DestroyVoice();
		g_pSourceVoice = nullptr;
	}

	g_pXAudio2.Release();

	return 0;
}

int GetMusicPositionHook(int* params)
{
	//const auto L0059305B = (int(*)())0x0059305B;

	//int pos = L0059305B();

	int pos = 0;
	*(int*)0x0608714 = 0;

	return pos;
}

int MusicLockHook(int* params)
{
	//LPDIRECTSOUNDBUFFER& s_V0x07B0F04 = *(LPDIRECTSOUNDBUFFER*)0x07B0F04;
	//int& s_V0x0608714 = *(int*)0x0608714;
	LPVOID& s_V0x07B0F20 = *(LPVOID*)0x07B0F20;
	DWORD& s_V0x07B0EF8 = *(DWORD*)0x07B0EF8;
	LPVOID& s_V0x07B0F24 = *(LPVOID*)0x07B0F24;
	DWORD& s_V0x07B0ECC = *(DWORD*)0x07B0ECC;

	HRESULT ebp04;

	//ebp04 = s_V0x07B0F04->Lock(s_V0x0608714 * 0x2000, 0x2000, &s_V0x07B0F20, &s_V0x07B0EF8, &s_V0x07B0F24, &s_V0x07B0ECC, 0);

	s_V0x07B0F20 = g_buffers[g_currentBuffer];
	s_V0x07B0EF8 = STREAMING_BUFFER_SIZE;
	s_V0x07B0F24 = nullptr;
	s_V0x07B0ECC = 0;
	ebp04 = S_OK;

	return ebp04;
}

int MusicUnlockHook(int* params)
{
	if (!g_pSourceVoice)
	{
		return S_OK;
	}

	*(int*)0x0789224 = 0;

	//LPDIRECTSOUNDBUFFER& s_V0x07B0F04 = *(LPDIRECTSOUNDBUFFER*)0x07B0F04;
	LPVOID& s_V0x07B0F20 = *(LPVOID*)0x07B0F20;
	DWORD& s_V0x07B0EF8 = *(DWORD*)0x07B0EF8;
	LPVOID& s_V0x07B0F24 = *(LPVOID*)0x07B0F24;
	DWORD& s_V0x07B0ECC = *(DWORD*)0x07B0ECC;

	//BYTE* buffer = g_buffers[g_currentBuffer];
	//memcpy(buffer, s_V0x07B0F20, s_V0x07B0EF8);
	//if (s_V0x07B0F24 && s_V0x07B0ECC)
	//{
	//	memcpy(buffer + s_V0x07B0EF8, s_V0x07B0F24, s_V0x07B0ECC);
	//}

	XAUDIO2_VOICE_STATE state;
	for (;; )
	{
		g_pSourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
		if (state.BuffersQueued < MAX_BUFFER_COUNT - 1)
		{
			break;
		}

		WaitForSingleObject(g_voiceContext.hBufferEndEvent, INFINITE);
	}

	bool empty = true;
	for (int i = 0; i < STREAMING_BUFFER_SIZE; i++)
	{
		if (g_buffers[g_currentBuffer][i] != 0)
		{
			empty = false;
			break;
		}
	}

	if (!empty)
	{
		XAUDIO2_BUFFER buf = { 0 };
		buf.AudioBytes = STREAMING_BUFFER_SIZE;
		buf.pAudioData = g_buffers[g_currentBuffer];

		g_pSourceVoice->SubmitSourceBuffer(&buf);

		g_currentBuffer++;
		g_currentBuffer %= MAX_BUFFER_COUNT;
	}

	HRESULT ebp04;

	//ebp04 = s_V0x07B0F04->Unlock(s_V0x07B0F20, s_V0x07B0EF8, s_V0x07B0F24, s_V0x07B0ECC);

	ebp04 = S_OK;

	return ebp04;
}

//int ComInitializeHook(int* params)
//{
//	return CoInitializeEx(nullptr, COINIT_MULTITHREADED);
//}

int SoundsVolumeHook(int* params)
{
	const auto ConvertVolume = (int(*)(int))0x00538FE0;

	int sfx_datapad_volume = *(unsigned char*)(0x0B0C7A0 + 0x00E7);

	int volume = params[0];
	volume = sfx_datapad_volume * 127 / 10;
	int volume2 = ConvertVolume(volume);

	return volume2;
}
