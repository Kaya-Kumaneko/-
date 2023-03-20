//=================================================================================================
//
// �I�[�f�B�I���� [audio.cpp]
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
	// XAudio2�G���W���̃C���X�^���X�̐���
	HRESULT hr = XAudio2Create(&m_XAudio, flags);
	if (FAILED(hr))
	{
		return;
	}


	// �}�X�^�[�����O�����̍쐬
	if (FAILED(hr = m_XAudio->CreateMasteringVoice(&m_MasteringVoice)))
	{
		SAFE_RELEASE(m_XAudio);
		return;
	}

}

void Audio::Init()
{
	// �f�o�C�X�̊m�F�p
	XAUDIO2_VOICE_DETAILS details;
	m_MasteringVoice->GetVoiceDetails(&details);
	m_Channels = details.InputChannels;
	m_SampleRate = details.InputSampleRate;

	XAUDIO2_DEVICE_DETAILS details2;
	m_XAudio->GetDeviceDetails(0, &details2);


	//// �{�����[�����[�^�̐ݒ�
	//HRESULT hr;
	//hr = XAudio2CreateVolumeMeter(&m_XAPO);

	{
		////UINT32 rflags = 0;
		////// ���o�[�u�̐���
		////XAudio2CreateReverb(&m_XAPO, rflags);

		//// EFFECT_DESCRIPTOR�̍쐬
		//XAUDIO2_EFFECT_DESCRIPTOR descriptor;
		//descriptor.InitialState = true;		// �ŏ�����L�����
		//UINT32 Channels = descriptor.OutputChannels = 2;		/*details2.OutputFormat.Format.nChannels;*/		// 2ch�̃G�t�F�N�g
		//descriptor.pEffect = m_XAPO;		// �G�t�F�N�g�̖{��

		//// EFFECT_CHAIN�̍쐬
		//XAUDIO2_EFFECT_CHAIN chain;
		//chain.EffectCount = 1;		// �}���̂͂P��
		//chain.pEffectDescriptors = &descriptor;		// �\���̂��w��

		//// �T�u�~�b�N�X�{�C�X�̐���
		//m_XAudio->CreateSubmixVoice(&m_WetSubmix, Channels, m_SampleRate);
		//m_XAudio->CreateSubmixVoice(&m_DrySubmix, Channels, m_SampleRate);

		//// �{�C�X��EFFECT_CHAIN��}��
		//m_WetSubmix->SetEffectChain(&chain);

		//// ���o�[�u��XAPO��p������
		//m_XAPO->Release();

		//// ���o�[�u�̃p�����[�^�\����
		//XAUDIO2FX_REVERB_PARAMETERS reverbParam;

		//// ���낢��ݒ�
		//XAUDIO2FX_REVERB_I3DL2_PARAMETERS i3dl2Param = XAUDIO2FX_I3DL2_PRESET_BATHROOM;
		//ReverbConvertI3DL2ToNative(&i3dl2Param, &reverbParam);		// I3DL2��REVERB_PARAM�ϊ�

		//reverbParam.WetDryMix = 0;

		//// �G�t�F�N�g�ɒʒm
		//m_WetSubmix->SetEffectParameters(0, &reverbParam, sizeof(reverbParam));

		//m_WetSubmix->SetVolume(0.5f);
		//m_DrySubmix->SetVolume(0.5f);
	}

	{
		UINT32 rflags = 0;
		// ���o�[�u�̐���
		XAudio2CreateReverb(&m_XAPO, rflags);


		// XAPOFX�̐���
		CreateFX(__uuidof(FXEcho), &m_XAPO);

		// EFFECT_DESCRIPTOR�̍쐬
		XAUDIO2_EFFECT_DESCRIPTOR descriptor;
		descriptor.InitialState = true;
		UINT32 Channels = descriptor.OutputChannels = 2;
		descriptor.pEffect = m_XAPO;

		// EFFECT_CHAIN�̍쐬
		XAUDIO2_EFFECT_CHAIN chain;
		chain.EffectCount = 1;
		chain.pEffectDescriptors = &descriptor;

		// �T�u�~�b�N�X�{�C�X�̍쐬
		m_XAudio->CreateSubmixVoice(&m_Echosubmix, Channels, m_SampleRate);

		m_Echosubmix->SetEffectChain(&chain);

		m_XAPO->Release();

		// �G�R�[�̃p�����[�^�\����
		FXECHO_PARAMETERS EchoRaram;

		EchoRaram.WetDryMix = FXECHO_MIN_WETDRYMIX; /*FXECHO_DEFAULT_WETDRYMIX;*/
		EchoRaram.Delay = /*FXECHO_DEFAULT_DELAY;*/
		EchoRaram.Feedback = FXECHO_DEFAULT_FEEDBACK;

		// �G�t�F�N�g�ɒʒm����
		m_Echosubmix->SetEffectParameters(0, &EchoRaram, sizeof(EchoRaram));
	}
}

