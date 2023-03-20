//=================================================================================================
//
// エネミー処理 [enemy.cpp]
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "animationmodel.h"
#include "enemy.h"

#include "enemystatecomponent.h"
#include "enemyidle.h"
#include "enemyrun.h"
#include "enemydeath.h"
#include "enemyattack.h"

#include "player.h"

AnimationModel* Enemy::m_Model = NULL;

void Enemy::Load()
{
	m_Model = new AnimationModel();
	m_Model->Load("asset\\model\\enemy\\EnemyIdle.fbx");
	m_Model->LoadAnimation("asset\\model\\enemy\\EnemyIdle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\enemy\\EnemyRun.fbx", "Run");
	m_Model->LoadAnimation("asset\\model\\enemy\\EnemyDeath.fbx", "Death");
	m_Model->LoadAnimation("asset\\model\\enemy\\EnemyAttack.fbx", "Attack");
}


void Enemy::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Enemy::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 20.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.02f, 0.02f, 0.02f);
	m_RotVel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Renderer::CreateVertexShader(&m_VertexShader[0], &m_VertexLayout, "depthShadowMappingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader[0], "depthShadowMappingPS.cso");

	Renderer::CreateVertexShader(&m_VertexShader[1], &m_VertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader[1], "unlitTexturePS.cso");


	m_EnemyStateComponent = AddComponent<EnemyStateComponent>();
	m_EnemyStateComponent->RegisterState(new EnemyIdle(m_EnemyStateComponent));
	m_EnemyStateComponent->RegisterState(new EnemyRun(m_EnemyStateComponent));
	m_EnemyStateComponent->RegisterState(new EnemyDeath(m_EnemyStateComponent));
	m_EnemyStateComponent->RegisterState(new EnemyAttack(m_EnemyStateComponent));

	m_EnemyStateComponent->ChangeState("Idle");
}

void Enemy::Uninit()
{
	m_VertexLayout->Release();

	for (int i = 0; i < 2; i++)
	{
		m_VertexShader[i]->Release();
		m_PixelShader[i]->Release();
	}

	GameObject::Uninit();
}

void Enemy::Update()
{
	GameObject::Update();

	// アニメーション用
	m_Frame++;

	//if (cross > 0.0f)
	//{
	//	m_Rotation.y += rad;
	//}
	//else if (cross < 0.0f)
	//{
	//	m_Rotation.y -= rad;
	//}

	if (m_Rotation.y > D3DX_PI * 2.0f)
	{
		m_Rotation.y = 0.0f;
	}
	if (m_Rotation.y < -(D3DX_PI * 2.0f))
	{
		m_Rotation.y = 0.0f;
	}


	//// ステート変更
	//if ((PlayerPos.x - m_Position.x) < 1.0f &&
	//	(PlayerPos.z - m_Position.z) < 1.0f)
	//{
	//	m_EnemyStateComponent->ChangeState("Death");
	//}
	//else
	//{
	//	m_EnemyStateComponent->ChangeState("Idle");
	//}

	if (m_NewState == "Run")
	{
		m_Velocity = GetForward() * 0.1f;
	}


	// 抵抗
	m_Velocity.x -= m_Velocity.x * 0.1f;
	m_Velocity.z -= m_Velocity.z * 0.1f;

	m_Position += m_Velocity;
	m_Rotation += m_RotVel;

	if (m_Rotation.y > D3DX_PI * 2.0f)
	{
		m_Rotation.y = 0.0f;
	}
	if (m_Rotation.y < - D3DX_PI * 2.0f)
	{
		m_Rotation.y = 0.0f;
	}	
}

void Enemy::Draw()
{
	GameObject::Draw();

	m_BlendRate = m_EnemyStateComponent->GetComponentBlend();
	const char* exstate = m_EnemyStateComponent->GetState();
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