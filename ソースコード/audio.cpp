//=================================================================================================
//
// オーディオ処理 [audio.cpp]
//
//=================================================================================================
#include "main.h"
#include "audio.h"



// Must match order of g_PRESET_NAMES
//XAUDIO2FX_REVERB_I3DL2_PARAMETERS g_PRESET_PARAMS[30] =
//{
//	XAUDIO2FX_I3DL2_PRESET_FOREST,
//	XAUDIO2FX_I3DL2_PRESET_DEFAULT,
//	XAUDIO2FX_I3DL2_PRESET_GENERIC,
//	XAUDIO2FX_I3DL2_PRESET_PADDEDCELL,
//	XAUDIO2FX_I3DL2_PRESET_ROOM,
//	XAUDIO2FX_I3DL2_PRESET_BATHROOM,
//	XAUDIO2FX_I3DL2_PRESET_LIVINGROOM,
//	XAUDIO2FX_I3DL2_PRESET_STONEROOM,
//	XAUDIO2FX_I3DL2_PRESET_AUDITORIUM,
//	XAUDIO2FX_I3DL2_PRESET_CONCERTHALL,
//	XAUDIO2FX_I3DL2_PRESET_CAVE,
//	XAUDIO2FX_I3DL2_PRESET_ARENA,
//	XAUDIO2FX_I3DL2_PRESET_HANGAR,
//	XAUDIO2FX_I3DL2_PRESET_CARPETEDHALLWAY,
//	XAUDIO2FX_I3DL2_PRESET_HALLWAY,
//	XAUDIO2FX_I3DL2_PRESET_STONECORRIDOR,
//	XAUDIO2FX_I3DL2_PRESET_ALLEY,
//	XAUDIO2FX_I3DL2_PRESET_CITY,
//	XAUDIO2FX_I3DL2_PRESET_MOUNTAINS,
//	XAUDIO2FX_I3DL2_PRESET_QUARRY,
//	XAUDIO2FX_I3DL2_PRESET_PLAIN,
//	XAUDIO2FX_I3DL2_PRESET_PARKINGLOT,
//	XAUDIO2FX_I3DL2_PRESET_SEWERPIPE,
//	XAUDIO2FX_I3DL2_PRESET_UNDERWATER,
//	XAUDIO2FX_I3DL2_PRESET_SMALLROOM,
//	XAUDIO2FX_I3DL2_PRESET_MEDIUMROOM,
//	XAUDIO2FX_I3DL2_PRESET_LARGEROOM,
//	XAUDIO2FX_I3DL2_PRESET_MEDIUMHALL,
//	XAUDIO2FX_I3DL2_PRESET_LARGEHALL,
//	XAUDIO2FX_I3DL2_PRESET_PLATE,
//};





IXAudio2* Audio::m_XAudio = NULL;
IXAudio2MasteringVoice* Audio::m_MasteringVoice = NULL;

// Helper macrose
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=nullptr; } }
#endif

void Audio::InitMaster()
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

}

