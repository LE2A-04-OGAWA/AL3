#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <map>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>

#pragma warning(push)
#pragma warning(disable:26451)
#pragma warning(disable:26495)
#pragma warning(disable:26812)
#include "fbxsdk.h"
#pragma warning(pop)

struct Node
{
	//名前
	std::string name;
	//ローカルスケール
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	//ローカル回転角
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//ローカル移動
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	//ローカル変形行列
	DirectX::XMMATRIX transform;
	//グローバル変形行列
	DirectX::XMMATRIX globalTransform;
	//親ノード
	Node* parent = nullptr;
};

class FbxModel
{
public:
	//フレンドクラス
	friend class FbxLoader;

private:
	//WRL省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::を省略
	using string = std::string;
	template <class T> using vector = std::vector<T>;

public:
	//ボーンインデックスの最大数
	static const int MAX_BONE_INDICES = 4;

public:
	//頂点データ構造体
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWeight[MAX_BONE_INDICES];
	};

	//ボーン構造体
	struct Bone
	{
		//名前
		std::string name;
		//初期姿勢の逆行列
		DirectX::XMMATRIX invInitialPose;
		//クラスター(FBX側のボーン情報)
		FbxCluster* fbxCluster;
		//コンストラクタ
		Bone(const std::string& name) { this->name = name; }
	};

	//Test(Sampleを参考に)
	struct MeshData
	{
		ComPtr<ID3D12Resource> m_VertBuff;
		ComPtr<ID3D12Resource> m_IndexBuff;
		std::vector<DirectX::XMFLOAT3> m_Pos;
		std::vector<DirectX::XMFLOAT3> m_Normal;
		std::vector<DirectX::XMFLOAT2> m_Uv;
		std::vector<DirectX::XMFLOAT4> m_Color;
		std::vector<UINT> m_Indices;
		std::string m_MaterialName;
	};
	struct Material
	{
		DirectX::XMFLOAT3 m_Ambient = { 1,1,1 };
		DirectX::XMFLOAT3 m_Diffuse = { 1,1,1 };
	};
	//Test
	std::vector<MeshData> m_MeshList;
	std::map<std::string, Material> m_Materials;
	std::map<std::string, std::pair<DirectX::TexMetadata, DirectX::ScratchImage>> m_DxTextures;

private:
	//モデル名
	std::string name;
	//ノード配列
	std::vector<Node> nodes;
	//メッシュを持つノード
	Node* meshNode = nullptr;
	//頂点データ配列
	std::vector<VertexPosNormalUvSkin> vertices;
	//頂点インデックス配列
	std::vector<unsigned short> indices;
	//アンビエント係数
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	//ディフューズ係数
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };
	//テクスチャメタデータ
	DirectX::TexMetadata metadata = {};
	//スクラッチイメージ
	DirectX::ScratchImage scratchImg = {};
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	//テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV用デスクリプタヒープ
	ID3D12DescriptorHeap* descHeapSRV;
	//ボーン配列
	std::vector<Bone> bones;
	//FBXシーン
	FbxScene* fbxScene = nullptr;

public:
	//デストラクタ
	~FbxModel();
	//バッファ生成
	void CreateBuffers(ID3D12Device* device);
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

public:
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

	//Getter
	std::vector<Bone>& GetBones() { return bones; }
	FbxScene* GetFbxScene() { return fbxScene; }

	ID3D12DescriptorHeap* GetDescHeap() { return descHeapSRV; }
};

