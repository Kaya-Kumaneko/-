//=================================================================================================
//
// �v���C���[�Ǘ� [player.h]
//
//=================================================================================================
#pragma once
#include "gameobject.h"

class Player : public GameObject
{
public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	static class AnimationModel* m_Model;
	class PlayerStateComponent* m_PlayerStateComponent;

	ID3D11VertexShader* m_VertexShader[2];
	ID3D11PixelShader* m_PixelShader[2];
	ID3D11InputLayout* m_VertexLayout;

	int m_Frame = 0;
	float m_BlendRate = 0.0f;

	const char* m_OldState = "Idle";
	const char* m_NewState = "Idle";

	class Enemy* m_Enemy;
};