//=================================================================================================
//
// AIステートコンポーネント管理 [aistatecomponent.h]
//
//=================================================================================================
#pragma once
#include <string>
#include <unordered_map>
#include "component.h"
#include "aistate.h"

class AIStateComponent : public Component
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

	void RegisterState(AIState* State)
	{
		m_StateMap.emplace(State->GetName(), State);
	}

	const char* GetState()
	{
		return m_CurrentState->GetName();
	}

private:
	std::unordered_map<std::string, AIState*> m_StateMap;
	AIState* m_CurrentState;

};