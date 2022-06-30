#include "GameScene.h"
#include "../3D/FbxLoader.h"
#include "../3D/FbxObject3D.h"
#include "../../imgui/ImguiControl.h"
#include "../Math/OgaJHelper.h"
#include "../Input/FunaInput.h"
#include "../Input/Input.h"

GameScene::GameScene()
{
	animationType = STAND;

	ObjectInitData normalInit;
	normalInit.m_psEntryPoint = "PSmain";
	normalInit.m_vsEntryPoint = "VSmain";
	normal = Object::CreateGraphicsPipeline(normalInit);

	ObjectInitData shadowInit;
	shadowInit.m_psEntryPoint = "PSBlack";
	shadowInit.m_vsEntryPoint = "VSShadowMain";
	shadow = Object::CreateGraphicsPipeline(shadowInit);

	ObjectInitData receiveInit;
	receiveInit.m_psEntryPoint = "PSShadowMain";
	receiveInit.m_vsEntryPoint = "VSShadowMain";
	receiveShadow = Object::CreateGraphicsPipeline(receiveInit);

	XMFLOAT3 camera = Camera::GetEye();
	OgaJHelper::ConvertToRadian(camera.y);
	cameraAngle = { 0,camera.y,0 };

	modelA = Model::CreateFromObj("triangle");
	modelB = Model::CreateFromObj("yuka");
	//modelB = Model::CreateFromObj("sponza");
	modelC = Model::CreateFromObj("monkey");
	modelD = Model::CreateFromObj("triangle");
	sword = Model::CreateFromObj("Sword");

	objA = Object::Create(modelA);
	objB = Object::Create(modelB);
	objC = Object::Create(modelC);
	objD = Object::Create(modelD);
	objSword = Object::Create(sword);
	objShadowSword = Object::Create(sword);

	const float objA_Scale = 40.0f;
	const float objB_Scale = 100.0f;
	const float objC_Scale = 20.0f;
	const float objD_Scale = 5.0f;
	const float objSword_Scale = 1.0f;
	const float fbx_Scale = 0.02f;

	objA->SetScale(XMFLOAT3(objA_Scale, objA_Scale, objA_Scale));
	objB->SetScale(XMFLOAT3(objB_Scale, objB_Scale, objB_Scale));
	objC->SetScale(XMFLOAT3(objC_Scale, objC_Scale, objC_Scale));
	objD->SetScale(XMFLOAT3(objD_Scale, objD_Scale, objD_Scale));
	objSword->SetScale(XMFLOAT3(objSword_Scale, objSword_Scale, objSword_Scale));
	objShadowSword->SetScale(XMFLOAT3(objSword_Scale, objSword_Scale, objSword_Scale));

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

	FbxInitData fbxNormalInit;
	fbxNormalInit.m_vsEntryPoint = "PSmain";
	fbxNormalInit.m_vsEntryPoint = "VSmain";
	fbx_normal = FbxObject3D::CreateGraphicsPipeline(fbxNormalInit);

	FbxInitData fbxShadowInit;
	fbxShadowInit.m_psEntryPoint = "PSBlack";
	fbxShadowInit.m_vsEntryPoint = "VSShadowMain";
	fbx_shadow = FbxObject3D::CreateGraphicsPipeline(fbxShadowInit);

	fbxModel1 = FbxLoader::GetInstance()->LoadModelFromFile("StandMiku");
	fbxModel2 = FbxLoader::GetInstance()->LoadModelFromFile("SlowRunMiku");
	fbxModel3 = FbxLoader::GetInstance()->LoadModelFromFile("RunMiku");

	fbxObj1 = new FbxObject3D();
	fbxObj1->Init();
	fbxObj1->SetModel(fbxModel1);
	fbxObj1->SetPosition(XMFLOAT3(0, 0, 0));
	fbxObj1->SetScale(XMFLOAT3(fbx_Scale, fbx_Scale, fbx_Scale));
	fbxObj1->SetRotation(XMFLOAT3(0, 0, 0));
	fbxObj1->PlayAnimation();

	fbxObj2 = new FbxObject3D();
	fbxObj2->Init();
	fbxObj2->SetModel(fbxModel2);
	fbxObj2->SetPosition(XMFLOAT3(0, 0, 0));
	fbxObj2->SetScale(XMFLOAT3(fbx_Scale, fbx_Scale, fbx_Scale));
	fbxObj2->SetRotation(XMFLOAT3(0, 0, 0));
	fbxObj2->PlayAnimation();

	fbxObj3 = new FbxObject3D();
	fbxObj3->Init();
	fbxObj3->SetModel(fbxModel3);
	fbxObj3->SetPosition(XMFLOAT3(0, 0, 0));
	fbxObj3->SetScale(XMFLOAT3(fbx_Scale, fbx_Scale, fbx_Scale));
	fbxObj3->SetRotation(XMFLOAT3(0, 0, 0));
	fbxObj3->PlayAnimation();


	fbxShadow1 = new FbxObject3D();
	fbxShadow1->Init();
	fbxShadow1->SetModel(fbxModel1);
	fbxShadow1->SetPosition(XMFLOAT3(0, 0, 0));
	fbxShadow1->SetScale(XMFLOAT3(fbx_Scale, fbx_Scale, fbx_Scale));
	fbxShadow1->SetRotation(XMFLOAT3(0, 0, 0));
	fbxShadow1->PlayAnimation();

	fbxShadow2 = new FbxObject3D();
	fbxShadow2->Init();
	fbxShadow2->SetModel(fbxModel2);
	fbxShadow2->SetPosition(XMFLOAT3(0, 0, 0));
	fbxShadow2->SetScale(XMFLOAT3(fbx_Scale, fbx_Scale, fbx_Scale));
	fbxShadow2->SetRotation(XMFLOAT3(0, 0, 0));
	fbxShadow2->PlayAnimation();

	fbxShadow3 = new FbxObject3D();
	fbxShadow3->Init();
	fbxShadow3->SetModel(fbxModel3);
	fbxShadow3->SetPosition(XMFLOAT3(0, 0, 0));
	fbxShadow3->SetScale(XMFLOAT3(fbx_Scale, fbx_Scale, fbx_Scale));
	fbxShadow3->SetRotation(XMFLOAT3(0, 0, 0));
	fbxShadow3->PlayAnimation();

	Sprite::LoadTexture(0, L"Resources/hamurabyss.png");
	GH1 = Sprite::Create(0, XMFLOAT2(0, WINDOW_HEIGHT / 2));
	GH1->SetSize(XMFLOAT2(32, 32));
}

