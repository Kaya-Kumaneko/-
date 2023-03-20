//=================================================================================================
//
// プレイヤー処理 [player.cpp]
//
//=================================================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "animationmodel.h"
#include "player.h"

#include "playerstatecomponent.h"
#include "playeridle.h"
#include "playerrun.h"
#include "playerback.h"
#include "playerleft.h"
#include "playerright.h"

#include "playerslash.h"

AnimationModel* Player::m_Model = NULL;

void Player::Load()
{
	m_Model = new AnimationModel();
	m_Model->Load("asset\\model\\player\\PlayerIdle.fbx");
	m_Model->LoadAnimation("asset\\model\\player\\PlayerIdle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\player\\PlayerRun.fbx", "Run");
	m_Model->LoadAnimation("asset\\model\\player\\PlayerRight.fbx", "Right");
	m_Model->LoadAnimation("asset\\model\\player\\PlayerLeft.fbx", "Left");
	m_Model->LoadAnimation("asset\\model\\player\\PlayerBack.fbx", "Back");
	m_Model->LoadAnimation("asset\\model\\player\\PlayerSlash.fbx", "Slash");
}

void Player::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Player::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.02f, 0.02f, 0.02f);

	Renderer::CreateVertexShader(&m_VertexShader[0], &m_VertexLayout, "depthShadowMappingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader[0], "depthShadowMappingPS.cso");

	Renderer::CreateVertexShader(&m_VertexShader[1], &m_VertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader[1], "unlitTexturePS.cso");


	m_PlayerStateComponent = AddComponent<PlayerStateComponent>();
	m_PlayerStateComponent->RegisterState(new PlayerIdle(m_PlayerStateComponent));
	m_PlayerStateComponent->RegisterState(new PlayerRun(m_PlayerStateComponent));
	m_PlayerStateComponent->RegisterState(new PlayerBack(m_PlayerStateComponent));
	m_PlayerStateComponent->RegisterState(new PlayerRight(m_PlayerStateComponent));
	m_PlayerStateComponent->RegisterState(new PlayerLeft(m_PlayerStateComponent));
	m_PlayerStateComponent->RegisterState(new PlayerSlash(m_PlayerStateComponent));
	m_PlayerStateComponent->ChangeState("Idle");

}

void Player::Uninit()
{
	m_VertexLayout->Release();

	for (int i = 0; i < 2; i++)
	{
		m_VertexShader[i]->Release();
		m_PixelShader[i]->Release();

	}

	GameObject::Uninit();
}

void Player::Update()
{
	GameObject::Update();

	// アニメーション用
	m_Frame++;

	// ベクトルの取得
	D3DXVECTOR3 forward = GetForward();
	D3DXVECTOR3 RightForward = GetRightForward();

	// 回転
	if (Input::GetKeyPress('D'))
	{
		m_Rotation.y += 0.03f;
	}
	if (Input::GetKeyPress('A'))
	{
		m_Rotation.y -= 0.03f;
	}
	
	// 移動
	if (Input::GetKeyPress('W'))
	{
		m_Velocity += forward * 0.025f;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Velocity -= forward * 0.02f;
	}
	if (Input::GetKeyPress('E'))
	{
		m_Velocity += RightForward * 0.02f;
	}
	if (Input::GetKeyPress('Q'))
	{
		m_Velocity -= RightForward * 0.02f;
	}
	if (((Input::GetKeyPress('W')) || (Input::GetKeyPress('S'))) &&
		((Input::GetKeyPress('Q')) || (Input::GetKeyPress('E'))))
	{
		m_Velocity *= 0.96f;
	}


	// 攻撃
	if (Input::GetKeyTrigger('I'))
	{
		if (m_PlayerStateComponent->GetState() != ("Slash"))
		{
			m_PlayerStateComponent->ChangeState("Slash");
			m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_Frame = 0;
		}
	}

	if (m_PlayerStateComponent->GetState() == "Slash")
	{
		m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}


	// 抵抗
	m_Velocity.x -= m_Velocity.x * 0.1f;
	m_Velocity.z -= m_Velocity.z * 0.1f;

	m_Position += m_Velocity;

}

void Player::Draw()
{
	GameObject::Draw();

	m_BlendRate = m_PlayerStateComponent->GetComponentBlend();
	const char* exstate = m_PlayerStateComponent->GetState();
	if (m_NewState != exstate)
	{
		m_OldState = m_NewState;
		m_NewState = exstate;
	}

	m_Model->Update(m_OldState, m_NewState, m_BlendRate, m_Frame);

	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader[0], NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader[0], NULL, 0);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader[1], NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader[1], NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y + D3DX_PI, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}