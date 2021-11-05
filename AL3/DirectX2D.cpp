#include "DirectX2D.h"
#include <cassert>

DirectX2D::DirectX2D()
{
	/*SpriteDraw ������*/
	sprData.spriteRotation = 0.0f;
	sprData.spriteMatProjection = XMMatrixOrthographicOffCenterLH(
		0.0f,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0.0f,
		0.0f,
		1.0f);
	//sprData.spriteMatProjection= XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(60.0f),
	//	(float)WINDOW_WIDTH / WINDOW_HEIGHT,
	//	0.1f, 1000.0f); //�O�[�A���[
	sprData.spritePosition = { 0.0f,0.0f,0.0f };
	sprData.spriteColor = { 1.0f,1.0f,1.0f,1.0f };
}

void DirectX2D::VSShaderCompile()
{
	HRESULT result;

	/*���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��*/
	result = D3DCompileFromFile(
		L"SpriteVertexShader.hlsl",
		nullptr,
		//�C���N���[�h�\�ɂ���
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		//�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		/*BasicVS.hlsl��main�֐����G���g���[�|�C���g
		�Ƃ����"main"�ƕ�����w�肵�Ă���B*/
		"VSmain", "vs_5_0",
		//�f�o�b�O�p�ݒ�
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);

	if (FAILED(result))
	{
		//errorBolb����̃G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

void DirectX2D::PSShaderCompile()
{
	HRESULT result;

	/*�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��*/
	result = D3DCompileFromFile(
		//�V�F�[�_�t�@�C����
		L"SpritePixelShader.hlsl",
		nullptr,
		//�C���N���[�h�\�ɂ���
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		//�G���g���[�|�C���g���A�V�F�[�_���f���w��
		"PSmain", "ps_5_0",
		//�f�o�b�O�w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);

	if (FAILED(result))
	{
		//errorBolb����̃G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

HRESULT DirectX2D::SpriteLoadTexture(UINT texnumber, const wchar_t* filename)
{
	HRESULT result;

	/*WIC�e�N�X�`���̃��[�h*/
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const Image* img = scratchImg.GetImage(0, 0, 0); //���f�[�^���o

	/*���\�[�X�ݒ�*/
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	/*�e�N�X�`���p�o�b�t�@�̐���*/
	result = DirectXImportant::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, //�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&spriteTexBuff[texnumber]));

	/*�e�N�X�`���o�b�t�@�ւ̃f�[�^�]��*/
	result = spriteTexBuff[texnumber]->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch
	);

	/*�V�F�[�_�[���\�[�X�r���[�ݒ�*/
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};             //�ݒ�\����
	//srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;       //RGBA
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	/*�V�F�[�_�[���\�[�X�r���[�쐬*/
	DirectXImportant::dev->CreateShaderResourceView(
		spriteTexBuff[texnumber].Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,                       //�e�N�X�`���ݒ���
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			spriteDescHeap->GetCPUDescriptorHandleForHeapStart(),
			texnumber,
			DirectXImportant::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	return S_OK;
}

void DirectX2D::InputLayout()
{
	/*���_���C�A�E�g*/
	inputLayout[0] = {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
	};
	inputLayout[1] = {
		"TEXCOORD",
		0,
		DXGI_FORMAT_R32G32_FLOAT,
		0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		0
	};

	/*���_���C�A�E�g�̐ݒ�*/
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	/*�}�`�̌`����O�p�`�ɐݒ�*/
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	/*���̑��̐ݒ�*/
	gpipeline.NumRenderTargets = 1;                       //�`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0~255�w���RGBA
	gpipeline.SampleDesc.Count = 1;                       //1�s�N�Z���ɂ�1��T���v�����O
}

void DirectX2D::SettingPipelineState()
{
	/*���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�*/
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	/*�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�*/
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;                        //�W���ݒ�
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);      //�W���ݒ�i�w�ʃJ�����O�A�h��Ԃ��A�[�x�N���b�s���O�L���j
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;               //�w�ʃJ�����O
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT); //�W���ݒ�i�[�x�e�X�g���s���A�������݋��A�[�x����������΍��i�j
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;    //��ɏ㏑�����[��

	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	//�W���ݒ�
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	/*���ʐݒ�*/
	//�u�����h��L���ɂ���
	blenddesc.BlendEnable = true;
	//���Z
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	//�\�[�X�̒l��100%�g��
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	//�f�X�g�̒l��0%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	/*���Z����*/
	////���Z
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	////�\�[�X�̒l��100%�g��
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;
	////�f�X�g�̒l��0%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;

	/*���Z����*/
	////�f�X�g����\�[�X�����Z
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	////�\�[�X�̒l��100%�g��
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;
	////�f�X�g�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;

	/*�F���]*/
	////���Z
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	////1.0f - �f�X�g�J���[�̒l
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
	////�g��Ȃ�
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;

	/*����������*/
	//���Z
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//�\�[�X�̃A���t�@�l
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	//1.0f - �\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//�u�����h�X�e�[�g�ɐݒ肷��
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	gpipeline.BlendState.AlphaToCoverageEnable = true;
}

void DirectX2D::RootSignature()
{
	HRESULT result;

	/*�f�X�N���v�^�����W*/
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};                   //�ݒ�\����
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //�V�F�[�_�[���猩����
	descHeapDesc.NumDescriptors = 3;                                //�o�b�t�@��2��

	result = DirectXImportant::dev->CreateDescriptorHeap(
		&descHeapDesc, IID_PPV_ARGS(&spriteDescHeap));              //����

	//�f�X�N���v�^�q�[�v�̐擪�n���h�����擾���Ă���
	/*D3D12_CPU_DESCRIPTOR_HANDLE */basicHeapHandle =
		spriteDescHeap->GetCPUDescriptorHandleForHeapStart();

	/*���[�g�p�����[�^�̐���*/
	CD3DX12_ROOT_PARAMETER rootparams[2];

	/*�萔�p*/
	rootparams[0].InitAsConstantBufferView(
		0,
		0,
		D3D12_SHADER_VISIBILITY_ALL);

	/*�e�N�X�`���p*/
	rootparams[1].InitAsDescriptorTable(
		1,
		&descRangeSRV,
		D3D12_SHADER_VISIBILITY_ALL);

	/*�f�X�N���v�^�e�[�u���̏�����*/
	/*�萔�o�b�t�@�̏���CPU/GPU�ԂŐ�������ׂ̐ݒ���s���B*/

	/*�f�X�N���v�^�����W�̐ݒ�*/
	CD3DX12_DESCRIPTOR_RANGE descTblRange{};
	descTblRange.NumDescriptors = 1;                          //�萔��1��
	descTblRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV; //��ʂ͒萔
	descTblRange.BaseShaderRegister = 0;                      //0�ԃX���b�g����
	descTblRange.OffsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                 //�W��

	/*���[�g�p�����[�^�̐ݒ�*/
	CD3DX12_ROOT_PARAMETER rootparam = {};
	rootparam.ParameterType =
		D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;              //���
	rootparam.DescriptorTable.pDescriptorRanges = &descTblRange; //�f�X�N���v�^�����W
	rootparam.DescriptorTable.NumDescriptorRanges = 1;           //�f�X�N���v�^�����W��
	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;    //�S�ẴV�F�[�_���猩����

	/*�T���v���[�f�X�N*/
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                //���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                //�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                //���J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK; //�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;                   //��Ԃ��Ȃ��i�j�A���X�g�l�C�o�[�j
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                                //�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;                                             //�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;          //�s�N�Z���V�F�[�_����̂݉�
	//CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	/*���[�g�V�O�l�`���̐���*/
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc; //���[�g�V�O�l�`���̐ݒ�
	rootSignatureDesc.Init_1_0(
		_countof(rootparams),
		rootparams,
		1,
		&samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;

	result = D3DX12SerializeVersionedRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);                                         //�o�[�W������������ł̃V���A���C�Y
	//assert(SUCCEEDED(DirectXImportant::result));

	result = DirectXImportant::dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&spriteRootSignature));              //���[�g�V�O�l�`���̐���
	//assert(SUCCEEDED(DirectXImportant::result));

	gpipeline.pRootSignature = spriteRootSignature.Get(); //�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
}

