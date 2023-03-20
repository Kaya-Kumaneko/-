//=================================================================================================
//
// ƒQ[ƒ€ƒTƒ“ƒvƒ‹ŠÇ— [gamesample.h]
//
//=================================================================================================
#pragma once
#include "scene.h"

class GameSample : public Scene
{
public:
	static void Load();
	static bool GetLoadFinish() { return m_LoadFinish; }

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	static bool m_LoadFinish;
	class Transition* m_Transition;

	class Field* m_Field;
	class Player* m_Player;
	class Enemy* m_Enemy;
	class Bob* m_Bob;
};