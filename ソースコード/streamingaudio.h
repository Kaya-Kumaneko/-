//=================================================================================================
//
// ストリーミングオーディオ管理 [streamingaudio.h]
//
//=================================================================================================
#pragma once

#include <xaudio2.h>
#include "gameobject.h"

// コールバック
class StreamingVoiceContext : public IXAudio2VoiceCallback
{
public:
	HANDLE hBufferEndEvent;
	StreamingVoiceContext() : hBufferEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL)) {}
	~StreamingVoiceContext() { CloseHandle(hBufferEndEvent); }

	void STDMETHODCALLTYPE OnStreamEnd() {/*SetEvent(hBufferEndEvent);*/ }
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() {}
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired) {}
	// Called when the voice has just finished playing an audio buffer
	void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) { SetEvent(hBufferEndEvent); }
	void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext) {};
	void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext) {/*SetEvent(hBufferEndEvent);*/ }
	void STDMETHODCALLTYPE OnVoiceError(void* pBufferContext, HRESULT Error) {}
};

class StreamingAudio : public GameObject
{
public:
	static void InitStreamingMaster();
	static void UninitStreamingMaster();

	void Init() {};
	void Uninit() {};
	void Update() {};
	void Draw() {};

	static void StreamingPlay(const char* FileName);
	static void SetStopFlag(bool flag = true) { m_StopFlag = flag; }

private:
	static IXAudio2* m_XAudio;
	static IXAudio2MasteringVoice* m_MasteringVoice;

	static int m_CurrentBufferIndex;
	static bool m_StopFlag;
};