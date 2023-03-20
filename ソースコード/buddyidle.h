//=================================================================================================
//
// バディアイドル管理 [buddyidle.h]
//
//=================================================================================================
#pragma once
#include "buddystate.h"
#include "input.h"

class BuddyIdle : public BuddyState
{
public:
	using BuddyState::BuddyState;

	void Update() override
	{
		m_StateBlend += 0.03f;

		if (m_StateBlend > 1.0f)
		{
			m_StateBlend = 1.0f;
		}

		if (Input::GetKeyPress('W'))
		{
			m_Parent->ChangeState("Run");
		}
		if (Input::GetKeyPress('S'))
		{
			m_Parent->ChangeState("Back");
		}
		if (Input::GetKeyPress('Q'))
		{
			m_Parent->ChangeState("Left");
		}
		if (Input::GetKeyPress('E'))
		{
			m_Parent->ChangeState("Right");
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
		return "Idle";
	}

	float GetStateBlend() const override
	{
		return m_StateBlend;
	}
};