GameScene::~GameScene()
{
	//�����Y��邩�烄�����I
	delete modelA;
	delete objA;
	delete modelB;
	delete objB;
	delete modelC;
	delete objC;
	delete modelD;
	delete objD;
	delete fbxModel1;
	delete fbxObj1;
	delete fbxModel2;
	delete fbxObj2;
	delete fbxModel3;
	delete fbxObj3;
	delete GH1;

	delete fbxShadow1;
	delete fbxShadow2;
	delete fbxShadow3;
}

void GameScene::Init(ID3D12Resource* texbuff)
{
	//Lich
	objA->SetPosition(XMFLOAT3(0.0f, 65.0f, -400.0f));
	objA->SetRotation(XMFLOAT3(0, -100, 0));

	//skydome or sponza
	objB->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	objB->SetRotation(XMFLOAT3(0, 90, 0));

	//monkey
	objC->SetPosition(XMFLOAT3(0.0f, 20.0f, 0.0f));
	objC->SetRotation(XMFLOAT3(0, 180, 0));

	//Sword
	objSword->SetPosition(XMFLOAT3(0.0f, 20.0f, 0.0f));
	objSword->SetRotation(XMFLOAT3(90, 0, 180));

	//Miku
	fbxObj1->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	fbxObj2->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	fbxObj3->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));

	//eye
	objD->SetBillboard(true);

	Camera::SetTarget(XMFLOAT3(
		fbxObj1->GetPosition().x,
		fbxObj1->GetPosition().y,
		fbxObj1->GetPosition().z));

	XMFLOAT3 enemyToPlayer = OgaJHelper::CalcDirectionVec3(objA->GetPosition(), fbxObj1->GetPosition());
	enemyToPlayer = OgaJHelper::CalcNormalizeVec3(enemyToPlayer);

	Camera::SetEye(XMFLOAT3(
		fbxObj1->GetPosition().x + enemyToPlayer.x * MAX_DISTANCE,
		50.0f,
		fbxObj1->GetPosition().z + enemyToPlayer.z * MAX_DISTANCE));

	cameraY = Camera::GetEye().y;

	objB->AddTexture(texbuff, modelB->GetDescHeap());
}

