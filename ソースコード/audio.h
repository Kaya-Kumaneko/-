//=================================================================================================
//
// �I�[�f�B�I�Ǘ� [audio.h]
//
//=================================================================================================
#pragma once

#include <xaudio2.h>
#include <xaudio2fx.h>
#include <xapofx.h>
#include <xapobase.h>

#pragma comment(lib, "xaudio2.lib")
//#pragma comment(lib, "xapofx.lib")		// Windows�̃o�[�W�����ɂ��
#pragma comment(lib, "xapobase.lib")

#include "gameobject.h"

class Audio : public GameObject
{
public:
	static void InitMaster();
	static void UninitMaster();

	void Init();
	void Uninit();
	void Update();
	void Draw() {};

	void UpdateFade();

	void Load(const char* FileName);
	void Play(bool Loop = false);

	// �{�����[���ݒ�
	void SetVol(float TargetVolume);

	// �t�F�[�h	(�ŏI�{�����[���l, ���t���[����)
	void Fade(float TargetVolume, float TargetTime);

	// �ꎞ��~
	void Pause();

	// �ĊJ
	void ReStart();

	// �t�B���^�[�Z�b�g
	void SetFilter();

private:
	static IXAudio2* m_XAudio;
	static IXAudio2MasteringVoice* m_MasteringVoice;

	IXAudio2SubmixVoice* m_WetSubmix;
	IXAudio2SubmixVoice* m_DrySubmix;

	IXAudio2SubmixVoice* m_Echosubmix;

	UINT32 m_Channels;
	UINT32 m_SampleRate;

	IUnknown* m_XAPO;		// ���o�[�u�p
	IUnknown* m_XAPOVolumeMeter;		// �{�����[�����[�^�p

	IXAudio2SourceVoice* m_SourceVoice;
	BYTE* m_SoundData;

	int m_Length;		// �ǂݎ��T�C�Y
	int m_PlayLength;		// ����

	// �{�����[���ݒ�p
	float m_NewVolume;
	float m_LastVolume;

	// �t�F�[�h�p
	bool m_mFadeFlag = false;
	float m_fLastVolume = 0.0f;
	float m_fTargetVolume = 0.0f;
	float m_fTargetTime = 0.0f;
	int m_fFadeFrame = 0;
};