void DirectX2D::CreatePipelineState()
{
	HRESULT result;

	//ID3D12Device* d = DirectXImportant::dev.Get();

	//D3D12_GRAPHICS_PIPELINE_STATE_DESC lpipeline{};

	//HRESULT result = DirectXImportant::dev->CreateGraphicsPipelineState(
	//	&lpipeline,
	//	IID_PPV_ARGS(&pipelinestate));


	result = DirectXImportant::dev->CreateGraphicsPipelineState(
		&gpipeline,
		IID_PPV_ARGS(&spritePipelineState));
	//assert(SUCCEEDED(DirectXImportant::result));

	const int spriteSRVCount = 512;
	//�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = spriteSRVCount;
	result = DirectXImportant::dev->CreateDescriptorHeap(
		&descHeapDesc,
		IID_PPV_ARGS(&spriteDescHeap));
}

void DirectX2D::PipelineStateObjInit()
{
	VSShaderCompile();
	PSShaderCompile();
	//SpriteLoadTexture();
	InputLayout();
	SettingPipelineState();
	RootSignature();
	CreatePipelineState();
}

HRESULT DirectX2D::CreateSpriteInit()
{
	HRESULT result = S_FALSE;

	/*�����A����A�E���A�E��*/
	enum { LB, LT, RB, RT };

	/*�e�N�X�`�����擾*/
	D3D12_RESOURCE_DESC resDesc =
		spriteTexBuff[sprData.texNumber]->GetDesc();
	float WIDTH = (float)resDesc.Width;
	float HEIGHT = (float)resDesc.Height;

	float left = (0.0f - anchorpoint.x) * WIDTH;
	float right = (1.0f - anchorpoint.x) * WIDTH;
	float top = (0.0f - anchorpoint.y) * HEIGHT;
	float bottom = (1.0f - anchorpoint.y) * HEIGHT;

	if (isFlipX) {
		left = -left;
		right = -right;
	}
	if (isFlipY) {
		top = -top;
		bottom = -bottom;
	}

	/*���_�f�[�^*/
	VertexPosUv vertices[] = {
		{{  left, bottom, 0.0f},{0.0f,1.0f}}, //����
		{{  left,    top, 0.0f},{0.0f,0.0f}}, //����
		{{ right, bottom, 0.0f},{1.0f,1.0f}}, //�E��
		{{ right,    top, 0.0f},{1.0f,0.0f}}, //�E��
	};

	/*���_�o�b�t�@����*/
	result = DirectXImportant::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteVertBuff));
	//assert(SUCCEEDED(result));

	/*�o�b�t�@�ւ̃f�[�^�]��*/
	VertexPosUv* vertMap = nullptr;
	result = spriteVertBuff->Map(
		0,
		nullptr,
		(void**)&vertMap);
	//assert(SUCCEEDED(result));
	memcpy(vertMap, vertices, sizeof(vertices));
	spriteVertBuff->Unmap(0, nullptr);

	//���\�[�X�ݒ�
	ComPtr<ID3D12Resource> depthBuffer;
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//���\�[�X����
	result = DirectXImportant::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, //�[�x�l�������݂Ɏg�p
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer));

	/*�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬*/
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	result = DirectXImportant::dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	/*�[�x�X�e���V���r���[�̍쐬*/
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DirectXImportant::dev->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());

	/*���_�o�b�t�@�r���[�̍쐬*/
	spriteVBView.BufferLocation = spriteVertBuff->GetGPUVirtualAddress();
	spriteVBView.SizeInBytes = sizeof(vertices);
	spriteVBView.StrideInBytes = sizeof(vertices[0]);

	//D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};                   //�ݒ�\����
	//descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //�V�F�[�_�[���猩����
	//descHeapDesc.NumDescriptors = 3;                                //�o�b�t�@��2��

	//result = DirectXImportant::dev->CreateDescriptorHeap(
	//	&descHeapDesc, IID_PPV_ARGS(&spriteDescHeap));               //����

	////�f�X�N���v�^�q�[�v�̐擪�n���h�����擾���Ă���
	///*D3D12_CPU_DESCRIPTOR_HANDLE */basicHeapHandle =
	//	spriteDescHeap->GetCPUDescriptorHandleForHeapStart();

	/*�萔�o�b�t�@�̐���*/
	result = DirectXImportant::dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteConstBuff));
	//assert(SUCCEEDED(result));

	/*�萔�o�b�t�@�Ƀf�[�^�]��*/
	ConstBufferData* constMap = nullptr;
	result = spriteConstBuff->Map(
		0,
		nullptr,
		(void**)&constMap);
	//assert(SUCCEEDED(result));
	constMap->color = XMFLOAT4(1, 1, 1, 1);
	constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f,
		1280, //�v�C��
		720,
		0.0f,
		0.0f,
		1.0f); //���s���e�s��̍���
	spriteConstBuff->Unmap(0, nullptr);

	return result;
}

