//=================================================================================================
//
// ゲームオブジェクト管理 [gameobject.h]
//
//=================================================================================================
#pragma once
#include <list>
#include <typeinfo>
#include "component.h"

class GameObject
{
public:
	GameObject() {}		// コンストラクタ
	virtual ~GameObject()		// デストラクタ(仮想関数)
	{
		// コンポーネントの削除
		for (auto c : m_ComponentList)
		{
			delete c;
		}
		m_ComponentList;
	}

	virtual void Init() = 0;		// 純粋仮想関数
	virtual void Uninit()
	{
		for (auto c : m_ComponentList)
		{
			c->Uninit();
		}
	}
	virtual void Update()
	{
		for (auto c : m_ComponentList)
		{
			c->Update();
		}
	}
	virtual void Draw()
	{
		for (auto c : m_ComponentList)
		{
			c->Draw();
		}
	}

	// コンポーネントの追加
	template <typename T>
	T* AddComponent()
	{
		T* component = new T(this);
		component->Init();
		m_ComponentList.push_back(component);

		return component;
	}

	// コンポーネントの削除
	template <typename T>
	T* RemoveComponent()
	{
		m_ComponentList.remove(typeid(T));
	}

	// コンポーネントの取得
	template <typename T>
	T* GetComponent()
	{
		for (auto c : m_ComponentList)
		{
			if (typeid(*c) == typeid(T))
			{
				return (T*)c;
			}
		}
		return nullptr;
	}

	//===========================================
	// Getter/Setter
	//===========================================
	D3DXVECTOR3 GetPosition() { return m_Position; }
	D3DXVECTOR3 GetVelocity() { return m_Velocity; }
	D3DXVECTOR3 GetRotation() { return m_Rotation; }
	void SetPosition(D3DXVECTOR3 Position) { m_Position = Position; }
	void SetVelocity(D3DXVECTOR3 Velocity) { m_Velocity = Velocity; }
	void SetRotation(D3DXVECTOR3 Rotation) { m_Rotation = Rotation; }
	void SetDestroy() { m_Destroy = true; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			Uninit();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}

	//-------------------------------------------
	// ベクトルの取得
	//-------------------------------------------
		// 前方ベクトル
	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;

		return forward;
	}
	// 右方ベクトル取得
	D3DXVECTOR3 GetRightForward()	// 右方ベクトル取得
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 lightforward;
		lightforward.x = rot._11;
		lightforward.y = rot._12;
		lightforward.z = rot._13;

		return lightforward;
	}
	// 上方ベクトル取得
	D3DXVECTOR3 GetUpForward()	// 上方ベクトル取得
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 Upforward;
		Upforward.x = rot._21;
		Upforward.y = rot._22;
		Upforward.z = rot._23;

		return Upforward;
	}


protected:		// アクセス指定子
	bool m_Destroy = false;

	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation;
	D3DXVECTOR3 m_Scale;
	D3DXVECTOR3 m_Velocity;

	std::list<Component*> m_ComponentList;
};