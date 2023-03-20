//=================================================================================================
//
// �����_���[�Ǘ� [renderer.h]
//
//=================================================================================================
#pragma once

// ���_�\����
struct VERTEX_3D
{
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR4 Diffuse;
	D3DXVECTOR2 TexCoord;
};

// �}�e���A���\����
struct MATERIAL
{
	D3DXCOLOR Ambient;
	D3DXCOLOR Diffuse;
	D3DXCOLOR Specular;
	D3DXCOLOR Emission;
	float Shininess;
	float Dummy[3];		// 16byte���E�p
};

// �}�e���A���\����
struct DX11_MODEL_MATERIAL
{
	MATERIAL Material;
	class CTexture* Texture;
};

// �`��T�u�Z�b�g
struct DX11_SUBSET
{
	unsigned int StartIndex;
	unsigned int IndexNum;
	DX11_MODEL_MATERIAL Material;
};

struct LIGHT
{
	BOOL Enable;
	BOOL Dummy[3];		// 16byte���E�p
	D3DXVECTOR4 Direction;
	D3DXCOLOR Diffuse;
	D3DXCOLOR Ambient;

	D3DXMATRIX ViewMatrix;		// ���C�g�J�����s��
	D3DXMATRIX ProjectionMatrix;		// ���C�g�v���W�F�N�V�����s��
};

class CVertexBuffer;
class CIndexBuffer;
class CTexture;

class Renderer
{
public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX* WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX* ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);

	static void SetCameraPosition(D3DXVECTOR3 CameraPosition);
	static void SetParameter(D3DXVECTOR4 Parameter);

	static ID3D11Device* GetDevice(void) { return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext(void) { return m_DeviceContext; }

	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileNmae);

	// �ǉ� �e�`��p
	static ID3D11ShaderResourceView* GetShadowDepthTexture()
	{
		return m_ShadowDepthShaderResourceView;
	}

	static void BeginDepth()
	{
		//�V���h�E�o�b�t�@��[�x�o�b�t�@�ɐݒ肵�A���e��1�œh��Ԃ����Ă���
		m_DeviceContext->OMSetRenderTargets(0, NULL, m_ShadowDepthStencilView);
		m_DeviceContext->ClearDepthStencilView(m_ShadowDepthStencilView,
			D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

private:
	static D3D_FEATURE_LEVEL m_FeatureLevel;
	
	static ID3D11Device* m_Device;
	static ID3D11DeviceContext* m_DeviceContext;
	static IDXGISwapChain* m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11Buffer* m_WorldBuffer;
	static ID3D11Buffer* m_ViewBuffer;
	static ID3D11Buffer* m_ProjectionBuffer;
	static ID3D11Buffer* m_MaterialBuffer;
	static ID3D11Buffer* m_LightBuffer;
	static ID3D11Buffer* m_CameraBuffer;
	static ID3D11Buffer* m_ParameterBuffer;

	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	// �ǉ� �e�`��p
	static ID3D11DepthStencilView* m_ShadowDepthStencilView;
	static ID3D11ShaderResourceView* m_ShadowDepthShaderResourceView;

};