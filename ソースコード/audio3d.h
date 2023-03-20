//=================================================================================================
//
// オーディオ3D管理 [audio3d.h]
//
//=================================================================================================
#pragma once
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>

#include "gameobject.h"

#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"x3daudio.lib")

// Global defines
#define INPUTCHANNELS 2
#define OUTPUTCHANNELS 16

#define NUM_PRESETS 30


class Audio3D : public GameObject
{
public:
	static void InitMaster();
	static void UninitMaster();

	void Init();
	void Uninit();
	void Update();
	void Draw() {}

	void Load(const char* FileName);
	void Play(bool Loop = false);

	// Init内で呼ぶ必要がある
	void SetEmitter(GameObject* obj) { m_EmitterObject = obj; }
private:
	static IXAudio2* m_XAudio;
	static IXAudio2MasteringVoice* m_MasteringVoice;

	static IUnknown* m_ReverbEffect;
	static IXAudio2SubmixVoice* m_SubmixVoice;

	// デバイス確認用
	static UINT32 m_InputChannels;
	static UINT32 m_InputSampleRates;

	static UINT32 m_nSampleRate;
	static DWORD m_dwChannelMask;
	static UINT32 m_nChannels;

	// 3D用
	static X3DAUDIO_HANDLE m_X3DInstance;

	X3DAUDIO_DSP_SETTINGS m_DspSettings;
	X3DAUDIO_LISTENER m_Listener;
	X3DAUDIO_EMITTER m_Emitter;


	X3DAUDIO_CONE m_EmitterCone;
	bool m_UseRedirectToLFE;

	FLOAT32 m_EmitterAzimuths[INPUTCHANNELS];
	FLOAT32 m_MatrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];


	IXAudio2SourceVoice* m_SourceVoice;
	BYTE* m_SoundData;

	int m_Length;
	int m_PlayLength;


	// リスナー用オブジェクト
	GameObject* m_EmitterObject = NULL;
	class Camera* m_ListenerObject = NULL;
	class Player* m_Player = NULL;
};