#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Mesh.h"

class Model
{
private:
	static const std::string baseDirectory;

private:
	//�f�o�C�X
	static ID3D12Device* device;
	//�f�X�N���v�^�̃T�C�Y
	static UINT descriptorHandleIncrementSize;

private:
	//���O
	std::string name;
	//���b�V���R���e�i
	std::vector<Mesh*> meshes;
	//�}�e���A���R���e�i
	std::unordered_map<std::string, Material*> materials;
	//�f�t�H���g�}�e���A��
	Material* defaultMaterial = nullptr;
	//�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;

public:
	//�ÓI������
	static void StaticInit(ID3D12Device* device);
	//���b�V������
	static Model* CreateFromObj(const std::string& modelname, bool smoothing = false);

public:
	Model();
	~Model();

	//������
	void Init(const std::string& modelname, bool smoothing);
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

private:
	//�}�e���A���Ǎ�
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);
	//�}�e���A���o�^
	void AddMaterial(Material* material);
	//�f�X�N���v�^�q�[�v����
	void CreateDescriptorHeap();
	//�e�N�X�`���Ǎ�
	void LoadTextures();
};

