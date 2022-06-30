#pragma once
#include <wrl.h>
#include <DirectXMath.h>
#include <d3d12.h>
#include <d3dx12.h>

//using namespace DirectX;
//using namespace Microsoft::WRL;

class Light
{
public:
	struct ConstBufferData {
		DirectX::XMVECTOR lightv; //ライトへの方向を表すベクトル
		DirectX::XMFLOAT3 lightcolor; //ライトの色
		DirectX::XMMATRIX lightViewproj;
	};

private:
	//デバイス
	static ID3D12Device* device;

private:
	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	//ライト光線方向（単位ベクトル）
	DirectX::XMVECTOR lightdir = { 1,0,0,0 };
	//ライト色
	DirectX::XMFLOAT3 lightcolor = { 1,1,1 };
	//ダーティフラグ
	bool dirty = false;

	//影用ライト座標
	DirectX::XMFLOAT3 eye = { 0,100,0 };
	DirectX::XMFLOAT3 target = { 0,0,0 };
	DirectX::XMFLOAT3 up = { 0,0,-1 };

public:
	//静的初期化
	static void StaticInit(ID3D12Device* device);
	//インスタンス生成
	static Light* Create();

public:
	//ライト
	void Init();
	//定数バッファ転送
	void TransferConstBuffer();
	//ライト方向をセット
	void SetLightDir(const DirectX::XMVECTOR& lightdir);
	//ライト色をセット
	void SetLightColor(const DirectX::XMFLOAT3& lightcolor);
	//更新
	void Update();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	//影用ライト座標をセット
	void SetShadowLigitEye(DirectX::XMFLOAT3 eye) { this->eye = eye;  dirty = true; }
	void SetShadowLigitTarget(DirectX::XMFLOAT3 target) { this->target = target; dirty = true; }
	void SetShadowLigitUp(DirectX::XMFLOAT3 up) { this->up = up; dirty = true; }

	DirectX::XMFLOAT3 GetShadowLigitEye() { return eye; }
	DirectX::XMFLOAT3 GetShadowLigitTarget() { return target; }
	DirectX::XMFLOAT3 GetShadowLigitUp() { return up; }
};