void DirectX2D::SpriteUpdate()
{
	/*�p�C�v���C���X�e�[�g�̐ݒ�*/
	DirectXImportant::cmdList->SetPipelineState(spritePipelineState.Get());

	/*���[�g�V�O�l�`���̐ݒ�*/
	DirectXImportant::cmdList->SetGraphicsRootSignature(spriteRootSignature.Get());

	/*�v���~�e�B�u�`���ݒ�*/
	DirectXImportant::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void DirectX2D::SpriteDraw(bool isDraw)
{
	if (!isDraw) {
		return;
	}

	/*���[���h�s��̍X�V*/
	sprData.spriteMatWorld = XMMatrixIdentity();
	sprData.spriteMatWorld *= XMMatrixRotationZ(XMConvertToRadians(sprData.spriteRotation));
	sprData.spriteMatWorld *= XMMatrixTranslationFromVector(sprData.spritePosition);

	/*�s��̓]��*/
	ConstBufferData* constMap = nullptr;
	HRESULT result = spriteConstBuff->Map(
		0,
		nullptr,
		(void**)&constMap);
	constMap->color = sprData.spriteColor;
	constMap->mat = sprData.spriteMatWorld * sprData.spriteMatProjection;
	spriteConstBuff->Unmap(0, nullptr);

	/*�f�X�N���v�^�q�[�v�̔z��*/
	ID3D12DescriptorHeap* ppHeaps[] = { spriteDescHeap.Get() };
	DirectXImportant::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	/*���_�o�b�t�@���Z�b�g*/
	DirectXImportant::cmdList->IASetVertexBuffers(0, 1, &spriteVBView);

	/*�萔�o�b�t�@���Z�b�g*/
	DirectXImportant::cmdList->SetGraphicsRootConstantBufferView(
		0,
		spriteConstBuff->GetGPUVirtualAddress());

	/*�V�F�[�_���\�[�X�r���[���Z�b�g*/
	DirectXImportant::cmdList->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteDescHeap->GetGPUDescriptorHandleForHeapStart(),
			sprData.texNumber,
			DirectXImportant::dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	/*�`��R�}���h*/
	DirectXImportant::cmdList->DrawInstanced(4, 1, 0, 0);
}
