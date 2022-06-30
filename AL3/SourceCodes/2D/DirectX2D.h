#pragma once
#include "../DirectX/DirectXImportant.h"
#include <DirectXTex.h>
#include <wrl.h>

const int spriteSRVCount = 512;

class DirectX2D
{
private:
	//�X�v���C�g
	struct VertexPosUv {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		DirectX::XMFLOAT4 color; //�F�iRGBA
		DirectX::XMMATRIX mat;   //3D�ϊ��s��
	};

	Microsoft::WRL::ComPtr<ID3D12RootSignature> spriteRootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> spritePipelineState;
	Microsoft::WRL::ComPtr<ID3D12Resource> spriteVertBuff;
	Microsoft::WRL::ComPtr<ID3D12Resource> spriteConstBuff;
	D3D12_VERTEX_BUFFER_VIEW spriteVBView{};

	DirectX::XMFLOAT2 anchorpoint = { 0.5f,0.5f };
	bool isFlipX = false;
	bool isFlipY = false;

public:
	struct SpriteData
	{
		float spriteRotation;           //Z�����̉�]�p
		DirectX::XMVECTOR spritePosition{};      //���W
		DirectX::XMMATRIX spriteMatWorld{};      //���[���h�s��
		DirectX::XMMATRIX spriteMatProjection{}; //�ˉe�s��
		DirectX::XMFLOAT4 spriteColor;           //�F
		UINT texNumber;                 //�e�N�X�`���ԍ�
	} sprData;


private:
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{}; //�O���t�B�b�N�X�p�C�v���C���ݒ�\����
	DirectX::TexMetadata metadata{};
	Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};      //�ݒ�\����

	//ComPtr<ID3D12RootSignature> rootsignature;
	//ComPtr<ID3D12PipelineState> pipelinestate;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> basicDescHeap;
	D3D12_INPUT_ELEMENT_DESC inputLayout[2];
	//ComPtr<ID3DBlob> rootSigBlob;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> spriteDescHeap;
	Microsoft::WRL::ComPtr<ID3D12Resource> spriteTexBuff[spriteSRVCount];

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
	//�ꏊ�̐ݒ�
	void SetPos(DirectX::XMFLOAT2 pos) {
		sprData.spritePosition = { pos.x,pos.y,0 };
	}
};

