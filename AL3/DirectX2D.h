#pragma once
#include "DirectXImportant.h"

const int spriteSRVCount = 512;

class DirectX2D
{
private:
	//�X�v���C�g
	struct VertexPosUv {
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		XMFLOAT4 color; //�F�iRGBA
		XMMATRIX mat;   //3D�ϊ��s��
	};

	ComPtr<ID3D12RootSignature> spriteRootSignature;
	ComPtr<ID3D12PipelineState> spritePipelineState;
	ComPtr<ID3D12Resource> spriteVertBuff;
	ComPtr<ID3D12Resource> spriteConstBuff;
	D3D12_VERTEX_BUFFER_VIEW spriteVBView{};

	XMFLOAT2 anchorpoint = { 0.5f,0.5f };
	bool isFlipX = false;
	bool isFlipY = false;

public:
	struct SpriteData
	{
		float spriteRotation;           //Z�����̉�]�p
		XMVECTOR spritePosition{};      //���W
		XMMATRIX spriteMatWorld{};      //���[���h�s��
		XMMATRIX spriteMatProjection{}; //�ˉe�s��
		XMFLOAT4 spriteColor;           //�F
		UINT texNumber;                 //�e�N�X�`���ԍ�
	} sprData;


private:
	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> psBlob;
	ComPtr<ID3DBlob> errorBlob;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //�O���t�B�b�N�X�p�C�v���C���ݒ�\����
	TexMetadata metadata{};
	ComPtr<ID3D12Resource> texbuff;
	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};      //�ݒ�\����

	//ComPtr<ID3D12RootSignature> rootsignature;
	//ComPtr<ID3D12PipelineState> pipelinestate;
	ComPtr<ID3D12DescriptorHeap> basicDescHeap;
	D3D12_INPUT_ELEMENT_DESC inputLayout[2];
	//ComPtr<ID3DBlob> rootSigBlob;

	ComPtr<ID3D12DescriptorHeap> spriteDescHeap;
	ComPtr<ID3D12Resource> spriteTexBuff[spriteSRVCount];

private:
	void VSShaderCompile();
	void PSShaderCompile();
	void InputLayout();
	void SettingPipelineState();
	void RootSignature();
	void CreatePipelineState();

public:
	DirectX2D();
	//�摜�ǂݍ���
	HRESULT SpriteLoadTexture(UINT texnumber, const wchar_t* filename);
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g�����i�������j
	void PipelineStateObjInit();
	//�X�v���C�g�����i�������j
	HRESULT CreateSpriteInit();
	//�p�C�v���C���ݒ�R�}���h�i���t���[�������j
	void SpriteUpdate();
	//�`��R�}���h�i���t���[�������j
	void SpriteDraw(bool isDraw = true);
};

