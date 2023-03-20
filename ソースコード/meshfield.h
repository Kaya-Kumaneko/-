//=================================================================================================
//
// メッシュフィールド管理 [meshfield.h]
//
//=================================================================================================
#pragma once
#include "gameobject.h"

class MeshField : public GameObject
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	float GetHeight(D3DXVECTOR3 Position);

private:
	VERTEX_3D m_Vertex[21][21];

	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

};