//=================================================================================================
//
// オーディオ3D処理 [audio3d.cpp]
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "audio3d.h"
#include "camera.h"
#include "player.h"

// Helper macrose
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=nullptr; } }
#endif

// Specify sound cone to add directionality to listener for artistic effect:
// Emitters behind the listener are defined here to be more attenuated,
// have a lower LPF cutoff frequency,
// yet have a slightly higher reverb send level.
static const X3DAUDIO_CONE Listener_DirectionalCone = { X3DAUDIO_PI * 5.0f / 6.0f, X3DAUDIO_PI * 11.0f / 6.0f, 1.0f, 0.75f, 0.0f, 0.25f, 0.708f, 1.0f };

// Specify LFE level distance curve such that it rolls off much sooner than
// all non-LFE channels, making use of the subwoofer more dramatic.
static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[3] = { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };

// Specify reverb send level distance curve such that reverb send increases
// slightly with distance before rolling off to silence.
// With the direct channels being increasingly attenuated with distance,
// this has the effect of increasing the reverb-to-direct sound ratio,
// reinforcing the perception of distance.
static const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[3] = { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
static const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };




// Must match order of g_PRESET_NAMES
XAUDIO2FX_REVERB_I3DL2_PARAMETERS g_PRESET_PARAMS[NUM_PRESETS] =
{
	XAUDIO2FX_I3DL2_PRESET_FOREST,
	XAUDIO2FX_I3DL2_PRESET_DEFAULT,
	XAUDIO2FX_I3DL2_PRESET_GENERIC,
	XAUDIO2FX_I3DL2_PRESET_PADDEDCELL,
	XAUDIO2FX_I3DL2_PRESET_ROOM,
	XAUDIO2FX_I3DL2_PRESET_BATHROOM,
	XAUDIO2FX_I3DL2_PRESET_LIVINGROOM,
	XAUDIO2FX_I3DL2_PRESET_STONEROOM,
	XAUDIO2FX_I3DL2_PRESET_AUDITORIUM,
	XAUDIO2FX_I3DL2_PRESET_CONCERTHALL,
	XAUDIO2FX_I3DL2_PRESET_CAVE,
	XAUDIO2FX_I3DL2_PRESET_ARENA,
	XAUDIO2FX_I3DL2_PRESET_HANGAR,
	XAUDIO2FX_I3DL2_PRESET_CARPETEDHALLWAY,
	XAUDIO2FX_I3DL2_PRESET_HALLWAY,
	XAUDIO2FX_I3DL2_PRESET_STONECORRIDOR,
	XAUDIO2FX_I3DL2_PRESET_ALLEY,
	XAUDIO2FX_I3DL2_PRESET_CITY,
	XAUDIO2FX_I3DL2_PRESET_MOUNTAINS,
	XAUDIO2FX_I3DL2_PRESET_QUARRY,
	XAUDIO2FX_I3DL2_PRESET_PLAIN,
	XAUDIO2FX_I3DL2_PRESET_PARKINGLOT,
	XAUDIO2FX_I3DL2_PRESET_SEWERPIPE,
	XAUDIO2FX_I3DL2_PRESET_UNDERWATER,
	XAUDIO2FX_I3DL2_PRESET_SMALLROOM,
	XAUDIO2FX_I3DL2_PRESET_MEDIUMROOM,
	XAUDIO2FX_I3DL2_PRESET_LARGEROOM,
	XAUDIO2FX_I3DL2_PRESET_MEDIUMHALL,
	XAUDIO2FX_I3DL2_PRESET_LARGEHALL,
	XAUDIO2FX_I3DL2_PRESET_PLATE,
};


IXAudio2* Audio3D::m_XAudio = NULL;
IXAudio2MasteringVoice* Audio3D::m_MasteringVoice = NULL;

IUnknown* Audio3D::m_ReverbEffect = NULL;
IXAudio2SubmixVoice* Audio3D::m_SubmixVoice = NULL;

UINT32 Audio3D::m_InputChannels = NULL;
UINT32 Audio3D::m_InputSampleRates = NULL;


UINT32 Audio3D::m_nSampleRate = NULL;
DWORD Audio3D::m_dwChannelMask = NULL;
UINT32 Audio3D::m_nChannels = NULL;

X3DAUDIO_HANDLE Audio3D::m_X3DInstance = { 0 };

