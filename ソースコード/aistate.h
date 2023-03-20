//=================================================================================================
//
// AIステート管理 [aistate.h]
//
//=================================================================================================
#pragma once

class AIState
{
public:
	AIState() = delete;
	AIState(AIStateComponent* StateComponent)
	{
		m_Parent = StateComponent;
	}
	virtual ~AIState() {};

	virtual void Update() {};
	virtual void OnEnter() {};
	virtual void OnExit() {};

	virtual const char* GetName() const = 0;

protected:
	class AIStateComponent* m_Parent;
};