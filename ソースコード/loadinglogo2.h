//=================================================================================================
//
// ローディングロゴ2管理 [loadinglogo2.h]
//
//=================================================================================================
#pragma once
#include "gameobject.h"

class LoadingLogo2 : public GameObject
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