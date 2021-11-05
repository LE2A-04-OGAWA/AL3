#pragma once
#include "DirectXImportant.h"
#include "Camera.h"

class Shape
{
private:
	struct Vertex {
		XMFLOAT3 pos; //xyz���W
		XMFLOAT3 normal = {};
		XMFLOAT2 uv = {};  //uv���W
	};

	std::vector<Vertex> CubeVertices;
	std::vector<unsigned short> CubeIndices;

	std::vector<Vertex> SphereVertices;
	std::vector<unsigned short> SphereIndices;

	//GS
	struct GSVertex {
		XMFLOAT3 pos; //xyz���W
	};

	std::vector<GSVertex> PointVertices;

	struct ConstBufferData {
		//�F�iRGBA�j
		XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
		XMMATRIX mat;   //3D�ϊ��s��
	};

	struct GSConstBufferData {
		XMMATRIX mat;   //3D�ϊ��s��
	};

	ID3D12Resource* constBuff = nullptr;
	ConstBufferData* constMap = nullptr;
	ID3D12Resource* vertBuff = nullptr;
	ID3D12Resource* indexBuff = nullptr;

	GSConstBufferData* GSconstMap = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ComPtr<ID3D12DescriptorHeap> basicDescHeap;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;

	ComPtr<ID3D12PipelineState> pipelinestate;
	ComPtr<ID3D12RootSignature> rootsignature;

public:
	XMMATRIX matWorld;        //�ėp�I���[���h�s��
	XMFLOAT3 scale;
	XMFLOAT3 rotation;        //XYZ������ɉ��x��]���邩
	XMFLOAT3 position;
	XMMATRIX matScale;
	XMMATRIX matRot;          //��]�s��
	XMMATRIX matTrans;

public:
	Shape();
	//�����̂̐����ir=���a�j
	void CreateCube(float r, const wchar_t* filename);
	//���̐����ir=���a�j
	void CreateSphere(float r);
	//�����̂̕`��
	void DrawCube();
	//���̕`��
	void DrawSphere();
	//GS�̐���
	void CreateGeometry(const wchar_t* filename);
	//GS�̕`��
	void DrawGeometry();
};

