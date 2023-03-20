//=================================================================================================
//
// バディ処理 [buddy.cpp]
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "animationmodel.h"
#include "buddy.h"

#include "buddystatecomponent.h"
#include "buddyidle.h"
#include "buddyrun.h"
#include "buddyback.h"
#include "buddyleft.h"
#include "buddyright.h"


#include "meshfield.h"

AnimationModel* Buddy::m_Model = NULL;

void Buddy::Load()
{
	m_Model = new AnimationModel();
	m_Model->Load("asset\\model\\buddy\\BuddyIdle.fbx");
	m_Model->LoadAnimation("asset\\model\\buddy\\BuddyIdle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\buddy\\BuddyRun.fbx", "Run");
	m_Model->LoadAnimation("asset\\model\\buddy\\BuddyRight.fbx", "Right");
	m_Model->LoadAnimation("asset\\model\\buddy\\BuddyLeft.fbx", "Left");
	m_Model->LoadAnimation("asset\\model\\buddy\\BuddyBack.fbx", "Back");
}

void Buddy::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Buddy::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.03f, 0.03f, 0.03f);

	Renderer::CreateVertexShader(&m_VertexShader[0], &m_VertexLayout, "vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader[0], "vertexLightingPS.cso");
	Renderer::CreateVertexShader(&m_VertexShader[1], &m_VertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader[1], "unlitTexturePS.cso");

	m_BuddyStateComponent = AddComponent<BuddyStateComponent>();
	m_BuddyStateComponent->RegisterState(new BuddyIdle(m_BuddyStateComponent));
	m_BuddyStateComponent->RegisterState(new BuddyRun(m_BuddyStateComponent));
	m_BuddyStateComponent->RegisterState(new BuddyBack(m_BuddyStateComponent));
	m_BuddyStateComponent->RegisterState(new BuddyRight(m_BuddyStateComponent));
	m_BuddyStateComponent->RegisterState(new BuddyLeft(m_BuddyStateComponent));
	m_BuddyStateComponent->ChangeState("Idle");
}

void Buddy::Uninit()
{
	m_VertexLayout->Release();

	for (int i = 0; i < 2; i++)
	{
		m_VertexShader[i]->Release();
		m_PixelShader[i]->Release();

	}

	GameObject::Uninit();
}

void Buddy::Update()
{
	GameObject::Update();

	Scene* scene = Manager::GetScene();

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
		m_Velocity += forward * 0.02f;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Velocity -= forward * 0.01f;
	}
	if (Input::GetKeyPress('E'))
	{
		m_Velocity += RightForward * 0.018f;
	}
	if (Input::GetKeyPress('Q'))
	{
		m_Velocity -= RightForward * 0.018f;
	}

	if (((Input::GetKeyPress('W')) || (Input::GetKeyPress('S'))) &&
		((Input::GetKeyPress('Q')) || (Input::GetKeyPress('E'))))
	{
		m_Velocity *= 0.96f;
	}

	// 重力
	m_Velocity.y -= 0.01f;

	// 抵抗
	m_Velocity.x -= m_Velocity.x * 0.1f;
	m_Velocity.y -= m_Velocity.y * 0.01f;
	m_Velocity.z -= m_Velocity.z * 0.1f;

	m_Position += m_Velocity;

	
	// メッシュフィールドの高さを得る
	MeshField* meshfield = scene->GetGameObject<MeshField>(LAYER_3D);
	float height = meshfield->GetHeight(m_Position);

	if (m_Position.y < height)
	{
		m_Position.y = height;
	}
}

void Buddy::Draw()
{
	GameObject::Draw();

	m_BlendRate = m_BuddyStateComponent->GetComponentBlend();
	const char* exstate = m_BuddyStateComponent->GetState();
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