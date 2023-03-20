//=================================================================================================
//
// ゲームサンプル処理 [gamesample.cpp]
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "gamesample.h"

#include "camera.h"
#include "skydome.h"
#include "field.h"
#include "player.h"
#include "enemy.h"
#include "bob.h"
#include "object.h"

#include "transition.h"
#include "loading2.h"

#include "audio3d.h"

#include "transition_enter.h"

bool GameSample::m_LoadFinish = false;

void GameSample::Load()
{
	Player::Load();
	Enemy::Load();
	Bob::Load();

	m_LoadFinish = true;
}

void GameSample::Init()
{
	AddGameObject<Camera>(LAYER_CAMERA);
	AddGameObject<SkyDome>(LAYER_3D);
	m_Field = AddGameObject<Field>(LAYER_3D);
	m_Player = AddGameObject<Player>(LAYER_3D);
	m_Enemy = AddGameObject<Enemy>(LAYER_3D);
	m_Bob = AddGameObject<Bob>(LAYER_3D);

	AddGameObject<Object>(LAYER_3D);

	AddGameObject<Transition_Enter>(LAYER_2D);

	Audio3D* se01 = AddGameObject<Audio3D>(LAYER_3DAUDIO);
	se01->SetEmitter(m_Enemy);
	se01->Load("asset\\audio\\se\\enemy01.wav");
	se01->Play(true);

	Audio3D* se02 = AddGameObject<Audio3D>(LAYER_3DAUDIO);
	se02->SetEmitter(m_Bob);
	se02->Load("asset\\audio\\bgm\\dance.wav");
	se02->Play(true);


	m_Transition = AddGameObject<Transition>(LAYER_2D);
	m_Transition->Start(true);
}

void GameSample::Uninit()
{
	Bob::Unload();
	Enemy::Unload();
	Player::Unload();

	Scene::Uninit();		// 継承元クラスのメソッド呼び出し

	m_LoadFinish = false;
}

void GameSample::Update()
{
	Scene::Update();

	// ゲームサンプル2へ
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		m_Transition->Start(false);
	}

	if (m_Transition->GetFinish())
	{
		Manager::SetScene<Loading2>();
	}

}

void GameSample::Draw()
{
	D3DXVECTOR3 Pos = m_Player->GetPosition();

	// ライトカメラの初期化
	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライトカメラのビュー行列を作成
	D3DXVECTOR3 lightPos = D3DXVECTOR3(-10.0f + Pos.x, 10.0f, -10.0f + Pos.z);
	D3DXVECTOR3 lightAt = D3DXVECTOR3(0.0f + Pos.x, 0.0f, 0.0f + Pos.z);
	D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&light.ViewMatrix, &lightPos,
		&lightAt, &lightUp);
	// ライトカメラのプロジェクション行列を作成
	D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 5.0f, 300.0f);

	// ライト情報をセット
	Renderer::SetLight(light);

	// 1パス目 シャドウバッファの作成
	Renderer::BeginDepth();

	// ライトカメラの行列をセット
	Renderer::SetViewMatrix(&light.ViewMatrix);
	Renderer::SetProjectionMatrix(&light.ProjectionMatrix);

	// 影を落としたいオブジェクトを描画（地面も）
	Scene* scene = Manager::GetScene();
	m_Field->Draw();
	m_Player->Draw();
	m_Enemy->Draw();



	// 2パス目 通常の描画
	Renderer::Begin();

	Scene::Draw();

	Renderer::End();
}