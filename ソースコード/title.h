//=================================================================================================
//
// �^�C�g���Ǘ� [title.h]
//
//=================================================================================================
#pragma once
#include "scene.h"

class Title : public Scene
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	class Transition* m_Transition;
};