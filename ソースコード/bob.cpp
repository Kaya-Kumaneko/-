//=================================================================================================
//
// ボブ処理 [bob.cpp]
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "animationmodel.h"
#include "bob.h"

AnimationModel* Bob::m_Model = NULL;

void Bob::Load()
{
	m_Model = new AnimationModel();
	m_Model->Load("asset\\model\\bob\\BobIdle.fbx");
	m_Model->LoadAnimation("asset\\model\\bob\\BobIdle.fbx", "Idle");
	m_Model->LoadAnimation("asset\\model\\bob\\BobDance.fbx", "Dance");
}


void Bob::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Bob::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 0.0f, 30.0f);
	m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.02f, 0.02f, 0.02f);

	Renderer::CreateVertexShader(&m_VertexShader[0], &m_VertexLayout, "depthShadowMappingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader[0], "depthShadowMappingPS.cso");

	Renderer::CreateVertexShader(&m_VertexShader[1], &m_VertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader[1], "unlitTexturePS.cso");
}

void Bob::Uninit()
{
	m_VertexLayout->Release();

	for (int i = 0; i < 2; i++)
	{
		m_VertexShader[i]->Release();
		m_PixelShader[i]->Release();
	}

	GameObject::Uninit();
}

void Bob::Update()
{
	GameObject::Update();

	// アニメーション用
	m_Frame++;

}

void Bob::Draw()
{
	GameObject::Draw();

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
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}