//=================================================================================================
//
// ƒQ[ƒ€ƒTƒ“ƒvƒ‹‚QŠÇ— [gamesample2.h]
//
//=================================================================================================
#pragma once
#include "scene.h"

class GameSample2 : public Scene
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

	class Audio* m_bgm;
};