//=================================================================================================
//
// インプット管理 [input.h]
//
//=================================================================================================
#pragma once

class Input
{
public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress(BYTE KeyCode);
	static bool GetKeyTrigger(BYTE KeyCode);

private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];
};