void Audio::Uninit()
{
	m_SourceVoice->Stop();
	m_SourceVoice->DestroyVoice();

	delete[] m_SoundData;

	// �T�u�~�b�N�X�{�C�X�̍폜
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
	// �}�X�^�[�{�C�X�̍폜
	if (m_MasteringVoice != NULL)
	{
		m_MasteringVoice->DestroyVoice();
		m_MasteringVoice = NULL;
	}
	// XAudio�G���W���̉��
	if (m_XAudio != NULL)
	{
		m_XAudio->Release();
		m_XAudio = NULL;
	}
	CoUninitialize();

}

void Audio::Load(const char* FileName)
{
	// �ǂݍ��ݗp�̃o�b�t�@
	WAVEFORMATEX wfx;


	{
		// WAV�t�@�C�����J��
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };

		hmmio = mmioOpen((LPSTR)FileName, &mmioinfo, MMIO_READ);		// ����(�t�@�C�����AMMIO���A���[�h)
		assert(hmmio);

		if (hmmio == NULL)
		{
			// �I�[�v�����s
			return;
		}



		// WAV�t�@�C���̐擪�`�����NRIFF�`�����N�ɐi��
		// RIFF�`�����N�ɐi�����邽�߂�fccType��WAVE��ݒ�
		MMCKINFO riffck_info = { 0 };

		riffck_info.fccType = mmioFOURCC('W', 'A', 'V', 'E');

		// RIFF�`�����N�ɐi������
		if (MMSYSERR_NOERROR != mmioDescend(
			hmmio,		// MMIO�n���h��
			&riffck_info,		// �擾�����`�����N�̏��
			NULL,		// �e�`�����N
			MMIO_FINDRIFF)		// �擾���̎��
			)
		{
			// ���s
			mmioClose(hmmio, MMIO_FHOPEN);
			return;
		}



		// WAVE�t�@�C���̃T���v�����O���[�g�Ȃǂ̏���ێ����Ă���[fmt�`�����N]�ɐi��
		MMCKINFO mmck_info = { 0 };
		// �i����̃`�����N��"fmt"�Ƃ��Đݒ肷��
		mmck_info.ckid = mmioFOURCC('f', 'm', 't', ' ');
		if (MMSYSERR_NOERROR != mmioDescend(
			hmmio,
			&mmck_info, &riffck_info, MMIO_FINDCHUNK)
			)
		{
			// fmt�`�����N���Ȃ�
			mmioClose(hmmio, MMIO_FHOPEN);
			return;
		}



		// fmt�`�����N�̏���ǂݍ���
		// fmt�f�[�^�̓ǂݍ���
		LONG read_size = mmioRead(
			hmmio,		// �n���h��
			(HPSTR)&wfx,		// �ǂݍ��ݗp�o�b�t�@
			sizeof(wfx)		// �o�b�t�@�T�C�Y
		);

		if (read_size != sizeof(wfx))
		{
			// �ǂݍ��݃T�C�Y����v���Ă��Ȃ��̂ŃG���[
			mmioClose(hmmio, MMIO_FHOPEN);
			return;
		}

		// fmt�`�����N��ޏo����
		if (mmioAscend(hmmio, &mmck_info, 0) != MMSYSERR_NOERROR)
		{
			mmioClose(hmmio, 0);
			return;
		}



		// ���f�[�^��ێ����Ă���data�`�����N�ɐi��
		// data�`�����N�ɐi��
		mmck_info.ckid = mmioFOURCC('d', 'a', 't', 'a');
		if (mmioDescend(hmmio, &mmck_info, &riffck_info, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
		{
			// �i�����s
			mmioClose(hmmio, MMIO_FHOPEN);
			return;
		}

		// ���f�[�^�̓ǂݍ���
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


		// WAV�t�@�C�������
		mmioClose(hmmio, 0);		// ����(�N���[�Y�Ώۂ̃n���h���A�I�v�V����(�ݒ�Ȃ���0))
	}

	// �T�E���h�\�[�X�̐���
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

	// �o�b�t�@�ݒ�
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = m_Length;
	bufinfo.pAudioData = m_SoundData;
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = m_PlayLength;

	// ���[�v�ݒ�
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
		//// �\�[�X�{�C�X�̏o�͐���}�X�^�[�{�C�X����T�u�~�b�N�X�{�C�X�ɐ؂�ւ���
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


	// �Đ�
	m_SourceVoice->Start();
}

void Audio::Update()
{
	if (m_mFadeFlag == true)
	{
		UpdateFade();
	}
}

// �{�����[���ݒ�p
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

// �t�F�[�h�C���p
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
	// ���[�p�X�w��
	XAUDIO2_FILTER_PARAMETERS FilterParams;		// �t�B���^�w���\����
	FilterParams.Type = XAUDIO2_FILTER_TYPE::NotchFilter;
	FilterParams.Frequency = 0.5f;
	FilterParams.OneOverQ = 0.707f;

	m_SourceVoice->SetFilterParameters(&FilterParams);

}