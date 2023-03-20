//=================================================================================================
//
// �A�j���[�V�������f���Ǘ� [animationmodel.h]
//
//=================================================================================================
#pragma once
#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp.lib")

// �ό`�㒸�_�\����
struct DEFORM_VERTEX
{
	aiVector3D Position;
	aiVector3D Normal;
	int BoneNum;
	std::string BoneName[4];		// �{���̓{�[���C���f�b�N�X�ŊǗ�����ׂ�
	float BoneWeight[4];
};

// �{�[���\����
struct BONE
{
	aiMatrix4x4 Matrix;
	aiMatrix4x4 AnimationMatrix;
	aiMatrix4x4 OffsetMatrix;
};

class AnimationModel
{
public:
	void Load(const char* FileName);
	void LoadAnimation(const char* FileName, const char* AnimationName);
	void Unload();
	void Update(const char* AnimationName1, const char* AnimationName2, float BlendRate, int Frame);
	void Draw();

private:
	const aiScene* m_AiScene = NULL;
	ID3D11Buffer** m_VertexBuffer;
	ID3D11Buffer** m_IndexBuffer;


	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;		// �ό`�㒸�_�f�[�^
	std::unordered_map<std::string, BONE> m_Bone;		// �{�[���f�[�^�i���O�ŎQ�Ɓj

	std::unordered_map<std::string, const aiScene*> m_Animation;
	
	void CreateBone(aiNode* node);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);
};