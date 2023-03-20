//=================================================================================================
//
// プレイヤースラッシュ管理 [playerslashrun.h]
//
//=================================================================================================
#pragma once
#include "playerstate.h"
#include "input.h"

class PlayerSlash : public PlayerState
{
public:
	using PlayerState::PlayerState;

	void Update() override
	{
		m_StateBlend += 0.03f;
		m_Frame++;

		if (m_StateBlend > 1.0f)
		{
			m_StateBlend = 1.0f;
		}
		if (m_Frame > 45)
		{
			if (Input::GetKeyPress('W'))
			{
				m_Parent->ChangeState("Run");
			}
			else if (Input::GetKeyPress('S'))
			{
				m_Parent->ChangeState("Back");
			}
			else if (Input::GetKeyPress('Q'))
			{
				m_Parent->ChangeState("Left");
			}
			else if (Input::GetKeyPress('E'))
			{
				m_Parent->ChangeState("Right");
			}
			else
			{
				m_Parent->ChangeState("Idle");
			}
		}

	}

	void OnEnter() override
	{
		//m_StateBlend = 1.0f - m_Parent->GetComponentBlend();
		m_StateBlend = 1.0f;
		m_Frame = 0;
	}

	void OnExit() override
	{

	}

	const char* GetName() const override
	{
		return "Slash";
	}

	float GetStateBlend() const override
	{
		return m_StateBlend;
	}

private:
	int m_Frame = 0;
};