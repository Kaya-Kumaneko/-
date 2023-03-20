//=================================================================================================
//
// ストリーミングオーディオ処理 [streamingaudio.cpp]
//
//=================================================================================================
#include "main.h"
#include "streamingaudio.h"

// Helper macrose
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=nullptr; } }
#endif

#define STREAMBUFFERSIZE 10240
#define BUFFERNUM 5
BYTE streamBuffers[BUFFERNUM][STREAMBUFFERSIZE];

IXAudio2* StreamingAudio::m_XAudio = NULL;
IXAudio2MasteringVoice* StreamingAudio::m_MasteringVoice = NULL;


int StreamingAudio::m_CurrentBufferIndex = 0;
bool StreamingAudio::m_StopFlag = false;

void StreamingAudio::InitStreamingMaster()
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

void StreamingAudio::UninitStreamingMaster()
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

void StreamingAudio::StreamingPlay(const char* FileName)
{
	// スレッド名の設定
	HRESULT hr = SetThreadDescription(
		GetCurrentThread(), L"ストリーミングスレッド"
	);


	HMMIO hmmio = NULL;
	MMIOINFO mmioinfo = { 0 };
	MMCKINFO riffchunkinfo = { 0 };
	MMCKINFO datachunkinfo = { 0 };
	MMCKINFO mmckinfo = { 0 };

	hmmio = mmioOpen((LPSTR)FileName, &mmioinfo, MMIO_READ);
	assert(hmmio);

	riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

	mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

	// WaveFormat情報の読み込み
	WAVEFORMATEX wfx = { 0 };

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

	StreamingVoiceContext callback;
	IXAudio2SourceVoice* pSourceVoice;
	m_XAudio->CreateSourceVoice(
		&pSourceVoice,
		&wfx,
		0,				// UINT32 Flags = 0;
		1.0f,	// float MaxFrequencyRatio = XAUDIO2_DEFAULT_FREQ_RATIO
		&callback
	);

	datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);

	int fileSize = datachunkinfo.cksize;
	int currentPos = 0;
	while (currentPos < fileSize)
	{
		DWORD size = STREAMBUFFERSIZE;
		mmioRead(hmmio, (HPSTR)streamBuffers[m_CurrentBufferIndex], size);

		currentPos += size;

		XAUDIO2_BUFFER buffer = { 0 };
		buffer.AudioBytes = size;
		buffer.pAudioData = streamBuffers[m_CurrentBufferIndex];

		pSourceVoice->SubmitSourceBuffer(&buffer);

		pSourceVoice->SetVolume(0.5f * 0.5f);

		pSourceVoice->Start(XAUDIO2_COMMIT_NOW);

		XAUDIO2_VOICE_STATE state;
		pSourceVoice->GetState(&state);
		while (state.BuffersQueued > BUFFERNUM - 1)
		{
			WaitForSingleObject(callback.hBufferEndEvent, INFINITE);
			pSourceVoice->GetState(&state);

			if (m_StopFlag == true)
			{
				pSourceVoice->Stop();
				pSourceVoice->DestroyVoice();
				mmioClose(hmmio, MMIO_FHOPEN);

				m_StopFlag = false;
				return;
			}
		}
		m_CurrentBufferIndex++;
		m_CurrentBufferIndex %= BUFFERNUM;
	}

	XAUDIO2_VOICE_STATE state;
	while (pSourceVoice->GetState(&state), state.BuffersQueued > 0)
	{
		WaitForSingleObject(callback.hBufferEndEvent, INFINITE);
	}
	pSourceVoice->DestroyVoice();

	mmioClose(hmmio, MMIO_FHOPEN);

}