//=================================================================================================
//
// É{Éuä«óù [bob.h]
//
//=================================================================================================
#pragma once
#include "gameobject.h"

class Bob : public GameObject
{
public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	static class AnimationModel* m_Model;
	class EnemyStateComponent* m_EnemyStateComponent;

	ID3D11VertexShader* m_VertexShader[2];
	ID3D11PixelShader* m_PixelShader[2];
	ID3D11InputLayout* m_VertexLayout;

	int m_Frame = 0;
	float m_BlendRate = 1.0f;

	const char* m_OldState = "Idle";
	const char* m_NewState = "Dance";
};