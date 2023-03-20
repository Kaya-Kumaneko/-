//=================================================================================================
//
// メイン処理 [main.cpp]
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"

#include "player.h"

void Camera::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 2.0f, -5.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0, 0.0f);
}

void Camera::Uninit()
{

}

void Camera::Update()
{
	// プレイヤー取得
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>(LAYER_3D);
	D3DXVECTOR3 playerPosition = player->GetPosition();
	D3DXVECTOR3 playerForward = player->GetForward();
	D3DXVECTOR3 playerUpForward = player->GetUpForward();

	// サードパーソンビュー
	m_Target = playerPosition + playerUpForward * 3.0;
	m_Position = playerPosition - playerForward * 6.0f + playerUpForward * 4.0f;
}

void Camera::Draw()
{
	// ビューマトリクス設定
	D3DXMATRIX viewMatrix;
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&viewMatrix, &m_Position, &m_Target, &up);

	Renderer::SetViewMatrix(&viewMatrix);

	// プロジェクションマトリクス設定
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);

	// カメラの座標をシェーダーへ渡す
	Renderer::SetCameraPosition(m_Position);
}