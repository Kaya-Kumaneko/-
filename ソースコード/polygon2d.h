//=================================================================================================
//
// É|ÉäÉSÉì2Dä«óù [polygon2d.h]
//
//=================================================================================================
#pragma once
#include "gameobject.h"

class Polygon2D : public GameObject
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11ShaderResourceView* m_Texture = NULL;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

};