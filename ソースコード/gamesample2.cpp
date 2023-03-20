//=================================================================================================
//
// �Q�[���T���v���Q���� [gamesample2.cpp]
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "gamesample2.h"

#include "camera2.h"
#include "skydome2.h"
#include "meshfield.h"

#include "buddy.h"

#include "transition.h"

#include "audio.h"

#include "setumei.h"

bool GameSample2::m_LoadFinish = false;

void GameSample2::Load()
{
	Buddy::Load();

	m_LoadFinish = true;
}

void GameSample2::Init()
{
	AddGameObject<Camera2>(LAYER_CAMERA);
	AddGameObject<SkyDome2>(LAYER_3D);
	AddGameObject<MeshField>(LAYER_3D);
	AddGameObject<Buddy>(LAYER_3D);

	m_Transition = AddGameObject<Transition>(LAYER_2D);
	m_Transition->Start(true);
	
	m_bgm = AddGameObject<Audio>(LAYER_2DAUDIO);
	m_bgm->Load("asset\\audio\\bgm\\battle.wav");

	AddGameObject<Setumei>(LAYER_2D);
}

void GameSample2::Uninit()
{
	Buddy::Unload();

	Scene::Uninit();		// �p�����N���X�̃��\�b�h�Ăяo��

	m_LoadFinish = false;

}

void GameSample2::Update()
{
	Scene::Update();

	//if (Input::GetKeyTrigger('P'))
	//{
	//	m_bgm->Play(true);
	//	m_bgm->SetVol(0.0f);
	//	m_bgm->Fade(0.5f, 300.0f);
	//}

	//if (Input::GetKeyTrigger('O'))
	//{
	//	m_bgm->Fade(0.1f, 1200.0f);
	//}

	if (Input::GetKeyTrigger('I'))
	{
		m_bgm->Pause();
	}

	if (Input::GetKeyTrigger('O'))
	{
		m_bgm->ReStart();
	}

	// ReSTART
	if (Input::GetKeyTrigger('P'))
	{
		m_bgm->Play(true);
	}

	//if (Input::GetKeyTrigger('F'))
	//{
	//	m_bgm->SetFilter();
	//}

}

void GameSample2::Draw()
{

	//// ���C�g�J�����\���̂̏������@�ŏ��̓��C�g�֘A�̐��l�����̒l�ɂ��Ă���
	//LIGHT light;
	//light.Enable = true;
	//light.Direction = D3DXVECTOR4(1.0f, -1.0f, 1.0f, 0.0f);		// ���̐i�ޕ���
	//D3DXVec4Normalize(&light.Direction, &light.Direction);
	//light.Ambient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);		// ���̓�����Ȃ������̔��ˌW��
	//light.Diffuse = D3DXCOLOR(2.0f, 2.0f, 2.0f, 2.0f);		// ���̓����镔���̔��ˌW��

	////���C�g�J�����̃r���[�s����쐬
	//D3DXVECTOR3 lightPos = D3DXVECTOR3(-10.0f, 10.0f, -10.0f);
	//D3DXVECTOR3 lightAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 lightUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//D3DXMatrixLookAtLH(&light.ViewMatrix, &lightPos,
	//	&lightAt, &lightUp);
	////���C�g�J�����̃v���W�F�N�V�����s����쐬
	//D3DXMatrixPerspectiveFovLH(&light.ProjectionMatrix, 1.0f,
	//	(float)SCREEN_WIDTH / SCREEN_HEIGHT, 5.0f, 30.0f);

	////���C�g�����Z�b�g
	//Renderer::SetLight(light);
	Renderer::Begin();

	Scene::Draw();

	Renderer::End();
}