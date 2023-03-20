//=================================================================================================
//
// �I�[�f�B�I3D�Ǘ� [audio3d.h]
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

	// Init���ŌĂԕK�v������
	void SetEmitter(GameObject* obj) { m_EmitterObject = obj; }
private:
	static IXAudio2* m_XAudio;
	static IXAudio2MasteringVoice* m_MasteringVoice;

	static IUnknown* m_ReverbEffect;
	static IXAudio2SubmixVoice* m_SubmixVoice;

	// �f�o�C�X�m�F�p
	static UINT32 m_InputChannels;
	static UINT32 m_InputSampleRates;

	static UINT32 m_nSampleRate;
	static DWORD m_dwChannelMask;
	static UINT32 m_nChannels;

	// 3D�p
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


	// ���X�i�[�p�I�u�W�F�N�g
	GameObject* m_EmitterObject = NULL;
	class Camera* m_ListenerObject = NULL;
	class Player* m_Player = NULL;
};