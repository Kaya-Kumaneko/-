//=================================================================================================
//
// プレイヤーステート管理 [playerstate.h]
//
//=================================================================================================
#pragma once

class PlayerState
{
public:
	PlayerState() = delete;
	PlayerState(PlayerStateComponent* StateComponent)
	{
		m_Parent = StateComponent;
	}
	virtual ~PlayerState() {};

	virtual void Update() {};
	virtual void OnEnter() {};
	virtual void OnExit() {};

	virtual const char* GetName() const = 0;
	virtual float GetStateBlend() const = 0;

protected:
	class PlayerStateComponent* m_Parent;
	float m_StateBlend = 1.0f;
};