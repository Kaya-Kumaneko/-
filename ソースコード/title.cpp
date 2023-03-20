//=================================================================================================
//
// É^ÉCÉgÉãèàóù [title.cpp]
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "title.h"

#include "transition.h"

#include "polygon2d.h"
#include "pressenter.h"

#include "loading.h"

#include <thread>
#include "streamingaudio.h"

void Title::Init()
{
	AddGameObject<Polygon2D>(LAYER_2D);
	AddGameObject<PressEnter>(LAYER_2D);

	std::thread th(StreamingAudio::StreamingPlay, "asset\\audio\\bgm\\title01.wav");
	th.detach();

	m_Transition = AddGameObject<Transition>(LAYER_2D);
	m_Transition->Start(true);
}

void Title::Uninit()
{
	Scene::Uninit();
}

void Title::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		StreamingAudio::SetStopFlag(true);
		m_Transition->Start(false);
	}

	if (m_Transition->GetFinish())
	{
		Manager::SetScene<Loading>();
	}
}

void Title::Draw()
{
	Renderer::Begin();

	Scene::Draw();

	Renderer::End();
}