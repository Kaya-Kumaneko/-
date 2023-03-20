//=================================================================================================
//
// �Q�[���T���v������ [gamesample.cpp]
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

	Scene::Uninit();		// �p�����N���X�̃��\�b�h�Ăяo��

	m_LoadFinish = false;
}

void GameSample::Update()
{
	Scene::Update();

	// �Q�[���T���v��2��
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

	// ���C�g�J�����̏�����
	LIGHT light;
	light.Enable = true;
	light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���C�g�J�����̃r���[�s����쐬
	D3DXVECTOR3 lightPos = D3DXVECTOR3(-10.0f + Pos.x, 10.0f, -10.0f + Pos.z);
	D3DXVECTOR3 lightAt = D3DXVECTOR3(0.0f + Pos.x, 0.0f, 0.0f + Pos.z);
	D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&light.ViewMatrix, &lightPos,
		&lightAt, &lightUp);
	// ���C�g�J�����̃v���W�F�N�V�����s����쐬
	D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 5.0f, 300.0f);

	// ���C�g�����Z�b�g
	Renderer::SetLight(light);

	// 1�p�X�� �V���h�E�o�b�t�@�̍쐬
	Renderer::BeginDepth();

	// ���C�g�J�����̍s����Z�b�g
	Renderer::SetViewMatrix(&light.ViewMatrix);
	Renderer::SetProjectionMatrix(&light.ProjectionMatrix);

	// �e�𗎂Ƃ������I�u�W�F�N�g��`��i�n�ʂ��j
	Scene* scene = Manager::GetScene();
	m_Field->Draw();
	m_Player->Draw();
	m_Enemy->Draw();



	// 2�p�X�� �ʏ�̕`��
	Renderer::Begin();

	Scene::Draw();

	Renderer::End();
}