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

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffSkin;

	FbxTime frameTime;
	FbxTime startTime;
	FbxTime endTime;
	FbxTime currentTime;
	bool isPlay = false;
};
