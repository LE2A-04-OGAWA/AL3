#include "GameScene.h"
#include "../3D/FbxLoader.h"
#include "../3D/FbxObject3D.h"
#include "../../imgui/ImguiControl.h"
#include "../Math/OgaJHelper.h"

GameScene::GameScene()
{
	const float objA_Scale = 40.0f;
	const float objB_Scale = 20.0f;
	const float objC_Scale = 20.0f;
	const float fbx1_Scale = 10.0f;

	XMFLOAT3 camera = Camera::GetEye();
	OgaJHelper::ConvertToRadian(camera.y);
	cameraAngle = { 0,camera.y,0 };

	modelA = Model::CreateFromObj("Lich");
	modelB = Model::CreateFromObj("ground");
	//modelB = Model::CreateFromObj("sponza");
	modelC = Model::CreateFromObj("monkey");

	objA = Object::Create(modelA);
	objB = Object::Create(modelB);
	objC = Object::Create(modelC);

	objA->SetScale(XMFLOAT3(objA_Scale, objA_Scale, objA_Scale));
	objB->SetScale(XMFLOAT3(objB_Scale, objB_Scale, objB_Scale));
	objC->SetScale(XMFLOAT3(objC_Scale, objC_Scale, objC_Scale));

	light = Light::Create();
	light->SetLightColor(
		{
			ImguiControl::Imgui_lightColor_r,
			ImguiControl::Imgui_lightColor_g,
			ImguiControl::Imgui_lightColor_b
		});
	light->SetLightDir(
		{
			ImguiControl::Imgui_lightDir_x,
			ImguiControl::Imgui_lightDir_y,
			ImguiControl::Imgui_lightDir_z,
		});

	Object::SetLight(light);

	FbxObject3D::SetDevice(DirectXImportant::dev.Get());
	FbxObject3D::CreateGraphicsPipeline();

	fbxModel1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	fbxObj1 = new FbxObject3D();
	fbxObj1->Init();
	fbxObj1->SetModel(fbxModel1);
	fbxObj1->SetScale(XMFLOAT3(fbx1_Scale, fbx1_Scale, fbx1_Scale));
	fbxObj1->SetRotation(XMFLOAT3(0, 0, 0));
	fbxObj1->PlayAnimation();

	Sprite::LoadTexture(0, L"Resources/hamurabyss.png");
	GH1 = Sprite::Create(0, XMFLOAT2(0, 0));
}

GameScene::~GameScene()
{
	//書き忘れるからヤメロ！
	delete modelA;
	delete objA;
	delete modelB;
	delete objB;
	delete modelC;
	delete objC;
	delete fbxModel1;
	delete fbxObj1;
}

void GameScene::Init()
{
	//Lich
	objA->SetPosition(XMFLOAT3(0.0f, 65.0f, -400.0f));
	objA->SetRotation(XMFLOAT3(0, -100, 0));

	//skydome or sponza
	objB->SetPosition(XMFLOAT3(0.0f, -10.0f, 0.0f));
	objB->SetRotation(XMFLOAT3(0, 90, 0));

	//monkey
	objC->SetPosition(XMFLOAT3(0.0f, 20.0f, 0.0f));
	objC->SetRotation(XMFLOAT3(0, 180, 0));

	//Camera::SetEye({ 20,20.0f,-50.0f });
	Camera::SetTarget({ 0.0f,15.0f,0.0f });
	//Camera::SetUp({ 0.0f,1.0f,0.0f });
}

