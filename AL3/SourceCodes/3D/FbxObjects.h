#pragma once
#include "FbxModels.h"
#include "Light.h"

//Struct2�K�v(FbxObject3D�Q��)

class FbxObjects
{
private:
	static const int MAX_BONES = 32;

	static ID3D12Device* device;
	static ID3D12GraphicsCommandList* cmdList;
	//static ObjPipelineSet pipelineSet;
	static Light* light;

private:
	struct ConstBufferDataB0
	{
		DirectX::XMMATRIX viewproj;		//�r���[�v���W�F�N�V�����s��
		DirectX::XMMATRIX world;		//���[���h�s��
		DirectX::XMFLOAT3 cameraPos;	//�J�������W�i���[���h���W�j
	};

	struct ConstBufferDataSkin
	{
		DirectX::XMMATRIX bones[MAX_BONES];
	};

public:
	static FbxObjects* Create(FbxModels* model = nullptr);

public:
	static void StaticInit(ID3D12Device* dev);
	static void SetDevice(ID3D12Device* device) { FbxObjects::device = device; }
	//static FbxPipelineSet CreateGraphicsPipeline(const FbxInitData& fbxInitdata);
	static void SetLight(Light* light) { FbxObjects::light = light; }

private:
	FbxModels* model = nullptr;
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	DirectX::XMFLOAT3 rotation = { 0,0,0 };
	DirectX::XMFLOAT3 position = { 0,0,0 };
	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();

	Microsoft::WRL::ComPtr<ID3D12Resource> constBufferDataB0;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffSkin;

	FbxTime frameTime;
	FbxTime startTime;
	FbxTime endTime;
	FbxTime currentTime;
	bool isPlay = false;

	ID3D12DescriptorHeap* fbxDescHeap = {};
	//�{�[���̖��O�ƍs��(Update��ɍX�V)
	std::vector<std::pair<std::string, DirectX::XMMATRIX>> affineTrans;
	//��̃��[���h�s��
	DirectX::XMMATRIX matrix = DirectX::XMMatrixIdentity();
	//�{�[���S���̉�]�s��
	std::vector<DirectX::XMMATRIX> matRots;

public:
	bool Init();
	void Update(bool isShadowCamera);
	void Draw();
	void PlayAnimation();

public:
	void SetModel(FbxModels* model) { this->model = model; }

private:
	void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src)
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				dst->r[i].m128_f32[j] = (float)src.Get(i, j);
			}
		}
	}
};
