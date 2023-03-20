//=================================================================================================
//
// スカイドーム管理 [skydome.h]
//
//=================================================================================================
#pragma once
#include "gameobject.h"

class SkyDome : public GameObject
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	class Model* m_Model;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_VertexLayout;

};