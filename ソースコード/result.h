//=================================================================================================
//
// ƒŠƒUƒ‹ƒgŠÇ— [result.h]
//
//=================================================================================================
#pragma once
#include "scene.h"

class Result : public Scene
{
public:
	void Init();
	void Uninit();
	void Update();

private:
	class Transition* m_Transition;
};