//=================================================================================================
//
// マネージャー処理 [manager.cpp]
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "audio.h"
#include "audio3d.h"
#include "streamingaudio.h"
#include "scene.h"

#include "title.h"

Scene* Manager::m_Scene;

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	StreamingAudio::InitStreamingMaster();
	Audio3D::InitMaster();
	Audio::InitMaster();

	SetScene<Title>();
}

void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	Audio::UninitMaster();
	Audio3D::UninitMaster();
	StreamingAudio::UninitStreamingMaster();
	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();

	m_Scene->Update();
}

void Manager::Draw()
{
	m_Scene->Draw();
}