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

struct ObjectInitData
{
	const char* m_vsEntryPoint = "VSmain";
	const char* m_psEntryPoint = "PSmain";
};

//パイプラインセット
struct ObjPipelineSet
{
	//ルートシグネチャ
	ID3D12RootSignature* rootsignature;
	//パイプラインステートオブジェクト
	ID3D12PipelineState* pipelinestate;
};

class BaseCollider;

class Object
{
public:
	//定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		DirectX::XMMATRIX viewproj;		//ビュープロジェクション行列
		DirectX::XMMATRIX world;		//ワールド行列
		DirectX::XMFLOAT3 cameraPos;	//カメラ座標（ワールド座標）
	};

private:
	//デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//パイプライン
	static ObjPipelineSet pipelineSet;
	//ライト
	static Light* light;

protected:
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0;
	//色
	DirectX::XMFLOAT4 color = { 1,1,1,1 };
	//ローカルスケール
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	//X,Y,Z軸回りのローカル回転角
	DirectX::XMFLOAT3 rotation = { 0,0,0 };
	//ローカル座標
	DirectX::XMFLOAT3 position = { 0,0,0 };
	//ローカルワールド変換行列
	DirectX::XMMATRIX matWorld;
	//親オブジェクト
	Object* parent = nullptr;
	//モデル
	Model* model = nullptr;
	//ビルボード
	bool isBillboard = false;

	//クラス名
	const char* name = nullptr;
	//コライダー
	BaseCollider* collider = nullptr;

private:
	//SRV用デスクリプタヒープ
	ID3D12DescriptorHeap* modelDescHeap;

	//親子構造用
	DirectX::XMMATRIX matrix;
	bool isAffine = false;

	//texbuffを追加するか
	bool isAddTexture = false;

	//回転行列
	DirectX::XMMATRIX matRot;

public:
	//静的初期化
	static void StaticInit(ID3D12Device* device);
	//グラフィックパイプラインの生成
	static ObjPipelineSet CreateGraphicsPipeline(const ObjectInitData& objectInitData);
	//描画前処理
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	//描画後処理
	static void PostDraw();
	//3Dオブジェクト生成
	static Object* Create(Model* model = nullptr);
	//ライト
	static void SetLight(Light* light) {
		Object::light = light;
	}

public:
	Object() = default;
	virtual ~Object();

	//初期化
	//bool Init();
	virtual bool Init();

	//毎フレーム処理
	//void Update();
	virtual void Update(bool isShadowCamera = false);

	//描画
	//void Draw();
	virtual void Draw(const ObjPipelineSet& pipelineSet);

public:
	//座標の取得
	const DirectX::XMFLOAT3& GetPosition() { return position; }
	//回転角の取得
	const DirectX::XMFLOAT3& GetRotation() { return rotation; }
	//スケールの取得
	const DirectX::XMFLOAT3& GetScale() { return scale; }
	//座標の設定
	void SetPosition(DirectX::XMFLOAT3 position) { this->position = position; }
	//回転角の設定
	void SetRotation(DirectX::XMFLOAT3 rotation) { this->rotation = rotation; }
	//スケールの設定
	void SetScale(DirectX::XMFLOAT3 scale) { this->scale = scale; }
	//モデルのセット
	void SetModel(Model* model) { this->model = model; }
	//ビルボードフラグのセット
	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }
	//色設定
	void SetColor(DirectX::XMFLOAT4 color) { this->color = color; }

	//ワールド行列取得
	const DirectX::XMMATRIX& GetMatWorld() { return matWorld; }
	//コライダーセット
	void SetCollider(BaseCollider* collider);
	//衝突時コールバック
	virtual void OnCollision(const CollisionInfo& info) {}
	//モデルを取得
	inline Model* GetModel() { return model; }
	//テクスチャ追加（マルチテクスチャ）
	void AddTexture(ID3D12Resource* texbuff, ID3D12DescriptorHeap* srv);
	//アフィン変換行列を乗算する(Update後)
	void MultiMatrix(DirectX::XMMATRIX matrix)
	{
		isAffine = true;
		this->matrix = matrix;
	}
	const DirectX::XMMATRIX GetMatRot() { return matRot; }	//親子関係は無視した回転行列です
};

