//=================================================================================================
//
// �V�[���Ǘ� [scene.h]
//
//=================================================================================================
#pragma once
#include <list>
#include <vector>
#include <typeinfo>
#include "gameobject.h"

typedef enum
{
	LAYER_CAMERA,
	LAYER_3DAUDIO,
	LAYER_2DAUDIO,
	LAYER_3D,
	LAYER_2D,

	LAYER_MAX
}LAYER;


class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Init() = 0;

	virtual void Uninit()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Uninit();
				delete object;
			}
			m_GameObject[i].clear();		// ���X�g�̃N���A
		}
	}

	virtual void Update()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (GameObject* object : m_GameObject[i])		// �͈�for���[�v
			{
				object->Update();
			}

			m_GameObject[i].remove_if([](GameObject* object) {return object->Destroy(); });		// �����_��
		}
	}

	virtual void Draw()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Draw();
			}
		}
	}

	template <typename T>		// �e���v���[�g�֐�
	T* AddGameObject(int Layer)
	{
		T* gameobject = new T();
		m_GameObject[Layer].push_back(gameobject);
		gameobject->Init();

		return gameobject;
	}

	template <typename T>
	T* GetGameObject(int Layer)
	{
		for (GameObject* object : m_GameObject[Layer])
		{
			if (typeid(*object) == typeid(T))		// �^�𒲂ׂ�(RTTI���I�^���)
			{
				return (T*)object;
			}
		}
		return NULL;
	}

	template <typename T>
	std::vector<T*> GetGameObjects(int Layer)
	{
		std::vector<T*> objects;		// STL�̔z��
		for (GameObject* object : m_GameObject[Layer])
		{
			if (typeid(*object) == typeid(T))
			{
				objects.push_back((T*)object);
			}
		}
		return objects;
	}

protected:
	std::list<GameObject*> m_GameObject[LAYER_MAX];		// STL�̃��X�g�\���̔z��
};
