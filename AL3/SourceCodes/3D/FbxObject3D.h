#pragma once
#include "FbxModel.h"
#include "../3D/Light.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class FbxObject3D
{
protected:
	//WRL�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//�{�[���̍ő吔
	static const int MAX_BONES = 32;

public:
	//�萔�o�b�t�@
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;	//�r���[�v���W�F�N�V�����s��
		XMMATRIX world;		//���[���h�s��
		XMFLOAT3 cameraPos;	//�J�������W(���[���h���W)
	};

	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

	struct ConstBufferData1
	{
		XMFLOAT3 ambient; // �A���r�G���g�W��
		float pad1; // �p�f�B���O
		XMFLOAT3 diffuse; // �f�B�t���[�Y�W��
		float pad2; // �p�f�B���O
		XMFLOAT3 specular; // �X�y�L�����[�W��
		float alpha;	// �A���t�@
	};

private:
	//�f�o�C�X
	static ID3D12Device* device;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	//���C�g
	static Light* light;

public:
	//Setter
	static void SetDevice(ID3D12Device* device) { FbxObject3D::device = device; }
	//�O���t�B�b�N�X�p�C�v���C���̐���
	static void CreateGraphicsPipeline();
	//���C�g�̃Z�b�g
	static void SetLight(Light* light) { FbxObject3D::light = light; }

private:
	//�A���r�G���g�e���x
	XMFLOAT3 ambient = { 0.8f, 0.8f, 0.8f };
	//�f�B�t���[�Y�e���x
	XMFLOAT3 diffuse = { 0.8f, 0.8f, 0.8f };
	//�X�y�L�����[�e���x
	XMFLOAT3 specular = { 0.8f, 0.8f, 0.8f };
	//�A���t�@
	float alpha = 1.0f;

protected:
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;
	//�萔�o�b�t�@�i���ˌ��j
	ComPtr<ID3D12Resource> constBuffData1;
	//���[�J���X�P�[��
	XMFLOAT3 scale = { 10,10,10 };
	//X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	//���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	//���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//���f��
	FbxModel* model = nullptr;
	//�萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;
	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;

	//�{�[���̖��O�ƍs��(Update��ɍX�V)
	std::vector<std::pair<std::string, DirectX::XMMATRIX>> affineTrans;
	//Test
	DirectX::XMMATRIX matrix;

public:
	//������
	void Init();
	//���t���[������
	void Update();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//�A�j���[�V�����J�n
	void PlayAnimation();

public:
	void SetModel(FbxModel* model) { this->model = model; }
	//Setter�����I
	void SetScale(XMFLOAT3& scale) { this->scale = scale; }
	void SetRotation(XMFLOAT3& rotation) { this->rotation = rotation; }
	void SetPosition(XMFLOAT3& position) { this->position = position; }
	const XMFLOAT3& GetScale() { return scale; }
	const XMFLOAT3& GetRotation() { return rotation; }
	const XMFLOAT3& GetPosition() { return position; }

	void StopAnimation() { isPlay = false; }
	void ResetAnimation() { currentTime = startTime; }
	void ReplayAnimation() { isPlay = true; }

	const FbxTime& GetStartTime() { return startTime; }
	const FbxTime& GetEndTime() { return endTime; }
	const FbxTime& GetNowTime() { return currentTime; }
	void SetStartTime(FbxTime& startTime) { this->startTime = startTime; }
	void SetEndTime(FbxTime& endTime) { this->endTime = endTime; }
	void SetNowTime(FbxTime& currentTime) { this->currentTime = currentTime; }

	const std::vector<std::pair<std::string, DirectX::XMMATRIX>>& GetAffineTrans() { return affineTrans; }
	XMMATRIX GetMatrix() { return matrix; }
};

