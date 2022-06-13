#pragma once
#include <wrl.h>
#include <DirectXMath.h>
#include <d3d12.h>
#include <d3dx12.h>

using namespace DirectX;
using namespace Microsoft::WRL;

class Light
{
public:
	struct ConstBufferData {
		XMVECTOR lightv; //���C�g�ւ̕�����\���x�N�g��
		XMFLOAT3 lightcolor; //���C�g�̐F
		XMMATRIX lightViewproj;
	};

private:
	//�f�o�C�X
	static ID3D12Device* device;

private:
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//���C�g���������i�P�ʃx�N�g���j
	XMVECTOR lightdir = { 1,0,0,0 };
	//���C�g�F
	XMFLOAT3 lightcolor = { 1,1,1 };
	//�_�[�e�B�t���O
	bool dirty = false;

public:
	//�ÓI������
	static void StaticInit(ID3D12Device* device);
	//�C���X�^���X����
	static Light* Create();

public:
	//���C�g
	void Init();
	//�萔�o�b�t�@�]��
	void TransferConstBuffer();
	//���C�g�������Z�b�g
	void SetLightDir(const XMVECTOR& lightdir);
	//���C�g�F���Z�b�g
	void SetLightColor(const XMFLOAT3& lightcolor);
	//�X�V
	void Update();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);
};

