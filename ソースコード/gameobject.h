//=================================================================================================
//
// �Q�[���I�u�W�F�N�g�Ǘ� [gameobject.h]
//
//=================================================================================================
#pragma once
#include <list>
#include <typeinfo>
#include "component.h"

class GameObject
{
public:
	GameObject() {}		// �R���X�g���N�^
	virtual ~GameObject()		// �f�X�g���N�^(���z�֐�)
	{
		// �R���|�[�l���g�̍폜
		for (auto c : m_ComponentList)
		{
			delete c;
		}
		m_ComponentList;
	}

	virtual void Init() = 0;		// �������z�֐�
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

	// �R���|�[�l���g�̒ǉ�
	template <typename T>
	T* AddComponent()
	{
		T* component = new T(this);
		component->Init();
		m_ComponentList.push_back(component);

		return component;
	}

	// �R���|�[�l���g�̍폜
	template <typename T>
	T* RemoveComponent()
	{
		m_ComponentList.remove(typeid(T));
	}

	// �R���|�[�l���g�̎擾
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
	// �x�N�g���̎擾
	//-------------------------------------------
		// �O���x�N�g��
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
	// �E���x�N�g���擾
	D3DXVECTOR3 GetRightForward()	// �E���x�N�g���擾
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 lightforward;
		lightforward.x = rot._11;
		lightforward.y = rot._12;
		lightforward.z = rot._13;

		return lightforward;
	}
	// ����x�N�g���擾
	D3DXVECTOR3 GetUpForward()	// ����x�N�g���擾
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 Upforward;
		Upforward.x = rot._21;
		Upforward.y = rot._22;
		Upforward.z = rot._23;

		return Upforward;
	}


protected:		// �A�N�Z�X�w��q
	bool m_Destroy = false;

	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation;
	D3DXVECTOR3 m_Scale;
	D3DXVECTOR3 m_Velocity;

	std::list<Component*> m_ComponentList;
};