void Audio3D::InitMaster()
{
	// Initialize XAudio2
	CoInitializeEx(NULL, COINIT_MULTITHREADED);


	UINT32 flags = 0;
	// XAudio2エンジンのインスタンスの生成
	HRESULT hr = XAudio2Create(&m_XAudio, flags);
	if (FAILED(hr))
	{
		return;
	}


	// マスターリング音声の作成
	if (FAILED(hr = m_XAudio->CreateMasteringVoice(&m_MasteringVoice)))
	{
		SAFE_RELEASE(m_XAudio);
		return;
	}

	// デバイスの確認
	XAUDIO2_VOICE_DETAILS vdetails;
	m_MasteringVoice->GetVoiceDetails(&vdetails);
	m_InputChannels = vdetails.InputChannels;
	m_InputSampleRates = vdetails.InputSampleRate;

	XAUDIO2_DEVICE_DETAILS details;
	m_XAudio->GetDeviceDetails(0, &details);
	m_nSampleRate = details.OutputFormat.Format.nSamplesPerSec;
	m_dwChannelMask = details.OutputFormat.dwChannelMask;
	m_nChannels = details.OutputFormat.Format.nChannels;

	UINT32 rflags = 0;
	// リバーブの生成
	if (FAILED(hr = XAudio2CreateReverb(&m_ReverbEffect, rflags)))
	{
		SAFE_RELEASE(m_XAudio);
		return;
	}

	// サブミックスボイスの作成
	XAUDIO2_EFFECT_DESCRIPTOR effects[] = { {m_ReverbEffect, TRUE, 1 } };
	XAUDIO2_EFFECT_CHAIN effectChain = { 1, effects };

	if (FAILED(hr = m_XAudio->CreateSubmixVoice(&m_SubmixVoice, 1,
		m_nSampleRate, 0, 0, NULL, &effectChain)))
	{
		SAFE_RELEASE(m_XAudio);
		SAFE_RELEASE(m_ReverbEffect);
		return;
	}

	// Set default FX params
	XAUDIO2FX_REVERB_PARAMETERS native;
	ReverbConvertI3DL2ToNative(&g_PRESET_PARAMS[0], &native);
	m_SubmixVoice->SetEffectParameters(0, &native, sizeof(native));

	// X3DAudio Initialize
	const float SPEEDOFSOUND = X3DAUDIO_SPEED_OF_SOUND;

	X3DAudioInitialize(m_dwChannelMask, SPEEDOFSOUND, m_X3DInstance);

}

void Audio3D::UninitMaster()
{
	// サブミックスボイスの削除
	if (m_SubmixVoice != NULL)
	{
		m_SubmixVoice->DestroyVoice();
		m_SubmixVoice = NULL;
	}
	// マスターボイスの削除
	if (m_MasteringVoice != NULL)
	{
		m_MasteringVoice->DestroyVoice();
		m_MasteringVoice = NULL;
	}
	// XAudioエンジンの解放
	if (m_XAudio != NULL)
	{
		m_XAudio->Release();
		m_XAudio = NULL;
	}
	CoUninitialize();

}

void Audio3D::Init()
{
	Scene* scene = Manager::GetScene();
	m_ListenerObject = scene->GetGameObject<Camera>(LAYER_CAMERA);
	m_Player = scene->GetGameObject<Player>(LAYER_3D);

	m_Listener = {};
	m_Emitter = {};


	m_Emitter.pCone = &m_EmitterCone;
	m_Emitter.pCone->InnerAngle = 0.0f;
	// Setting the inner cone angles to X3DAUDIO_2PI and
	// outer cone other than 0 causes
	// the emitter to act like a point emitter using the
	// INNER cone settings only.
	m_Emitter.pCone->OuterAngle = 0.0f;
	// Setting the outer cone angles to zero causes
	// the emitter to act like a point emitter using the
	// OUTER cone settings only.
	m_Emitter.pCone->InnerVolume = 0.0f;
	m_Emitter.pCone->OuterVolume = 1.0f;
	m_Emitter.pCone->InnerLPF = 0.0f;
	m_Emitter.pCone->OuterLPF = 1.0f;
	m_Emitter.pCone->InnerReverb = 0.0f;
	m_Emitter.pCone->OuterReverb = 1.0f;

	m_Emitter.Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Emitter.OrientFront = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Emitter.OrientTop = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Emitter.OrientFront.z = 1.0f;
	m_Emitter.OrientTop.y = 1.0f;

	m_Emitter.ChannelCount = INPUTCHANNELS;
	m_Emitter.ChannelRadius = 1.0f;
	m_Emitter.pChannelAzimuths = m_EmitterAzimuths;

	// Use of Inner radius allows for smoother transitions as
	// a sound travels directly through, above, or below the listener.
	// It also may be used to give elevation cues.
	m_Emitter.InnerRadius = 2.0f;
	m_Emitter.InnerRadiusAngle = X3DAUDIO_PI / 4.0f;;

	m_Emitter.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;
	m_Emitter.pLFECurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;
	m_Emitter.pLPFDirectCurve = nullptr; // use default curve
	m_Emitter.pLPFReverbCurve = nullptr; // use default curve
	m_Emitter.pReverbCurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;
	m_Emitter.CurveDistanceScaler = 14.0f;
	m_Emitter.DopplerScaler = 1.0f;




	m_DspSettings.SrcChannelCount = 2;
	m_DspSettings.DstChannelCount = m_nChannels;
	m_DspSettings.pMatrixCoefficients = m_MatrixCoefficients;

	m_UseRedirectToLFE = ((m_dwChannelMask & SPEAKER_LOW_FREQUENCY) != 0);
}

