//=================================================================================================
//
// エネミー管理 [enemy.h]
//
//=================================================================================================
#pragma once
#include "gameobject.h"

class Enemy : public GameObject
{
public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetRotVel(D3DXVECTOR3 rot)
	{
		m_RotVel = rot;
	}

private:
	D3DXVECTOR3 m_RotVel;

	static class AnimationModel* m_Model;
	class EnemyStateComponent* m_EnemyStateComponent;
	class Player* m_Player;

	ID3D11VertexShader* m_VertexShader[2];
	ID3D11PixelShader* m_PixelShader[2];
	ID3D11InputLayout* m_VertexLayout;

	int m_Frame = 0;
	float m_BlendRate = 0.0f;

	const char* m_OldState = "Idle";
	const char* m_NewState = "Idle";

	int m_HP = 0;
};