void Audio::Init()
{
	// デバイスの確認用
	XAUDIO2_VOICE_DETAILS details;
	m_MasteringVoice->GetVoiceDetails(&details);
	m_Channels = details.InputChannels;
	m_SampleRate = details.InputSampleRate;

	XAUDIO2_DEVICE_DETAILS details2;
	m_XAudio->GetDeviceDetails(0, &details2);


	//// ボリュームメータの設定
	//HRESULT hr;
	//hr = XAudio2CreateVolumeMeter(&m_XAPO);

	{
		////UINT32 rflags = 0;
		////// リバーブの生成
		////XAudio2CreateReverb(&m_XAPO, rflags);

		//// EFFECT_DESCRIPTORの作成
		//XAUDIO2_EFFECT_DESCRIPTOR descriptor;
		//descriptor.InitialState = true;		// 最初から有効状態
		//UINT32 Channels = descriptor.OutputChannels = 2;		/*details2.OutputFormat.Format.nChannels;*/		// 2chのエフェクト
		//descriptor.pEffect = m_XAPO;		// エフェクトの本体

		//// EFFECT_CHAINの作成
		//XAUDIO2_EFFECT_CHAIN chain;
		//chain.EffectCount = 1;		// 挿すのは１個
		//chain.pEffectDescriptors = &descriptor;		// 構造体を指示

		//// サブミックスボイスの生成
		//m_XAudio->CreateSubmixVoice(&m_WetSubmix, Channels, m_SampleRate);
		//m_XAudio->CreateSubmixVoice(&m_DrySubmix, Channels, m_SampleRate);

		//// ボイスにEFFECT_CHAINを挿す
		//m_WetSubmix->SetEffectChain(&chain);

		//// リバーブのXAPOを廃棄する
		//m_XAPO->Release();

		//// リバーブのパラメータ構造体
		//XAUDIO2FX_REVERB_PARAMETERS reverbParam;

		//// いろいろ設定
		//XAUDIO2FX_REVERB_I3DL2_PARAMETERS i3dl2Param = XAUDIO2FX_I3DL2_PRESET_BATHROOM;
		//ReverbConvertI3DL2ToNative(&i3dl2Param, &reverbParam);		// I3DL2→REVERB_PARAM変換

		//reverbParam.WetDryMix = 0;

		//// エフェクトに通知
		//m_WetSubmix->SetEffectParameters(0, &reverbParam, sizeof(reverbParam));

		//m_WetSubmix->SetVolume(0.5f);
		//m_DrySubmix->SetVolume(0.5f);
	}

	{
		UINT32 rflags = 0;
		// リバーブの生成
		XAudio2CreateReverb(&m_XAPO, rflags);


		// XAPOFXの生成
		CreateFX(__uuidof(FXEcho), &m_XAPO);

		// EFFECT_DESCRIPTORの作成
		XAUDIO2_EFFECT_DESCRIPTOR descriptor;
		descriptor.InitialState = true;
		UINT32 Channels = descriptor.OutputChannels = 2;
		descriptor.pEffect = m_XAPO;

		// EFFECT_CHAINの作成
		XAUDIO2_EFFECT_CHAIN chain;
		chain.EffectCount = 1;
		chain.pEffectDescriptors = &descriptor;

		// サブミックスボイスの作成
		m_XAudio->CreateSubmixVoice(&m_Echosubmix, Channels, m_SampleRate);

		m_Echosubmix->SetEffectChain(&chain);

		m_XAPO->Release();

		// エコーのパラメータ構造体
		FXECHO_PARAMETERS EchoRaram;

		EchoRaram.WetDryMix = FXECHO_MIN_WETDRYMIX; /*FXECHO_DEFAULT_WETDRYMIX;*/
		EchoRaram.Delay = /*FXECHO_DEFAULT_DELAY;*/
		EchoRaram.Feedback = FXECHO_DEFAULT_FEEDBACK;

		// エフェクトに通知する
		m_Echosubmix->SetEffectParameters(0, &EchoRaram, sizeof(EchoRaram));
	}
}

void Audio::Uninit()
{
	m_SourceVoice->Stop();
	m_SourceVoice->DestroyVoice();

	delete[] m_SoundData;

	// サブミックスボイスの削除
	if (m_WetSubmix != NULL)
	{
		m_WetSubmix->DestroyVoice();
		m_WetSubmix = NULL;
	}
	if (m_DrySubmix != NULL)
	{
		m_DrySubmix->DestroyVoice();
		m_DrySubmix = NULL;
	}
	if (m_Echosubmix != NULL)
	{
		m_Echosubmix->DestroyVoice();
		m_Echosubmix = NULL;
	}
}


