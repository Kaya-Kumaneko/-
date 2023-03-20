//=================================================================================================
//
// �o�f�B�����Ǘ� [buddyrun.h]
//
//=================================================================================================
#pragma once
#include "buddystate.h"
#include "input.h"

class BuddyRun : public BuddyState
{
public:
	using BuddyState::BuddyState;

	void Update() override
	{
		if (Input::GetKeyPress('W'))
		{
			m_StateBlend += 0.03f;

			if (m_StateBlend > 1.0f)
			{
				m_StateBlend = 1.0f;
			}
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

	void OnEnter() override
	{
		m_StateBlend = 1.0f - m_Parent->GetComponentBlend();
	}

	void OnExit() override
	{

	}

	const char* GetName() const override
	{
		return "Run";
	}

	float GetStateBlend() const override
	{
		return m_StateBlend;
	}

};