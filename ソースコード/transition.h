//=================================================================================================
//
// ÉgÉâÉìÉWÉVÉáÉìä«óù [transition.h]
//
//=================================================================================================
#pragma once
#include "gameobject.h"

class Transition : public GameObject
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void Start(bool In)
	{
		m_In = In;
		m_Finish = false;

		if (m_In)
		{
			m_Count = 30;
		}
		else
		{
			m_Count = 0;
		}
	}

	bool GetFinish() { return m_Finish; }

private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

	int m_Count;
	bool m_In;
	bool m_Finish;
};