void Audio::UninitMaster()
{
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

void Audio::Load(const char* FileName)
{
	// 読み込み用のバッファ
	WAVEFORMATEX wfx;


	{
		// WAVファイルを開く
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };

		hmmio = mmioOpen((LPSTR)FileName, &mmioinfo, MMIO_READ);		// 引数(ファイル名、MMIO情報、モード)
		assert(hmmio);

		if (hmmio == NULL)
		{
			// オープン失敗
			return;
		}



		// WAVファイルの先頭チャンクRIFFチャンクに進入
		// RIFFチャンクに進入するためにfccTypeにWAVEを設定
		MMCKINFO riffck_info = { 0 };

		riffck_info.fccType = mmioFOURCC('W', 'A', 'V', 'E');

		// RIFFチャンクに進入する
		if (MMSYSERR_NOERROR != mmioDescend(
			hmmio,		// MMIOハンドル
			&riffck_info,		// 取得したチャンクの情報
			NULL,		// 親チャンク
			MMIO_FINDRIFF)		// 取得情報の種類
			)
		{
			// 失敗
			mmioClose(hmmio, MMIO_FHOPEN);
			return;
		}



		// WAVEファイルのサンプリングレートなどの情報を保持している[fmtチャンク]に進入
		MMCKINFO mmck_info = { 0 };
		// 進入先のチャンクを"fmt"として設定する
		mmck_info.ckid = mmioFOURCC('f', 'm', 't', ' ');
		if (MMSYSERR_NOERROR != mmioDescend(
			hmmio,
			&mmck_info, &riffck_info, MMIO_FINDCHUNK)
			)
		{
			// fmtチャンクがない
			mmioClose(hmmio, MMIO_FHOPEN);
			return;
		}



		// fmtチャンクの情報を読み込む
		// fmtデータの読み込み
		LONG read_size = mmioRead(
			hmmio,		// ハンドル
			(HPSTR)&wfx,		// 読み込み用バッファ
			sizeof(wfx)		// バッファサイズ
		);

		if (read_size != sizeof(wfx))
		{
			// 読み込みサイズが一致していないのでエラー
			mmioClose(hmmio, MMIO_FHOPEN);
			return;
		}

		// fmtチャンクを退出する
		if (mmioAscend(hmmio, &mmck_info, 0) != MMSYSERR_NOERROR)
		{
			mmioClose(hmmio, 0);
			return;
		}



		// 音データを保持しているdataチャンクに進入
		// dataチャンクに進入
		mmck_info.ckid = mmioFOURCC('d', 'a', 't', 'a');
		if (mmioDescend(hmmio, &mmck_info, &riffck_info, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
		{
			// 進入失敗
			mmioClose(hmmio, MMIO_FHOPEN);
			return;
		}

		// 音データの読み込み
		m_SoundData = new unsigned char[mmck_info.cksize];
		read_size = mmioRead(hmmio, (HPSTR)m_SoundData, mmck_info.cksize);
		if (read_size != mmck_info.cksize)
		{
			mmioClose(hmmio, MMIO_FHOPEN);
			delete[] m_SoundData;
			return;
		}

		m_Length = read_size;
		m_PlayLength = read_size / wfx.nBlockAlign;


		// WAVファイルを閉じる
		mmioClose(hmmio, 0);		// 引数(クローズ対象のハンドル、オプション(設定なしは0))
	}

	// サウンドソースの生成
	HRESULT hr;
	if (FAILED(hr = m_XAudio->CreateSourceVoice(&m_SourceVoice, &wfx)))
	{
		return;
	}
	//if (FAILED(hr = m_XAudio->CreateSourceVoice(&m_SourceVoice, &wfx, XAUDIO2_VOICE_USEFILTER, 16.0f)))
	//{
	//	return;
	//}
}

void Audio::Play(bool Loop)
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
	m_LastVolume = 0.5f * 0.5f;

	{
		//// ソースボイスの出力先をマスターボイスからサブミックスボイスに切り替える
		//XAUDIO2_SEND_DESCRIPTOR send = { 0, m_SubmixVoice };
		//XAUDIO2_VOICE_SENDS sendlist = { 1, &send };

		//m_SourceVoice->SetOutputVoices(&sendlist);
	}

	{
		//XAUDIO2_SEND_DESCRIPTOR send[2] = { 0, m_WetSubmix, 0, m_DrySubmix };
		//XAUDIO2_VOICE_SENDS sendlist = { 2, send };
	}

	XAUDIO2_SEND_DESCRIPTOR send = { 0, m_Echosubmix };
	XAUDIO2_VOICE_SENDS sendlist = { 1, &send };

	m_SourceVoice->SetOutputVoices(&sendlist);


	// 再生
	m_SourceVoice->Start();
}

void Audio::Update()
{
	if (m_mFadeFlag == true)
	{
		UpdateFade();
	}
}

// ボリューム設定用
void Audio::SetVol(float TargetVolume)
{
	m_NewVolume = TargetVolume * TargetVolume;

	if (m_NewVolume != m_LastVolume)
	{
		m_SourceVoice->SetVolume(m_NewVolume);
		m_LastVolume = m_NewVolume;
	}
}

void Audio::UpdateFade()
{
	m_fFadeFrame += 1;

	int num = m_fFadeFrame % 4;

	if (num != 0)
	{
		return;
	}

	float time = m_fFadeFrame / m_fTargetTime;
	float vel = m_fLastVolume - m_fTargetVolume;

	if (time < 1.0f)
	{
		float vel2 = m_LastVolume - (vel * time);
		SetVol(vel2);
		return;
	}

	if (m_fFadeFrame >= m_fTargetTime)
	{
		m_mFadeFlag = false;
		m_fFadeFrame = 0;
	}
}

// フェードイン用
void Audio::Fade(float TargetVolume, float TargetTime)
{
	if (m_LastVolume == (TargetVolume * TargetVolume))
	{
		m_fFadeFrame = false;
		return;
	}

	m_mFadeFlag = true;
	m_fLastVolume = m_LastVolume;
	m_fTargetVolume = TargetVolume * TargetVolume;
	m_fTargetTime = TargetTime;
	m_fFadeFrame = 0;
}

void Audio::Pause()
{ 
	Fade(0.0f, 30.0f);
	m_SourceVoice->Stop(XAUDIO2_PLAY_TAILS); 
}

void Audio::ReStart()
{ 
	m_SourceVoice->Start();
	Fade(0.5f, 30.0f);
}

void Audio::SetFilter()
{
	// ローパス指定
	XAUDIO2_FILTER_PARAMETERS FilterParams;		// フィルタ指示構造体
	FilterParams.Type = XAUDIO2_FILTER_TYPE::NotchFilter;
	FilterParams.Frequency = 0.5f;
	FilterParams.OneOverQ = 0.707f;

	m_SourceVoice->SetFilterParameters(&FilterParams);

}