void GameScene::Update()
{

#pragma region Calc

	//FunaInput
	FunaInput::Update();

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

	//�e��ϐ�
	XMFLOAT3 objCpos = fbxObj1->GetPosition();
	//XMFLOAT3 objCpos = fbxObj1->GetPosition();
	XMFLOAT3 cameraPos = Camera::GetEye();
	XMFLOAT3 targetPos = Camera::GetTarget();
	XMFLOAT3 enemyToPlayer = OgaJHelper::CalcDirectionVec3(objA->GetPosition(), fbxObj1->GetPosition());
	//XMFLOAT3 enemyToPlayer = OgaJHelper::CalcDirectionVec3(objA->GetPosition(), objC->GetPosition());
	enemyToPlayer = OgaJHelper::CalcNormalizeVec3(enemyToPlayer);
	XMFLOAT3 cameraToPlayer = OgaJHelper::CalcDirectionVec3(Camera::GetEye(), fbxObj1->GetPosition());
	//XMFLOAT3 cameraToPlayer = OgaJHelper::CalcDirectionVec3(Camera::GetEye(), objC->GetPosition());
	cameraToPlayer = OgaJHelper::CalcNormalizeVec3(cameraToPlayer);

	if (!isTarget)
	{
		//pad
		if (FunaInput::isConnect)
		{
			if (FunaInput::isPadTrigger(XINPUT_GAMEPAD_RIGHT_THUMB))
			{
				cameraMoveEase = 0.0f;
				isTarget = true;
			}

			if (0.3 < fabs(FunaInput::isPadThumb(XINPUT_THUMB_LEFTSIDE)) ||
				0.3 < fabs(FunaInput::isPadThumb(XINPUT_THUMB_LEFTVERT)))
			{
				animationType = RUN;

				XMFLOAT3 vec = { 0,0,0 };
				vec.x = FunaInput::isPadThumb(XINPUT_THUMB_LEFTSIDE);
				vec.z = FunaInput::isPadThumb(XINPUT_THUMB_LEFTVERT);

				objCpos.x += vec.z * cameraToPlayer.x * MAX_MOVE_SPEED;
				objCpos.z += vec.z * cameraToPlayer.z * MAX_MOVE_SPEED;

				float rad = atan2(cameraToPlayer.x, cameraToPlayer.z);
				rad += DirectX::XM_PI / 2;
				objCpos.x += vec.x * sinf(rad) * MAX_MOVE_SPEED;
				objCpos.z += vec.x * cosf(rad) * MAX_MOVE_SPEED;

				cameraPos.x += vec.z * cameraToPlayer.x * MAX_MOVE_SPEED;
				cameraPos.z += vec.z * cameraToPlayer.z * MAX_MOVE_SPEED;
				cameraPos.x += vec.x * sinf(rad) * MAX_MOVE_SPEED;
				cameraPos.z += vec.x * cosf(rad) * MAX_MOVE_SPEED;

				targetPos.x += vec.z * cameraToPlayer.x * MAX_MOVE_SPEED;
				targetPos.z += vec.z * cameraToPlayer.z * MAX_MOVE_SPEED;
				targetPos.x += vec.x * sinf(rad) * MAX_MOVE_SPEED;
				targetPos.z += vec.x * cosf(rad) * MAX_MOVE_SPEED;

				float deg = atan2(vec.x, vec.z);
				OgaJHelper::ConvertToDegree(deg);
				float cameraRad = atan2(cameraToPlayer.x, cameraToPlayer.z);
				OgaJHelper::ConvertToDegree(cameraRad);

				Camera::SetEye(cameraPos);
				Camera::SetTarget(targetPos);
				fbxObj1->SetRotation(XMFLOAT3(0, deg + cameraRad, 0));
				fbxObj2->SetRotation(XMFLOAT3(0, deg + cameraRad, 0));
				fbxObj3->SetRotation(XMFLOAT3(0, deg + cameraRad, 0));

				fbxShadow1->SetRotation(XMFLOAT3(0, deg + cameraRad, 0));
				fbxShadow2->SetRotation(XMFLOAT3(0, deg + cameraRad, 0));
				fbxShadow3->SetRotation(XMFLOAT3(0, deg + cameraRad, 0));
			}

			else { animationType = STAND; }

			if (0.3 < fabs(FunaInput::isPadThumb(XINPUT_THUMB_RIGHTSIDE)) ||
				0.3 < fabs(FunaInput::isPadThumb(XINPUT_THUMB_RIGHTVERT)))
			{
				//�������ǂ��ɂ�������,target�͍ň��⊮������
				XMFLOAT3 objc = fbxObj1->GetPosition();
				//XMFLOAT3 objc = objC->GetPosition();
				XMFLOAT3 eye = Camera::GetEye();

				XMFLOAT3 playerToCamera = OgaJHelper::CalcDirectionVec3(fbxObj1->GetPosition(), Camera::GetEye());
				//XMFLOAT3 playerToCamera = OgaJHelper::CalcDirectionVec3(objC->GetPosition(), Camera::GetEye());
				playerToCamera = OgaJHelper::CalcNormalizeVec3(playerToCamera);

				float xz = atan2(playerToCamera.x, playerToCamera.z);
				OgaJHelper::ConvertToDegree(xz);
				//�v�Z
				xz += FunaInput::isPadThumb(XINPUT_THUMB_RIGHTSIDE) * MAX_CAMERA_MOVE_SPEED;
				cameraPos.y += FunaInput::isPadThumb(XINPUT_THUMB_RIGHTVERT) * MAX_CAMERA_MOVE_SPEED;

				//�����ƍ���
				float diff = 0;
				if (cameraPos.y < 0)
				{
					//�n�ʂƂ̉�����
					diff = cameraPos.y;
					cameraPos.y = 0;
				}

				//rad�ɖ߂�
				OgaJHelper::ConvertToRadian(xz);
				float s = sinf(xz);
				float c = cosf(xz);
				//sin.cos
				Camera::SetEye(XMFLOAT3(
					objCpos.x + s * (MAX_DISTANCE + diff),
					cameraPos.y,
					objCpos.z + c * (MAX_DISTANCE + diff)
				));
			}

			if (isEase)
			{
				XMFLOAT3 target;
				if (cameraMoveEase < 1.0f)
				{
					target = OgaJEase::easeOutCubicXMFLOAT3(Camera::GetTarget(), objC->GetPosition(), cameraMoveEase);
					cameraMoveEase += EASE_CAMERA_TIMER * 2;
				}
				else
				{
					cameraMoveEase = 1.0f;
					target = OgaJEase::easeOutCubicXMFLOAT3(Camera::GetTarget(), objC->GetPosition(), cameraMoveEase);
				}
				Camera::SetTarget(target);
			}
		}

		//�L�[(��U���u)
		else
		{
			if (Input::isKeyTrigger(DIK_T))
			{
				cameraMoveEase = 0.0f;
				isTarget = true;
			}

			//�J�����n
			if (Input::isKey(DIK_RIGHT) || Input::isKey(DIK_LEFT) || Input::isKey(DIK_UP) || Input::isKey(DIK_DOWN))
			{
				if (Input::isKey(DIK_RIGHT))
				{
					cameraPos.x += MAX_CAMERA_MOVE_SPEED;
					targetPos.x -= MAX_CAMERA_MOVE_SPEED;
				}
				if (Input::isKey(DIK_LEFT))
				{
					cameraPos.x -= MAX_CAMERA_MOVE_SPEED;
					targetPos.x += MAX_CAMERA_MOVE_SPEED;
				}
				if (Input::isKey(DIK_UP))
				{
					cameraPos.y += MAX_CAMERA_MOVE_SPEED;
					targetPos.y -= MAX_CAMERA_MOVE_SPEED;
				}
				if (Input::isKey(DIK_DOWN))
				{
					cameraPos.y -= MAX_CAMERA_MOVE_SPEED;
					targetPos.y += MAX_CAMERA_MOVE_SPEED;
				}

				//�����ƍ���
				float diff = 0;

				//�n�ʂƂ̉�����
				if (cameraPos.y < 0) {
					//diff = cameraPos.y;
					//cameraPos.y = 0;
				}

				//�Z�b�g
				XMFLOAT3 targetToCamera = OgaJHelper::CalcDirectionVec3(targetPos, cameraPos);
				targetToCamera = OgaJHelper::CalcNormalizeVec3(targetToCamera);
				XMFLOAT3 cameraToTarget = OgaJHelper::CalcDirectionVec3(cameraPos, targetPos);
				cameraToTarget = OgaJHelper::CalcNormalizeVec3(cameraToTarget);

				/*Camera::SetEye(XMFLOAT3(
					objC->GetPosition().x + targetToCamera.x * (MAX_DISTANCE + diff),
					objC->GetPosition().y + targetToCamera.y * (MAX_DISTANCE + diff),
					objC->GetPosition().z + targetToCamera.z * (MAX_DISTANCE + diff)
				));*/
				Camera::SetEye(cameraPos);
				/*Camera::SetTarget(XMFLOAT3(
					objC->GetPosition().x + cameraToTarget.x * (MAX_DISTANCE + diff),
					objC->GetPosition().y + cameraToTarget.y * (MAX_DISTANCE + diff),
					objC->GetPosition().z + cameraToTarget.z * (MAX_DISTANCE + diff)
				));*/
				Camera::SetTarget(targetPos);

				XMFLOAT3 eye = Camera::GetEye();
				XMFLOAT3 target = Camera::GetTarget();
			}

			//�ړ��Ɖ�]
			if (Input::isKey(DIK_W) || Input::isKey(DIK_S) || Input::isKey(DIK_D) || Input::isKey(DIK_A))
			{
				animationType = RUN;

				XMFLOAT2 vec = { 0,0 };

				if (Input::isKey(DIK_W))
				{
					objCpos.x += cameraToPlayer.x * MAX_MOVE_SPEED;
					objCpos.z += cameraToPlayer.z * MAX_MOVE_SPEED;

					cameraPos.x += cameraToPlayer.x * MAX_MOVE_SPEED;
					cameraPos.z += cameraToPlayer.z * MAX_MOVE_SPEED;

					targetPos.x += cameraToPlayer.x * MAX_MOVE_SPEED;
					targetPos.z += cameraToPlayer.z * MAX_MOVE_SPEED;

					vec.x += 0;
					vec.y += 1;
				}

				if (Input::isKey(DIK_S))
				{
					objCpos.x += cameraToPlayer.x * MAX_MOVE_SPEED * -1;
					objCpos.z += cameraToPlayer.z * MAX_MOVE_SPEED * -1;

					cameraPos.x += cameraToPlayer.x * MAX_MOVE_SPEED * -1;
					cameraPos.z += cameraToPlayer.z * MAX_MOVE_SPEED * -1;

					targetPos.x += cameraToPlayer.x * MAX_MOVE_SPEED * -1;
					targetPos.z += cameraToPlayer.z * MAX_MOVE_SPEED * -1;

					vec.x += 0;
					vec.y += -1;
				}

				if (Input::isKey(DIK_D))
				{
					float cameraRad = atan2(cameraToPlayer.x, cameraToPlayer.z);
					cameraRad += DirectX::XM_PI / 2;
					objCpos.x += sinf(cameraRad) * MAX_MOVE_SPEED;
					objCpos.z += cosf(cameraRad) * MAX_MOVE_SPEED;

					cameraPos.x += sinf(cameraRad) * MAX_MOVE_SPEED;
					cameraPos.z += cosf(cameraRad) * MAX_MOVE_SPEED;

					targetPos.x += sinf(cameraRad) * MAX_MOVE_SPEED;
					targetPos.z += cosf(cameraRad) * MAX_MOVE_SPEED;
					//OgaJHelper::ConvertToDegree(cameraRad);

					vec.x += -1;
					vec.y += 0;
				}

				if (Input::isKey(DIK_A)) {
					float cameraRad = atan2(cameraToPlayer.x, cameraToPlayer.z);
					cameraRad -= DirectX::XM_PI / 2;
					objCpos.x += sinf(cameraRad) * MAX_MOVE_SPEED;
					objCpos.z += cosf(cameraRad) * MAX_MOVE_SPEED;

					cameraPos.x += sinf(cameraRad) * MAX_MOVE_SPEED;
					cameraPos.z += cosf(cameraRad) * MAX_MOVE_SPEED;

					targetPos.x += sinf(cameraRad) * MAX_MOVE_SPEED;
					targetPos.z += cosf(cameraRad) * MAX_MOVE_SPEED;
					//OgaJHelper::ConvertToDegree(cameraRad);

					vec.x += 1;
					vec.y += 0;
				}

				float deg = atan2(vec.y, vec.x);
				OgaJHelper::ConvertToDegree(deg);
				float cameraRad = atan2(cameraToPlayer.x, cameraToPlayer.z);
				OgaJHelper::ConvertToDegree(cameraRad);

				Camera::SetEye(cameraPos);
				Camera::SetTarget(targetPos);
				fbxObj1->SetRotation(XMFLOAT3(0, deg + cameraRad - 90.0f, 0));
				//objC->SetRotation(XMFLOAT3(0, deg + cameraRad - 90.0f, 0));
				//fbxObj1->SetRotation(XMFLOAT3(0, deg + cameraRad - 90.0f, 0));
				fbxObj2->SetRotation(XMFLOAT3(0, deg + cameraRad - 90.0f, 0));
				fbxObj3->SetRotation(XMFLOAT3(0, deg + cameraRad - 90.0f, 0));
				//obj1->SetRotation(XMFLOAT3(0, deg + cameraRad + 90.0f, 0));

				fbxShadow1->SetRotation(XMFLOAT3(0, deg + cameraRad - 90.0f, 0));
				fbxShadow2->SetRotation(XMFLOAT3(0, deg + cameraRad - 90.0f, 0));
				fbxShadow3->SetRotation(XMFLOAT3(0, deg + cameraRad - 90.0f, 0));
			}

			else { animationType = STAND; }
		}
	}

	else
	{
		//pad
		if (FunaInput::isConnect)
		{
			if (FunaInput::isPadTrigger(XINPUT_GAMEPAD_RIGHT_THUMB))
			{
				cameraMoveEase = 0.0f;
				isTarget = false;
				isEase = true;
			}

			if (0.3 < fabs(FunaInput::isPadThumb(XINPUT_THUMB_LEFTSIDE)) ||
				0.3 < fabs(FunaInput::isPadThumb(XINPUT_THUMB_LEFTVERT)))
			{
				animationType = RUN;

				XMFLOAT3 vec = { 0,0,0 };
				vec.x += FunaInput::isPadThumb(XINPUT_THUMB_LEFTSIDE);
				vec.z += FunaInput::isPadThumb(XINPUT_THUMB_LEFTVERT);
				//XMFLOAT3 normalize = OgaJHelper::CalcNormalizeVec3(XMFLOAT3(vec.x, vec.y, 0));

				objCpos.x += vec.z * cameraToPlayer.x * MAX_MOVE_SPEED;
				objCpos.z += vec.z * cameraToPlayer.z * MAX_MOVE_SPEED;

				float rad = atan2(cameraToPlayer.x, cameraToPlayer.z);
				rad += DirectX::XM_PI / 2;
				objCpos.x += vec.x * sinf(rad) * MAX_MOVE_SPEED;
				objCpos.z += vec.x * cosf(rad) * MAX_MOVE_SPEED;
			}

			else { animationType = STAND; }
		}

		else
		{
			//�؂�ւ�
			if (Input::isKeyTrigger(DIK_T))
			{
				cameraMoveEase = 0.0f;
				isTarget = false;
			}

			//�ړ�
			if (Input::isKey(DIK_W) || Input::isKey(DIK_S) || Input::isKey(DIK_D) || Input::isKey(DIK_A))
			{
				animationType = RUN;

				if (Input::isKey(DIK_W))
				{
					objCpos.x += cameraToPlayer.x * MAX_MOVE_SPEED;
					objCpos.z += cameraToPlayer.z * MAX_MOVE_SPEED;
				}

				if (Input::isKey(DIK_S))
				{
					objCpos.x += cameraToPlayer.x * MAX_MOVE_SPEED * -1;
					objCpos.z += cameraToPlayer.z * MAX_MOVE_SPEED * -1;
				}

				if (Input::isKey(DIK_D))
				{
					float cameraRad = atan2(cameraToPlayer.x, cameraToPlayer.z);
					cameraRad += DirectX::XM_PI / 2;
					objCpos.x += sinf(cameraRad) * MAX_MOVE_SPEED;
					objCpos.z += cosf(cameraRad) * MAX_MOVE_SPEED;
				}

				if (Input::isKey(DIK_A)) {
					float cameraRad = atan2(cameraToPlayer.x, cameraToPlayer.z);
					cameraRad -= DirectX::XM_PI / 2;
					objCpos.x += sinf(cameraRad) * MAX_MOVE_SPEED;
					objCpos.z += cosf(cameraRad) * MAX_MOVE_SPEED;
				}
			}

			else { animationType = STAND; }
		}

		//�S�[���n�_
		const XMFLOAT3 GoalCameraTarget = {
			objA->GetPosition().x,
			objA->GetPosition().y - 30.0f,
			objA->GetPosition().z
		};
		const XMFLOAT3 GoalCameraEye = {
			objCpos.x + enemyToPlayer.x * MAX_DISTANCE,
			50.0f,
			objCpos.z + enemyToPlayer.z * MAX_DISTANCE,
		};

		XMFLOAT3 target;
		XMFLOAT3 eye;

		//�J���������Ǘ��p
		if (cameraMoveEase < 1.0f)
		{
			target = OgaJEase::easeOutCubicXMFLOAT3(
				Camera::GetTarget(),
				GoalCameraTarget,
				cameraMoveEase);
			eye = OgaJEase::easeOutCubicXMFLOAT3(
				Camera::GetEye(),
				GoalCameraEye,
				cameraMoveEase);

			cameraMoveEase += EASE_CAMERA_TIMER;
		}

		else
		{
			cameraMoveEase = 1.0f;

			target = OgaJEase::easeOutCubicXMFLOAT3(
				Camera::GetTarget(),
				GoalCameraTarget,
				cameraMoveEase);
			eye = OgaJEase::easeOutCubicXMFLOAT3(
				Camera::GetEye(),
				GoalCameraEye,
				cameraMoveEase);
		}

		//�Z�b�g
		Camera::SetTarget(target);
		Camera::SetEye(eye);

		//����
		float pRadian = atan2(cosf(fbxObj1->GetRotation().z), sinf(fbxObj1->GetRotation().x));
		//float pRadian = atan2(cosf(objC->GetRotation().z), sinf(objC->GetRotation().x));
		OgaJHelper::ConvertToDegree(pRadian);
		float cRadian = atan2(cameraToPlayer.z, cameraToPlayer.x);
		OgaJHelper::ConvertToDegree(cRadian);
		float rot = OgaJHelper::RotateEarliestArc(pRadian, cRadian) * -1;
		//float sub = objC->GetRotation().y - rot;
		float diff = 0;
		if (fbxObj1->GetRotation().y - rot > 0) { diff = 2.0f; }
		//if (objC->GetRotation().y - rot > 0) { diff = 2.0f; }
		else if (fbxObj1->GetRotation().y - rot < 0) { diff = -2.0f; }
		//else if (objC->GetRotation().y - rot < 0) { diff = -2.0f; }
		fbxObj1->SetRotation(XMFLOAT3(
			fbxObj1->GetRotation().x,
			rot + diff,
			fbxObj1->GetRotation().z
		));

		fbxObj2->SetRotation(XMFLOAT3(
			fbxObj1->GetRotation().x,
			rot + diff,
			fbxObj1->GetRotation().z
		));

		fbxObj3->SetRotation(XMFLOAT3(
			fbxObj1->GetRotation().x,
			rot + diff,
			fbxObj1->GetRotation().z
		));

		/*objC->SetRotation(XMFLOAT3(
			objC->GetRotation().x,
			rot + diff,
			objC->GetRotation().z
		));*/
	}

#pragma endregion

	/*----------Update,Setter----------*/
	objB->SetPosition(XMFLOAT3(0.0f, ImguiControl::Imgui_ground_y, 0.0f));
	fbxObj1->SetPosition(objCpos);
	fbxObj2->SetPosition(objCpos);
	fbxObj3->SetPosition(objCpos);

	fbxShadow1->SetPosition(objCpos);
	fbxShadow2->SetPosition(objCpos);
	fbxShadow3->SetPosition(objCpos);

	objD->SetPosition(Camera::GetTarget());

	Object::SetLight(light);
	FbxObject3D::SetLight(light);

	objA->Update();
	objB->Update();
	objC->Update();
	objD->Update();

	objSword->SetPosition(XMFLOAT3(
		ImguiControl::Imgui_Sword_x,
		ImguiControl::Imgui_Sword_y,
		ImguiControl::Imgui_Sword_z));

	objSword->SetRotation(XMFLOAT3(
		ImguiControl::Imgui_Sword_rotx,
		ImguiControl::Imgui_Sword_roty,
		ImguiControl::Imgui_Sword_rotz));

	if (Input::isKeyTrigger(DIK_1))
	{
		fbxObj1->StopAnimation();
		fbxObj2->StopAnimation();
		fbxObj3->StopAnimation();
		fbxShadow1->StopAnimation();
		fbxShadow2->StopAnimation();
		fbxShadow3->StopAnimation();
	}
	if (Input::isKeyTrigger(DIK_2))
	{
		fbxObj1->ResetAnimation();
		fbxObj2->ResetAnimation();
		fbxObj3->ResetAnimation();
		fbxShadow1->ResetAnimation();
		fbxShadow2->ResetAnimation();
		fbxShadow3->ResetAnimation();
	}
	if (Input::isKeyTrigger(DIK_3))
	{
		fbxObj1->ReplayAnimation();
		fbxObj2->ReplayAnimation();
		fbxObj3->ReplayAnimation();
		fbxShadow1->ReplayAnimation();
		fbxShadow2->ReplayAnimation();
		fbxShadow3->ReplayAnimation();
	}

	fbxShadow1->Update(true);
	fbxShadow2->Update(true);
	fbxShadow3->Update(true);

	if (animationType == STAND)
	{
		fbxObj1->Update();
		objSword->MultiMatrix(fbxObj1->GetMatrix());
		objShadowSword->MultiMatrix(fbxObj1->GetMatrix());
	}
	else if (animationType == SLOWRUN)
	{
		fbxObj2->Update();
		objSword->MultiMatrix(fbxObj2->GetMatrix());
		objShadowSword->MultiMatrix(fbxObj2->GetMatrix());
	}
	else if (animationType == RUN)
	{
		fbxObj3->Update();
		objSword->MultiMatrix(fbxObj3->GetMatrix());
		objShadowSword->MultiMatrix(fbxObj3->GetMatrix());
	}

	objSword->Update();
	objShadowSword->SetRotation(objSword->GetRotation());
	objShadowSword->Update(true);

	XMFLOAT3 shadowCameraPos = light->GetShadowLigitEye();
	light->SetShadowLigitEye(XMFLOAT3(
		fbxObj1->GetPosition().x,
		shadowCameraPos.y,
		fbxObj1->GetPosition().z));
	light->SetShadowLigitTarget(fbxObj1->GetPosition());
	light->Update();

	//if (Input::isKeyTrigger(DIK_R)) t = 0.0f;
	//x = (WINDOW_WIDTH - 32 - 0) * OgaJEase::easeInCubic(t) + 0;
	//if (t < 0.99f) t += 0.01f;
	//GH1->SetPosition(XMFLOAT2(x, WINDOW_HEIGHT / 2));

	/*----------Update,Setter----------*/
}

