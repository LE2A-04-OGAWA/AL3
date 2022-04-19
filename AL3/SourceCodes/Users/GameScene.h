#pragma once
#include "../2D/Sprite.h"
#include "../3D/Object.h"
#include "../3D/Model.h"
#include "../3D/Shape.h"
#include "../XAudio2/Music.h"
#include "../3D/FbxObject3D.h"

class GameScene
{
private:
	const float g = 9.8f;

private:
	XMFLOAT2 p;	//�ʒu
	XMFLOAT2 v;	//���x
	XMFLOAT2 a;	//�����x
	XMFLOAT2 f;	//������
	float m;	//����
	float t;	//����

private:
	Light* light = nullptr;

	Model* modelA = nullptr;
	Model* modelB = nullptr;

	Object* objA = nullptr;
	Object* objB = nullptr;

	FbxModel* fbxModel1 = nullptr;
	FbxObject3D* fbxObj1 = nullptr;

	Sprite* GH1;

public:
	GameScene();
	~GameScene();
	void Init();
	void Update();
	void Draw();
};