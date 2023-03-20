//=================================================================================================
//
// オーディオ管理 [audio.h]
//
//=================================================================================================
#pragma once

#include <xaudio2.h>
#include <xaudio2fx.h>
#include <xapofx.h>
#include <xapobase.h>

#pragma comment(lib, "xaudio2.lib")
//#pragma comment(lib, "xapofx.lib")		// Windowsのバージョンによる
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

	// ボリューム設定
	void SetVol(float TargetVolume);

	// フェード	(最終ボリューム値, 何フレームで)
	void Fade(float TargetVolume, float TargetTime);

	// 一時停止
	void Pause();

	// 再開
	void ReStart();

	// フィルターセット
	void SetFilter();

private:
	static IXAudio2* m_XAudio;
	static IXAudio2MasteringVoice* m_MasteringVoice;

	IXAudio2SubmixVoice* m_WetSubmix;
	IXAudio2SubmixVoice* m_DrySubmix;

	IXAudio2SubmixVoice* m_Echosubmix;

	UINT32 m_Channels;
	UINT32 m_SampleRate;

	IUnknown* m_XAPO;		// リバーブ用
	IUnknown* m_XAPOVolumeMeter;		// ボリュームメータ用

	IXAudio2SourceVoice* m_SourceVoice;
	BYTE* m_SoundData;

	int m_Length;		// 読み取りサイズ
	int m_PlayLength;		// 長さ

	// ボリューム設定用
	float m_NewVolume;
	float m_LastVolume;

	// フェード用
	bool m_mFadeFlag = false;
	float m_fLastVolume = 0.0f;
	float m_fTargetVolume = 0.0f;
	float m_fTargetTime = 0.0f;
	int m_fFadeFrame = 0;
};