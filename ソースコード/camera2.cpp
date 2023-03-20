//=================================================================================================
//
// ���C������ [main.cpp]
//
//=================================================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera2.h"

#include "buddy.h"

void Camera2::Init()
{
	m_Position = D3DXVECTOR3(0.0f, 2.0f, -5.0f);
	m_Target = D3DXVECTOR3(0.0f, 0.0, 0.0f);
}

void Camera2::Uninit()
{

}

void Camera2::Update()
{
	// �v���C���[�擾
	Scene* scene = Manager::GetScene();
	Buddy* buddy = scene->GetGameObject<Buddy>(LAYER_3D);
	D3DXVECTOR3 buddyPosition = buddy->GetPosition();
	D3DXVECTOR3 buddyForward = buddy->GetForward();
	D3DXVECTOR3 buddyUpForward = buddy->GetUpForward();

	// �T�[�h�p�[�\���r���[
	m_Target = buddyPosition + buddyUpForward * 3.0;
	m_Position = buddyPosition - buddyForward * 6.0f + buddyUpForward * 4.0f;
}

void Camera2::Draw()
{
	// �r���[�}�g���N�X�ݒ�
	D3DXMATRIX viewMatrix;
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&viewMatrix, &m_Position, &m_Target, &up);

	Renderer::SetViewMatrix(&viewMatrix);

	// �v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);

	// �J�����̍��W���V�F�[�_�[�֓n��
	Renderer::SetCameraPosition(m_Position);
}