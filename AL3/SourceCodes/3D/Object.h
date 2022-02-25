#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "Model.h"
#include "Light.h"
#include "../Math/CollisionInfo.h"

class BaseCollider;

class Object
{
public:
	//�p�C�v���C���Z�b�g
	struct PipelineSet
	{
		//���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> rootsignature;
		//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

	//�萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDataB0
	{
		//XMMATRIX mat;	// �R�c�ϊ��s��
		XMMATRIX viewproj; //�r���[�v���W�F�N�V�����s��
		XMMATRIX world; //���[���h�s��
		XMFLOAT3 cameraPos; //�J�������W�i���[���h���W�j
	};

private:
	//�f�o�C�X
	static ID3D12Device* device;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�p�C�v���C��
	static PipelineSet pipelineSet;
	//���C�g
	static Light* light;

protected:
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0;
	//�F
	XMFLOAT4 color = { 1,1,1,1 };
	//���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	//X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	//���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	//���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//�e�I�u�W�F�N�g
	Object* parent = nullptr;
	//���f��
	Model* model = nullptr;
	//�r���{�[�h
	bool isBillboard = false;

	//�N���X��
	const char* name = nullptr;
	//�R���C�_�[
	BaseCollider* collider = nullptr;

public:
	//�ÓI������
	static void StaticInit(ID3D12Device* device);
	//�O���t�B�b�N�p�C�v���C���̐���
	static void CreateGraphicsPipeline();
	//�`��O����
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	//�`��㏈��
	static void PostDraw();
	//3D�I�u�W�F�N�g����
	static Object* Create(Model* model = nullptr);
	//���C�g
	static void SetLight(Light* light) {
		Object::light = light;
	}

public:
	Object() = default;
	virtual ~Object();

	//������
	//bool Init();
	virtual bool Init();

	//���t���[������
	//void Update();
	virtual void Update();

	//�`��
	//void Draw();
	virtual void Draw();

public:
	//���W�̎擾
	const XMFLOAT3& GetPosition() { return position; }
	//��]�p�̎擾
	const XMFLOAT3& GetRotation() { return rotation; }
	//���W�̐ݒ�
	void SetPosition(XMFLOAT3 position) { this->position = position; }
	//��]�p�̐ݒ�
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	//�X�P�[���̐ݒ�
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }
	//���f���̃Z�b�g
	void SetModel(Model* model) { this->model = model; }
	//�r���{�[�h�t���O�̃Z�b�g
	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }
	//�F�ݒ�
	void SetColor(XMFLOAT4 color) { this->color = color; }

	//���[���h�s��擾
	const XMMATRIX& GetMatWorld() { return matWorld; }
	//�R���C�_�[�Z�b�g
	void SetCollider(BaseCollider* collider);
	//�Փˎ��R�[���o�b�N
	virtual void OnCollision(const CollisionInfo& info) {}
	//���f�����擾
	inline Model* GetModel() { return model; }
};