void GameScene::Update()
{
	objB->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));

	light->SetLightColor(
		{
			ImguiControl::Imgui_lightColor_r,
			ImguiControl::Imgui_lightColor_g,
			ImguiControl::Imgui_lightColor_b
		});
	light->SetLightDir(
		{
			ImguiControl::Imgui_lightDir_x,
			ImguiControl::Imgui_lightDir_y,
			ImguiControl::Imgui_lightDir_z,
		});

	XMFLOAT3 objCpos = objC->GetPosition();
	XMFLOAT3 cameraPos = { objCpos.x,0,objCpos.z };

	if (!isTarget)
	{
		if (Input::isKeyTrigger(DIK_T)) { isTarget = true; }

		//カメラ系
		if (Input::isKey(DIK_RIGHT)) { cameraAngle.x += MAX_CAMERA_MOVE_SPEED; }
		if (Input::isKey(DIK_LEFT)) { cameraAngle.x -= MAX_CAMERA_MOVE_SPEED; }
		if (Input::isKey(DIK_UP)) {
			if (cameraAngle.y < XM_PI / 2) { cameraAngle.y += MAX_CAMERA_MOVE_SPEED; }
		}
		if (Input::isKey(DIK_DOWN)) {
			if (cameraAngle.y > -XM_PI / 5) { cameraAngle.y -= MAX_CAMERA_MOVE_SPEED; }
		}

		//距離と差分
		float distance = MAX_DISTANCE;
		float diff = 0;

		cameraPos.y += sinf(cameraAngle.y) * MAX_DISTANCE;
		//近寄るかの判定
		if (cameraPos.y < 0) {
			diff = cameraPos.y; cameraPos.y = 0;
		}

		cameraPos.x += sinf(cameraAngle.x) * (distance + diff);
		cameraPos.z += cosf(cameraAngle.x) * (distance + diff);

		//移動系
		XMFLOAT3 cameraToPlayer = OgaJHelper::CalcDirectionVec3(cameraPos, objCpos);
		cameraToPlayer = OgaJHelper::CalcNormalizeVec3(cameraToPlayer);

		//向き
		if (Input::isKey(DIK_W) || Input::isKey(DIK_S) || Input::isKey(DIK_D) || Input::isKey(DIK_A))
		{
			XMFLOAT2 vec = { 0,0 };

			if (Input::isKey(DIK_W))
			{
				objCpos.x += cameraToPlayer.x * MAX_MOVE_SPEED;
				objCpos.z += cameraToPlayer.z * MAX_MOVE_SPEED;

				vec.x += 0;
				vec.y += 1;
			}

			if (Input::isKey(DIK_S))
			{
				objCpos.x += cameraToPlayer.x * MAX_MOVE_SPEED * -1;
				objCpos.z += cameraToPlayer.z * MAX_MOVE_SPEED * -1;

				vec.x += 0;
				vec.y += -1;
			}

			if (Input::isKey(DIK_D))
			{
				float cameraRad = atan2(cameraToPlayer.x, cameraToPlayer.z);
				cameraRad += XM_PI / 2;
				objCpos.x += sinf(cameraRad) * MAX_MOVE_SPEED;
				objCpos.z += cosf(cameraRad) * MAX_MOVE_SPEED;
				OgaJHelper::ConvertToDegree(cameraRad);

				vec.x += -1;
				vec.y += 0;
			}

			if (Input::isKey(DIK_A)) {
				float cameraRad = atan2(cameraToPlayer.x, cameraToPlayer.z);
				cameraRad -= XM_PI / 2;
				objCpos.x += sinf(cameraRad) * MAX_MOVE_SPEED;
				objCpos.z += cosf(cameraRad) * MAX_MOVE_SPEED;
				OgaJHelper::ConvertToDegree(cameraRad);

				vec.x += 1;
				vec.y += 0;
			}

			float deg = atan2(vec.y, vec.x);
			OgaJHelper::ConvertToDegree(deg);
			float cameraRad = atan2(cameraToPlayer.x, cameraToPlayer.z);
			OgaJHelper::ConvertToDegree(cameraRad);

			objC->SetRotation(XMFLOAT3(0, deg + cameraRad - 90.0f, 0));
		}

		Camera::SetEye(cameraPos);
		Camera::SetTarget(objC->GetPosition());
	}

	else
	{
		XMFLOAT3 enemyToPlayer = OgaJHelper::CalcDirectionVec3(objA->GetPosition(), objC->GetPosition());
		enemyToPlayer = OgaJHelper::CalcNormalizeVec3(enemyToPlayer);

		Camera::SetEye(XMFLOAT3(
			objC->GetPosition().x + enemyToPlayer.x * MAX_DISTANCE,
			sinf(cameraAngle.y) * MAX_DISTANCE,
			objC->GetPosition().z + enemyToPlayer.z * MAX_DISTANCE));

		//移動系
		XMFLOAT3 cameraToPlayer = OgaJHelper::CalcDirectionVec3(Camera::GetEye(), objCpos);
		cameraToPlayer = OgaJHelper::CalcNormalizeVec3(cameraToPlayer);

		if (Input::isKeyTrigger(DIK_T))
		{
			isTarget = false;
		}

		//向き
		if (Input::isKey(DIK_W) || Input::isKey(DIK_S) || Input::isKey(DIK_D) || Input::isKey(DIK_A))
		{
			XMFLOAT2 vec = { 0,0 };

			if (Input::isKey(DIK_W))
			{
				objCpos.x += cameraToPlayer.x * MAX_MOVE_SPEED;
				objCpos.z += cameraToPlayer.z * MAX_MOVE_SPEED;

				vec.x += 0;
				vec.y += 1;
			}

			if (Input::isKey(DIK_S))
			{
				objCpos.x += cameraToPlayer.x * MAX_MOVE_SPEED * -1;
				objCpos.z += cameraToPlayer.z * MAX_MOVE_SPEED * -1;

				vec.x += 0;
				vec.y += -1;
			}

			if (Input::isKey(DIK_D))
			{
				float cameraRad = atan2(cameraToPlayer.x, cameraToPlayer.z);
				cameraRad += XM_PI / 2;
				objCpos.x += sinf(cameraRad) * MAX_MOVE_SPEED;
				objCpos.z += cosf(cameraRad) * MAX_MOVE_SPEED;
				OgaJHelper::ConvertToDegree(cameraRad);

				vec.x += -1;
				vec.y += 0;
			}

			if (Input::isKey(DIK_A)) {
				float cameraRad = atan2(cameraToPlayer.x, cameraToPlayer.z);
				cameraRad -= XM_PI / 2;
				objCpos.x += sinf(cameraRad) * MAX_MOVE_SPEED;
				objCpos.z += cosf(cameraRad) * MAX_MOVE_SPEED;
				OgaJHelper::ConvertToDegree(cameraRad);

				vec.x += 1;
				vec.y += 0;
			}

			float deg = atan2(vec.y, vec.x);
			OgaJHelper::ConvertToDegree(deg);
			float cameraRad = atan2(cameraToPlayer.x, cameraToPlayer.z);
			OgaJHelper::ConvertToDegree(cameraRad);

			objC->SetRotation(XMFLOAT3(0, deg + cameraRad - 90.0f, 0));
		}

		Camera::SetTarget(objA->GetPosition());
	}


	/*----------Update,Setter----------*/
	objB->SetPosition(XMFLOAT3(0.0f, ImguiControl::Imgui_ground_y, 0.0f));
	objC->SetPosition(objCpos);

	Object::SetLight(light);
	FbxObject3D::SetLight(light);

	light->Update();

	objA->Update();
	objB->Update();
	objC->Update();

	fbxObj1->Update();
	/*----------Update,Setter----------*/
}

void GameScene::Draw()
{
	Object::PreDraw(DirectXImportant::cmdList.Get());
	objA->Draw();
	objB->Draw();
	objC->Draw();
	Object::PostDraw();

	//fbxObj1->Draw(DirectXImportant::cmdList.Get());

	Sprite::PreDraw(DirectXImportant::cmdList.Get());
	//GH1->Draw();
	Sprite::PostDraw();
}
