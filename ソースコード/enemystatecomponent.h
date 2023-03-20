//=================================================================================================
//
// エネミーステートコンポーネント管理 [enemystatecomponent.h]
//
//=================================================================================================
#pragma once
#include <string>
#include <unordered_map>
#include "component.h"
#include "enemystate.h"

class EnemyStateComponent : public Component
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

	void RegisterState(EnemyState* State)
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

	Enemy* GetParent()
	{
		return (Enemy*)m_Parent;
	}

private:
	std::unordered_map<std::string, EnemyState*> m_StateMap;
	EnemyState* m_CurrentState;
	float m_ComponentBlend = 0.0f;
};