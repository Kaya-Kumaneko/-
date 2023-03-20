//=================================================================================================
//
//	エネミーステート管理 [enemystate.h]
//
//=================================================================================================
#pragma once

class EnemyState
{
public:
	EnemyState() = delete;
	EnemyState(EnemyStateComponent* StateComponent)
	{
		m_Parent = StateComponent;
	}
	virtual ~EnemyState() {};

	virtual void Update() {};
	virtual void OnEnter() {};
	virtual void OnExit() {};

	virtual const char* GetName() const = 0;
	virtual float GetStateBlend() const = 0;

protected:
	class EnemyStateComponent* m_Parent;
	float m_StateBlend = 1.0f;
};