#pragma once
#include "../DirectX/DirectXImportant.h"
#include "../DirectX/Camera.h"
#include <forward_list>

const int MAX_VERTEX_COUNT = 100;

class Shape
{
private:
	//Shape* shape = nullptr;
	int count = 0;

	struct Vertex {
		XMFLOAT3 pos; //xyz座標
		XMFLOAT3 normal = {};
		XMFLOAT2 uv = {};  //uv座標
	};

	std::vector<Vertex> CubeVertices;
	std::vector<unsigned short> CubeIndices;

	std::vector<Vertex> SphereVertices;
	std::vector<unsigned short> SphereIndices;

	//XMFLOAT4 Color = { 0.0f,0.0f,1.0f,1.0f };

	//GS
	struct GSVertex {
		XMFLOAT3 pos; //xyz座標
		float scale;
		XMFLOAT4 color = {
		1.0f,
		1.0f,
		1.0f,
		1.0f
		};
	};

	std::vector<GSVertex> PointVertices;

	struct ConstBufferData {
		//色（RGBA）
		XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
		XMMATRIX mat;   //3D変換行列
	};

	struct GSConstBufferData {
		XMMATRIX mat;   //3D変換行列
		XMMATRIX matBillBoard;
	};

	struct Particle
	{
		XMFLOAT3 pos = {};
		XMFLOAT3 vel = {};
		XMFLOAT3 accel = {};
		int frame = 0;
		int num_frame = 0;

		float scale = 1.0f;
		float s_scale = 1.0f;
		float e_scale = 0.0f;
	};

	std::forward_list<Particle> particles;

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
	XMMATRIX matWorld;        //汎用的ワールド行列
	XMFLOAT3 scale;
	XMFLOAT3 rotation;        //XYZ軸を基準に何度回転するか
	XMFLOAT3 position;
	XMMATRIX matScale;
	XMMATRIX matRot;          //回転行列
	XMMATRIX matTrans;

public:
	Shape();
	//立方体の生成（r=半径）
	void CreateCube(float r, const wchar_t* filename);
	//球の生成（r=半径）
	void CreateSphere(float r);
	//立方体の描画
	void DrawCube();
	//球の描画
	void DrawSphere();
	//GSの生成
	void CreateGeometry(const wchar_t* filename);
	//GSの描画
	void DrawGeometry();

public:
	void Add(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale);
	Shape* Create();
	/*void SetRandomColor() {
		GSVertex* vertex = nullptr;
		vertex->color = {
			(float)(rand() / 101 / 10) ,
			(float)(rand() / 101 / 10) ,
			(float)(rand() / 101 / 10) ,
			1.0f };
	};*/
};

