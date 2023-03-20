//=================================================================================================
//
// ÉJÉÅÉâ2ä«óù [camera2.h]
//
//=================================================================================================
#pragma once
#include "gameobject.h"

class Camera2 : public GameObject
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	D3DXMATRIX GetViewMatrix()
	{
		return m_ViewMatrix;
	}

	D3DXVECTOR3 GetTarget()
	{
		return m_Target;
	}

private:
	D3DXVECTOR3 m_Target;
	D3DXMATRIX m_ViewMatrix;
};