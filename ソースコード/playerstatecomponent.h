//=================================================================================================
//
// プレイヤーステートコンポーネント管理 [playerstatecomponent.h]
//
//=================================================================================================
#pragma once
#include <string>
#include <unordered_map>
#include "component.h"
#include "playerstate.h"

class PlayerStateComponent : public Component
{
public:
	using Component::Component;

	void Update() override
	{
		if (m_CurrentState)
		{
			m_CurrentState->Update();
		}
	}

	void ChangeState(const std::string& name)
	{
		if (m_CurrentState)
		{
			m_CurrentState->OnExit();
		}

		auto iter = m_StateMap.find(name);
		if (iter != m_StateMap.end())
		{
			m_CurrentState = iter->second;
			m_CurrentState->OnEnter();
		}
		else
		{
			m_CurrentState = nullptr;
		}
	}

	void RegisterState(PlayerState* State)
	{
		m_StateMap.emplace(State->GetName(), State);
	}

	const char* GetState()
	{
		return m_CurrentState->GetName();
	}

	float GetComponentBlend()
	{
		m_ComponentBlend = m_CurrentState->GetStateBlend();
		return m_ComponentBlend;
	}

private:
	std::unordered_map<std::string, PlayerState*> m_StateMap;
	PlayerState* m_CurrentState;
	float m_ComponentBlend = 0.0f;
};