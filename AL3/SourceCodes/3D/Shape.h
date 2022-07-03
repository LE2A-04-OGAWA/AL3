//#pragma once
//#include "../DirectX/DirectXImportant.h"
//#include "../DirectX/Camera.h"
//#include <forward_list>
//#include <vector>
//
//const int MAX_VERTEX_COUNT = 100;
//
//class Shape
//{
//private:
//	//Shape* shape = nullptr;
//	int count = 0;
//
//	struct Vertex {
//		DirectX::XMFLOAT3 pos; //xyz���W
//		DirectX::XMFLOAT3 normal = {};
//		DirectX::XMFLOAT2 uv = {};  //uv���W
//	};
//
//	std::vector<Vertex> CubeVertices;
//	std::vector<unsigned short> CubeIndices;
//
//	std::vector<Vertex> SphereVertices;
//	std::vector<unsigned short> SphereIndices;
//
//	//XMFLOAT4 Color = { 0.0f,0.0f,1.0f,1.0f };
//
//	//GS
//	struct GSVertex {
//		DirectX::XMFLOAT3 pos; //xyz���W
//		float scale;
//		DirectX::XMFLOAT4 color = {
//		1.0f,
//		1.0f,
//		1.0f,
//		1.0f
//		};
//	};
//
//	std::vector<GSVertex> PointVertices;
//
//	struct ConstBufferData {
//		//�F�iRGBA�j
//		DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
//		DirectX::XMMATRIX mat;   //3D�ϊ��s��
//	};
//
//	struct GSConstBufferData {
//		DirectX::XMMATRIX mat;   //3D�ϊ��s��
//		DirectX::XMMATRIX matBillBoard;
//	};
//
//	struct Particle
//	{
//		DirectX::XMFLOAT3 pos = {};
//		DirectX::XMFLOAT3 vel = {};
//		DirectX::XMFLOAT3 accel = {};
//		int frame = 0;
//		int num_frame = 0;
//
//		float scale = 1.0f;
//		float s_scale = 1.0f;
//		float e_scale = 0.0f;
//	};
//
//	std::forward_list<Particle> particles;
//
//	ID3D12Resource* constBuff = nullptr;
//	ConstBufferData* constMap = nullptr;
//	ID3D12Resource* vertBuff = nullptr;
//	ID3D12Resource* indexBuff = nullptr;
//
//	GSConstBufferData* GSconstMap = nullptr;
//
//	D3D12_VERTEX_BUFFER_VIEW vbView{};
//	D3D12_INDEX_BUFFER_VIEW ibView{};
//	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> basicDescHeap;
//	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;
//
//	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate;
//	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;
//
//public:
//	DirectX::XMMATRIX matWorld;        //�ėp�I���[���h�s��
//	DirectX::XMFLOAT3 scale;
//	DirectX::XMFLOAT3 rotation;        //XYZ������ɉ��x��]���邩
//	DirectX::XMFLOAT3 position;
//	DirectX::XMMATRIX matScale;
//	DirectX::XMMATRIX matRot;          //��]�s��
//	DirectX::XMMATRIX matTrans;
//
//public:
//	Shape();
//	//�����̂̐����ir=���a�j
//	void CreateCube(float r, const wchar_t* filename);
//	//���̐����ir=���a�j
//	void CreateSphere(float r);
//	//�����̂̕`��
//	void DrawCube();
//	//���̕`��
//	void DrawSphere();
//	//GS�̐���
//	void CreateGeometry(const wchar_t* filename);
//	//GS�̕`��
//	void DrawGeometry();
//
//public:
//	void Add(int life, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, DirectX::XMFLOAT3 accel, float start_scale, float end_scale);
//	Shape* Create();
//	/*void SetRandomColor() {
//		GSVertex* vertex = nullptr;
//		vertex->color = {
//			(float)(rand() / 101 / 10) ,
//			(float)(rand() / 101 / 10) ,
//			(float)(rand() / 101 / 10) ,
//			1.0f };
//	};*/
//};