void Audio3D::Uninit()
{
	if (m_SourceVoice)
	{
		m_SourceVoice->Stop();
		m_SourceVoice->DestroyVoice();

	}

	delete[] m_SoundData;

}

void Audio3D::Update()
{
	m_Listener.Position = m_ListenerObject->GetPosition();
	m_Emitter.Position = m_EmitterObject->GetPosition();

	m_Listener.OrientFront = m_Player->GetForward();
	m_Listener.OrientTop = m_Player->GetUpForward();

	m_Emitter.OrientFront = m_EmitterObject->GetForward();
	m_Emitter.OrientTop = m_EmitterObject->GetUpForward();


	DWORD dwCalcFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER
		| X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_LPF_REVERB
		| X3DAUDIO_CALCULATE_REVERB;
	if (m_UseRedirectToLFE)
	{
		// On devices with an LFE channel, allow the mono source data
		// to be routed to the LFE destination channel.
		dwCalcFlags |= X3DAUDIO_CALCULATE_REDIRECT_TO_LFE;
	}

	X3DAudioCalculate(m_X3DInstance, &m_Listener, &m_Emitter,
		dwCalcFlags, &m_DspSettings);

	IXAudio2SourceVoice* voice = m_SourceVoice;
	if (voice)
	{
		voice->SetFrequencyRatio(m_DspSettings.DopplerFactor);
		voice->SetOutputMatrix(m_MasteringVoice, 2, m_nChannels, m_MatrixCoefficients);

		voice->SetOutputMatrix(m_SubmixVoice, 1, 1, &m_DspSettings.ReverbLevel);

		//XAUDIO2_FILTER_PARAMETERS FilterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * m_DspSettings.LPFDirectCoefficient), 1.0f };
		//voice->SetFilterParameters(&FilterParameters);

		XAUDIO2_FILTER_PARAMETERS FilterParametersDirect = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * m_DspSettings.LPFDirectCoefficient), 1.0f }; // see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
		voice->SetOutputFilterParameters(m_MasteringVoice, &FilterParametersDirect);
		XAUDIO2_FILTER_PARAMETERS FilterParametersReverb = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * m_DspSettings.LPFReverbCoefficient), 1.0f }; // see XAudio2CutoffFrequencyToRadians() in XAudio2.h for more information on the formula used here
		voice->SetOutputFilterParameters(m_SubmixVoice, &FilterParametersReverb);

	}

}

void Audio3D::Load(const char* FileName)
{
	// サウンドデータ読込
	WAVEFORMATEX wfx = { 0 };

	{
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;


		hmmio = mmioOpen((LPSTR)FileName, &mmioinfo, MMIO_READ);
		assert(hmmio);

		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
		{
			mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
		}
		else
		{
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}
		mmioAscend(hmmio, &mmckinfo, 0);

		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);



		buflen = datachunkinfo.cksize;
		m_SoundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)m_SoundData, buflen);


		m_Length = readlen;
		m_PlayLength = readlen / wfx.nBlockAlign;


		mmioClose(hmmio, 0);
	}

	XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];
	sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER; // LPF direct-path
	sendDescriptors[0].pOutputVoice = m_MasteringVoice;
	sendDescriptors[1].Flags = XAUDIO2_SEND_USEFILTER; // LPF reverb-path -- omit for better performance at the cost of less realistic occlusion
	sendDescriptors[1].pOutputVoice = m_SubmixVoice;
	const XAUDIO2_VOICE_SENDS sendList = { 2, sendDescriptors };

	// create the source voice
	m_XAudio->CreateSourceVoice(&m_SourceVoice, &wfx, 0,
		2.0f, NULL, &sendList);

	assert(m_SourceVoice);

}

void Audio3D::Play(bool Loop)
{
	m_SourceVoice->Stop();
	m_SourceVoice->FlushSourceBuffers();

	// バッファ設定
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = m_Length;
	bufinfo.pAudioData = m_SoundData;
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = m_PlayLength;

	// ループ設定
	if (Loop)
	{
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = m_PlayLength;
		bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	m_SourceVoice->SubmitSourceBuffer(&bufinfo, NULL);

	m_SourceVoice->SetVolume(0.5f * 0.5f);

	// 再生
	m_SourceVoice->Start();

}