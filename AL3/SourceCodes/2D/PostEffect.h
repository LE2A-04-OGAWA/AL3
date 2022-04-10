#pragma once
#include "Sprite.h"
#include "Texture.h"
#include <array>

//AlphaBlendMode
enum AlphaBlendMode {
	ALPHA_BLENDMODE_NONE,	//����
	ALPHA_BLENDMODE_TRANS,	//����������
	ALPHA_BLENDMODE_ADD,	//���Z����
};

//���̍\����
struct SpriteInitData {
	std::array<const wchar_t*, 100> m_fileName = { nullptr };				//�t�@�C���p�X��
	std::array<Texture*, 100> m_textures = { nullptr };						//�g�p����e�N�X�`��
	const char* m_vsEntryPoint = "VSmain";								//���_�V�F�[�_�[�̃G���g���[�|�C���g
	const char* m_psEntryPoint = "PSmain";								//�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
	const wchar_t* m_vsShaderName =
		L"Resources/Shaders/PostEffectTestVS.hlsl";						//�ǂݍ���vs�̖��O
	const wchar_t* m_psShaderName =
		L"Resources/Shaders/PostEffectTestPS.hlsl";						//�ǂݍ���ps�̖��O
	UINT m_width = 0;													//�X�v���C�g�̕��B
	UINT m_height = 0;													//�X�v���C�g�̍����B
	void* m_expandConstantBuffer = nullptr;								//���[�U�[�g���̒萔�o�b�t�@
	int m_expandConstantBufferSize = 0;									//���[�U�[�g���̒萔�o�b�t�@�̃T�C�Y�B
	//IShaderResource* m_expandShaderResoruceView = nullptr;			//���[�U�[�g���̃V�F�[�_�[���\�[�X�B
	AlphaBlendMode m_alphaBlendMode = ALPHA_BLENDMODE_NONE;				//AlphaBlendMode
	//D3D12_FILTER m_samplerFilter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;	//�T���v���̃t�B���^�[�B
	//std::array<DXGI_FORMAT, MAX_RENDERING_TARGET> m_colorBufferFormat = {
	//	DXGI_FORMAT_R8G8B8A8_UNORM,
	//	DXGI_FORMAT_UNKNOWN,
	//	DXGI_FORMAT_UNKNOWN,
	//	DXGI_FORMAT_UNKNOWN,
	//	DXGI_FORMAT_UNKNOWN,
	//	DXGI_FORMAT_UNKNOWN,
	//	DXGI_FORMAT_UNKNOWN,
	//	DXGI_FORMAT_UNKNOWN,
	//};	//�����_�����O����J���[�o�b�t�@�̃t�H�[�}�b�g�B
};

class PostEffect :public Sprite
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	static const float clearColor[4];

private:
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;
	//SRv�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthbuff;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//�O���t�B�b�N�X�p�C�v���C��
	ComPtr<ID3D12PipelineState> pipelineState;
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;

private:
	//���擾�p
	DXGI_FORMAT texresDescFormat;

public:
	PostEffect();
	void Init(const SpriteInitData& spriteInitData);
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//�V�[���`��O����
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
	//�V�[���`��㏈��
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

private:
	//�p�C�v���C������(���g�ėp�����߂čׂ����֐�������������������)
	void CreateGraphicsPipelineState(const SpriteInitData& spriteInitData);

	//�e�N�X�`������
	//SRV�쐬
	//RTV�쐬
	//�[�x�o�b�t�@����
	//DSV�쐬

public:
	DXGI_FORMAT GetFormat()
	{
		return texresDescFormat;
	}
};