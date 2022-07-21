#include "FbxObjects.h"
#include "../DirectX/Camera.h"
#include "../../imgui/ImguiControl.h"

ID3D12Device* FbxObjects::device = nullptr;
Light* FbxObjects::light = nullptr;

void FbxObjects::StaticInit(ID3D12Device* dev)
{
	assert(!FbxObjects::device);
	assert(device);

	FbxObjects::device = device;
	FbxModels::StaticInit(device);
}

FbxObjects* FbxObjects::Create(FbxModels* model)
{
	FbxObjects* object = new FbxObjects();
	//std::shared_ptr<Object> object = std::make_shared<Object>();
	if (object == nullptr) { return nullptr; }
	if (!object->Init()) { assert(0); }
	if (model) { object->SetModel(model); }

	return object;
}

bool FbxObjects::Init()
{
	HRESULT result;

	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xFF),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBufferDataB0)
	);

	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xFF),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin)
	);

	//1�t���[�����̎��Ԃ�60FPS�Őݒ�
	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < MAX_BONES; i++) {
		constMapSkin->bones[i] = DirectX::XMMatrixIdentity();
	}
	constBuffSkin->Unmap(0, nullptr);

	return true;
}

void FbxObjects::Update(bool isShadowCamera)
{
	DirectX::XMMATRIX matScale, matRot, matTrans;

	//�A�j���[�V����
	if (isPlay) {
		//1�t���[���i�߂�
		currentTime += frameTime;

		//�Ō�܂ōĐ�������擪�ɖ߂�
		if (currentTime > endTime) { currentTime = startTime; }
	}

	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = DirectX::XMMatrixIdentity();
	matRot *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.z));
	matRot *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.x));
	matRot *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.y));
	matTrans = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	//���[���h�s��̍���
	matWorld = DirectX::XMMatrixIdentity(); //�ό`�����Z�b�g
	matWorld *= matScale;					//���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot;						//���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans;					//���[���h�s��ɕ��s�ړ��𔽉f

	const DirectX::XMMATRIX& matViewProjection =
		Camera::ViewMatrix() * Camera::PerspectiveMatrix();
	const DirectX::XMFLOAT3& cameraPos = Camera::GetEye();

	DirectX::XMMATRIX& lightMatViewProjection = Camera::ViewMatrix();
	if (isShadowCamera)
	{
		//�e�p
		DirectX::XMMATRIX matView = DirectX::XMMatrixLookAtLH(
			XMLoadFloat3(&light->GetShadowLigitEye()),
			XMLoadFloat3(&light->GetShadowLigitTarget()),
			XMLoadFloat3(&light->GetShadowLigitUp()));

		float fov = ImguiControl::Imgui_fov;
		DirectX::XMMATRIX lightMatPerspective = DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(fov),
			(float)WINDOW_WIDTH / WINDOW_HEIGHT,
			0.1f, ImguiControl::Imgui_far_z); //�O�[�A���[

		lightMatViewProjection = matView * lightMatPerspective;
	}

	//���f���̃��b�V���g�����X�t�H�[��
	const DirectX::XMMATRIX& modelTransform = model->GetModelTransform();

	HRESULT result;

	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB0* constMap = nullptr;
	result = constBufferDataB0->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		if (isShadowCamera) { constMap->viewproj = lightMatViewProjection; }
		else { constMap->viewproj = matViewProjection; }
		constMap->world = modelTransform * matWorld;
		constMap->cameraPos = cameraPos;
		constBufferDataB0->Unmap(0, nullptr);
	}

	//�{�[���z��
	std::vector<FbxModels::Bone>& bones = model->GetBones();
	std::vector<std::pair<std::string, DirectX::XMMATRIX>> fbxData;

	//��]�s��z��
	std::vector<DirectX::XMMATRIX> localMatRots;

	//�萔�o�b�t�@�փf�[�^�]��(pmx->fbx�ɂ����f�[�^���Ƃ������Ԃ���񂾒l�ɂȂ�)��������
	ConstBufferDataSkin* constMapSkin = nullptr;
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);
	for (int i = 0; i < bones.size(); i++) {

		//���̎p���s��
		DirectX::XMMATRIX matCurrentPose;

		//���̎p���s����擾
		FbxAMatrix fbxCurrentPose =
			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);

		//XMMATRIX�ɕϊ�
		ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

		//�������ăX�L�j���O�s���
		DirectX::XMMATRIX inverse = DirectX::XMMatrixInverse(nullptr, model->GetModelTransform());
		DirectX::XMMATRIX trans = model->GetModelTransform();
		constMapSkin->bones[i] = trans * bones[i].invInitialPose * matCurrentPose * inverse;

		//FbxData�擾�p
		fbxData.push_back(std::make_pair(bones[i].name, trans * matCurrentPose * matWorld));
		FbxVector4 fbxMatRot =
			bones[i].fbxCluster->GetLink()->EvaluateLocalRotation(currentTime);

		//��]�s��擾�p
		DirectX::XMMATRIX matRot = DirectX::XMMatrixIdentity();
		matRot *= DirectX::XMMatrixRotationZ(static_cast<float>(fbxMatRot.mData[0]) / 180.0f * 3.14f);
		matRot *= DirectX::XMMatrixRotationX(static_cast<float>(fbxMatRot.mData[1]) / 180.0f * 3.14f);
		matRot *= DirectX::XMMatrixRotationY(static_cast<float>(fbxMatRot.mData[2]) / 180.0f * 3.14f);
		localMatRots.push_back(matRot);

		//��̃��[���h�s��擾
		if (bones[i].name.find("RightHand", 0) != std::string::npos)
		{
			FbxAMatrix fbxMatrix = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);
			ConvertMatrixFromFbx(&matrix, fbxMatrix);
			matrix = trans * matrix * matWorld;
		}
	}
	constBuffSkin->Unmap(0, nullptr);

	affineTrans.clear();
	std::copy(fbxData.begin(), fbxData.end(), std::back_inserter(affineTrans));
	matRots.clear();
	std::copy(localMatRots.begin(), localMatRots.end(), std::back_inserter(matRots));
}

void FbxObjects::Draw()
{
	assert(device);
	assert(FbxObjects::cmdList);

	if (model == nullptr) { return; }

	//�p�C�v���C���X�e�[�g�̐ݒ�
	//cmdList->SetPipelineState(pipelineSet.pipelinestate);
	//���[�g�V�O�l�`���̐ݒ�
	//cmdList->SetGraphicsRootSignature(pipelineSet.rootsignature);
	// 

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�萔�o�b�t�@�r���[���Z�b�g��rootoaramIndex�������Ɗm�F���ׂ�
	cmdList->SetGraphicsRootConstantBufferView(
		0,
		constBufferDataB0->GetGPUVirtualAddress()
	);
	cmdList->SetGraphicsRootConstantBufferView(
		2,
		constBuffSkin->GetGPUVirtualAddress()
	);

	light->Draw(cmdList, 3);
	model->Draw(cmdList);
}

void FbxObjects::PlayAnimation()
{
	FbxScene* fbxScene = model->GetFbxScene();

	FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);

	const char* animstackname = animstack->GetName();

	FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);

	startTime = takeinfo->mLocalTimeSpan.GetStart();

	endTime = takeinfo->mLocalTimeSpan.GetStop();

	currentTime = startTime;

	isPlay = true;
}
