//=================================================================================================
//
// �v���C���[���t�g�Ǘ� [playerleft.h]
//
//=================================================================================================
#pragma once
#include "playerstate.h"
#include "input.h"

class PlayerLeft : public PlayerState
{
public:
	using PlayerState::PlayerState;

	void Update() override
	{
		if (Input::GetKeyPress('Q'))
		{
			m_StateBlend += 0.03f;

			if (m_StateBlend > 1.0f)
			{
				m_StateBlend = 1.0f;
			}
		}
		else if (Input::GetKeyPress('W'))
		{
			m_Parent->ChangeState("Run");
		}
		else if (Input::GetKeyPress('S'))
		{
			m_Parent->ChangeState("Back");
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

	void OnEnter() override
	{
		m_StateBlend = 1.0f - m_Parent->GetComponentBlend();
	}

	void OnExit() override
	{

	}

	const char* GetName() const override
	{
		return "Left";
	}

	float GetStateBlend() const override
	{
		return m_StateBlend;
	}

};