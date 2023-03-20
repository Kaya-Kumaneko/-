//=================================================================================================
//
// エネミーアイドル管理 [enemyidle.h]
//
//=================================================================================================
#pragma once
#include "enemystate.h"
#include "input.h"

#include "player.h"

class EnemyIdle : public EnemyState
{
public:
	using EnemyState::EnemyState;

	void Update() override
	{
		m_StateBlend += 0.03f;

		if (m_StateBlend > 1.0f)
		{
			m_StateBlend = 1.0f;
		}

		// プレイヤーとの角度
		D3DXVECTOR3 PlayerPos = m_Player->GetPosition();
		D3DXVECTOR3 Vec1 = PlayerPos - m_Enemy->GetPosition();

		if ((Vec1.x > 2.0f || Vec1.z > 2.0f) ||
			(Vec1.x < 2.0f || Vec1.z < 2.0f))
		{
			m_Parent->ChangeState("Run");
		}
	}

	void OnEnter() override
	{
		m_StateBlend = 1.0f - m_Parent->GetComponentBlend();

		Scene* scene = Manager::GetScene();
		m_Player = scene->GetGameObject<Player>(LAYER_3D);

		m_Enemy = m_Parent->GetParent();

	}

	void OnExit() override
	{

	}

	const char* GetName() const override
	{
		return "Idle";
	}

	float GetStateBlend() const override
	{
		return m_StateBlend;
	}

private:
	class Player* m_Player;
	class Enemy* m_Enemy;

};