#pragma once
#include "Easing.h"
#include <vector>

//using namespace DirectX;

//static const XMFLOAT2 operator+ (XMFLOAT2 lhs, XMFLOAT2 rhs) {
//	return XMFLOAT2{ lhs.x + rhs.x,lhs.y + rhs.x };
//}

namespace OgaJHelper {

using XMFLOAT3 = DirectX::XMFLOAT3;
using XMFLOAT2 = DirectX::XMFLOAT2;

//方向ベクトルを算出
XMFLOAT3 CalcDirectionVec3(XMFLOAT3 start, XMFLOAT3 end) {
	XMFLOAT3 vec = { 0,0,0 };
	vec.x = end.x - start.x;
	vec.y = end.y - start.y;
	vec.z = end.z - start.z;
	return vec;
}

//XMFLOAT3を正規化
XMFLOAT3 CalcNormalizeVec3(XMFLOAT3 vec3) {
	DirectX::XMStoreFloat3(&vec3, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&vec3)));
	return vec3;
}

//XMFLOAT3の2点間の距離を算出
float CalcDist(XMFLOAT3 pos1, XMFLOAT3 pos2) {
	float diff;
	diff = sqrtf(
		(pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.y - pos2.y) * (pos1.y - pos2.y) +
		(pos1.z - pos2.z) * (pos1.z - pos2.z));
	return diff;
}

//角度をラジアンにする
void ConvertToRadian(float& degree) {
	degree = degree / 180.0f * DirectX::XM_PI;
}

//ラジアンを角度にする
void ConvertToDegree(float& radian) {
	radian = radian * 180.0f / DirectX::XM_PI;
}

//2つのベクトルのなす角(ラジアン)を算出
float CalcTwoVec2toRadian(XMFLOAT2 first, XMFLOAT2 second) {
	float dot = first.x * second.x + first.y * second.y;
	float absA = sqrtf(first.x * first.x + first.y * first.y);
	float absB = sqrtf(second.x * second.x + second.y * second.y);
	float cosTheta = dot / (absA * absB);
	return acosf(-cosTheta);
}

//Vectorの中身にnumberがあるか判定
bool VectorFinder(std::vector<int> vec, int number) {
	auto itr = std::find(vec.begin(), vec.end(), number);
	size_t index = std::distance(vec.begin(), itr);
	if (index != vec.size()) { return true; }
	else { return false; }
}

//近い方を角度を算出
float RotateEarliestArc(float NowAngle, float EndAngle)
{
	if (fabsf(EndAngle - NowAngle) > 180.0f) {
		if (NowAngle < 180.0f) {
			NowAngle += 360.0f;
		}
		else {
			NowAngle -= 360.0f;
		}
	}
	return EndAngle - NowAngle;
}

XMFLOAT3 Cross(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	XMFLOAT3 vec;
	vec.x = v1.y * v2.z - v1.z * v2.y;
	vec.y = v1.z * v2.x - v1.x * v2.z;
	vec.z = v1.x * v2.y - v1.y * v2.x;
	return vec;
}
}
