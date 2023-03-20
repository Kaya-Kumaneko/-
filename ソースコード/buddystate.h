//=================================================================================================
//
// バディステート管理 [buddystate.h]
//
//=================================================================================================
#pragma once

class BuddyState
{
public:
	BuddyState() = delete;
	BuddyState(BuddyStateComponent* StateComponent)
	{
		m_Parent = StateComponent;
	}
	virtual ~BuddyState() {};

	virtual void Update() {};
	virtual void OnEnter() {};
	virtual void OnExit() {};

	virtual const char* GetName() const = 0;
	virtual float GetStateBlend() const = 0;

protected:
	class BuddyStateComponent* m_Parent;
	float m_StateBlend = 1.0f;
};