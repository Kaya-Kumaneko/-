//=================================================================================================
//
// エネミーデス管理 [enemydeath.h]
//
//=================================================================================================
#pragma once
#include "enemystate.h"
#include "input.h"

class EnemyDeath : public EnemyState
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
	}

	void OnEnter() override
	{
		m_StateBlend = 1.0f - m_Parent->GetComponentBlend();
	}

	void OnExit() override
	{

	}

	const char* GetName() const override
	{
		return "Death";
	}

	float GetStateBlend() const override
	{
		return m_StateBlend;
	}
};