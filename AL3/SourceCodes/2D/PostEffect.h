#pragma once
#include "Sprite.h"

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

public:
	PostEffect();
	void Init();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//�V�[���`��O����
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
	//�V�[���`��㏈��
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);
	//�p�C�v���C������(���g�ėp�����߂čׂ����֐�������������������)
	void CreateGraphicsPipelineState();

	//�e�N�X�`������
	//SRV�쐬
	//RTV�쐬
	//�[�x�o�b�t�@����
	//DSV�쐬
};