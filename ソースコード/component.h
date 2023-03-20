//=================================================================================================
//
// コンポーネント管理 [component.h]
//
//=================================================================================================
#pragma once

class GameObject;

class Component
{
public:
	Component() = delete;
	Component(GameObject* GameObject)
	{
		m_Parent = GameObject;
	}
	virtual ~Component() {}

	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}

protected:
	GameObject* m_Parent;
};