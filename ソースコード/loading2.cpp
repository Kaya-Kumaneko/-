//=================================================================================================
//
// ローディング２処理 [loading2.cpp]
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "loading2.h"

#include "gamesample2.h"

#include "loadinglogo2.h"

#include <thread>

void Loading2::Init()
{
	AddGameObject<LoadingLogo2>(LAYER_2D);

	std::thread th(GameSample2::Load);
	th.detach();
}

void Loading2::Uninit()
{
	Scene::Uninit();
}

void Loading2::Update()
{
	Scene::Update();

	if (GameSample2::GetLoadFinish())
	{
		Manager::SetScene<GameSample2>();
	}
}

void Loading2::Draw()
{
	Renderer::Begin();

	Scene::Draw();

	Renderer::End();
}