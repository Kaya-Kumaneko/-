//=================================================================================================
//
// ローディング処理 [loading.cpp]
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "loading.h"

#include "gamesample.h"

#include "loadinglogo.h"

#include <thread>

void Loading::Init()
{
	AddGameObject<LoadingLogo>(LAYER_2D);

	std::thread th(GameSample::Load);
	th.detach();
}

void Loading::Uninit()
{
	Scene::Uninit();
}

void Loading::Update()
{
	Scene::Update();

	if (GameSample::GetLoadFinish())
	{
		Manager::SetScene<GameSample>();
	}
}
void Loading::Draw()
{
	Renderer::Begin();

	Scene::Draw();

	Renderer::End();
}