void GameScene::Draw(ID3D12Resource* texbuff)
{
	if (animationType == STAND) { fbxObj1->Draw(DirectXImportant::cmdList.Get(), fbx_normal); }
	else if (animationType == SLOWRUN) { fbxObj2->Draw(DirectXImportant::cmdList.Get(), fbx_normal); }
	else if (animationType == RUN) { fbxObj3->Draw(DirectXImportant::cmdList.Get(), fbx_normal); }

	Object::PreDraw(DirectXImportant::cmdList.Get());
	objB->Draw(receiveShadow);
	objSword->Draw(normal);
	Object::PostDraw();

	//Sprite::PreDraw(DirectXImportant::cmdList.Get());
	//GH1->Draw();
	//Sprite::PostDraw();
}

void GameScene::LuminanceDraw()
{
	if (animationType == STAND) { fbxObj1->Draw(DirectXImportant::cmdList.Get(), fbx_normal); }
	else if (animationType == SLOWRUN) { fbxObj2->Draw(DirectXImportant::cmdList.Get(), fbx_normal); }
	else if (animationType == RUN) { fbxObj3->Draw(DirectXImportant::cmdList.Get(), fbx_normal); }
}

void GameScene::ShadowDraw()
{
	Object::PreDraw(DirectXImportant::cmdList.Get());
	objShadowSword->Draw(shadow);
	Object::PostDraw();

	if (animationType == STAND) { fbxShadow1->Draw(DirectXImportant::cmdList.Get(), fbx_shadow); }
	else if (animationType == SLOWRUN) { fbxShadow2->Draw(DirectXImportant::cmdList.Get(), fbx_shadow); }
	else if (animationType == RUN) { fbxShadow3->Draw(DirectXImportant::cmdList.Get(), fbx_shadow); }
}
