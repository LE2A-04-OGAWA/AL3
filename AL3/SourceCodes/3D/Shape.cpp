#include "Shape.h"
#include <DirectXTex.h>
#include <d3dcompiler.h>
#include <string>
#include <d3dx12.h>
#include "../Input/Input.h"
//#include <dinput.h>

#pragma comment(lib,"d3dcompiler.lib")

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	DirectX::XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

Shape::Shape()
{
	scale = { 1.0f,1.0f,1.0f };
	rotation = { 0.0f,0.0f,0.0f };  //XYZ������ɉ��x��]���邩
	position = { 0.0f,0.0f,0.0f };
}

void Shape::CreateCube(float r, const wchar_t* filename)
{
	HRESULT result;
	//�O
	CubeVertices.push_back({ {-r, -r, -r}, {}, {0.0f, 1.0f} });
	CubeVertices.push_back({ {-r,  r, -r}, {}, {0.0f, 0.0f} });
	CubeVertices.push_back({ { r, -r, -r}, {}, {1.0f, 1.0f} });
	CubeVertices.push_back({ { r,  r, -r}, {}, {1.0f, 0.0f} });
	//��
	CubeVertices.push_back({ {-r, -r, r}, {}, {0.0f, 1.0f} });
	CubeVertices.push_back({ {-r,  r, r}, {}, {0.0f, 0.0f} });
	CubeVertices.push_back({ { r, -r, r}, {}, {1.0f, 1.0f} });
	CubeVertices.push_back({ { r,  r, r}, {}, {1.0f, 0.0f} });
	//��
	CubeVertices.push_back({ {-r, -r, -r}, {}, {0.0f, 1.0f} });
	CubeVertices.push_back({ {-r, -r,  r}, {}, {0.0f, 0.0f} });
	CubeVertices.push_back({ {-r,  r, -r}, {}, {1.0f, 1.0f} });
	CubeVertices.push_back({ {-r,  r,  r}, {}, {1.0f, 0.0f} });
	//�E
	CubeVertices.push_back({ { r, -r,  r}, {}, {0.0f, 1.0f} });
	CubeVertices.push_back({ { r, -r, -r}, {}, {0.0f, 0.0f} });
	CubeVertices.push_back({ {r, r, r}, {}, {0.0f, 1.0f} });
	CubeVertices.push_back({ {r, r, -r}, {}, {0.0f, 1.0f} });
	//��
	CubeVertices.push_back({ {-r, -r, -r}, {}, {0.0f, 1.0f} });
	CubeVertices.push_back({ {-r, -r, r}, {}, {0.0f, 1.0f} });
	CubeVertices.push_back({ {r, -r, -r}, {}, {0.0f, 1.0f} });
	CubeVertices.push_back({ {r, -r, r}, {}, {0.0f, 1.0f} });
	//��
	CubeVertices.push_back({ {-r, r, -r}, {}, {0.0f, 1.0f} });
	CubeVertices.push_back({ {-r, r, r}, {}, {0.0f, 1.0f} });
	CubeVertices.push_back({ {r, r, -r}, {}, {0.0f, 1.0f} });
	CubeVertices.push_back({ {r, r, r}, {}, {0.0f, 1.0f} });

	/*�C���f�b�N�X�f�[�^*/
	CubeIndices = {
		//�O
		0,1,2,    //�O�p�`1��
		2,1,3,    //�O�p�`2��

		//��
		5,4,6,    //�O�p�`3��
		5,6,7,    //�O�p�`4��

		//��
		8,9,10,   //�O�p�`5��
		10,9,11,  //�O�p�`6��

		//�E
		12,13,14, //�O�p�`7��
		14,13,15, //�O�p�`8��

		//��
		16,17,18, //�O�p�`9��
		18,17,19, //�O�p�`10��

		//��
		20,21,22, //�O�p�`11��
		22,21,23, //�O�p�`12��
	};

	/*WIC�e�N�X�`���̃��[�h*/
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename,
		DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const DirectX::Image* img = scratchImg.GetImage(0, 0, 0); //���f�[�^���o

	/*�e�N�X�`���o�b�t�@�̐���*/
	D3D12_HEAP_PROPERTIES texHeapProp{};                       //�e�N�X�`���q�[�v�ݒ�
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//D3D12_RESOURCE_DESC texresDesc{};                          //���\�[�X�ݒ�
	//texresDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; //2D�e�N�X�`���p
	//texresDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;        //RGBA�t�H�[�}�b�g
	//texresDesc.Width = metadata.width;
	//texresDesc.Height = metadata.width;
	//texresDesc.DepthOrArraySize = 1;
	//texresDesc.MipLevels = 1;
	//texresDesc.SampleDesc.Count = 1;

	D3D12_RESOURCE_DESC texresDesc{};
	texresDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	texresDesc.Format = metadata.format;
	texresDesc.Width = metadata.width;
	texresDesc.Height = (UINT)metadata.height;
	texresDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texresDesc.MipLevels = (UINT16)metadata.mipLevels;
	texresDesc.SampleDesc.Count = 1;

	ID3D12Resource* texbuff = nullptr;
	result = DirectXImportant::dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texbuff));

	////�e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
	//result = texbuff->WriteToSubresource(
	//	0,
	//	nullptr,                        //�S�̈�փR�s�[
	//	texturedata,                    //���f�[�^�A�h���X
	//	sizeof(XMFLOAT4) * metadata.width,    //1���C���T�C�Y
	//	sizeof(XMFLOAT4) * texDataCount //�S�T�C�Y
	//);

	//�e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
	result = texbuff->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch
	);

	//delete[] texturedata;               //���f�[�^���

	//�@���̌v�Z
	for (int i = 0; i < CubeIndices.size() / 3; i++)
	{
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = CubeIndices[i * 3 + 0];
		unsigned short index1 = CubeIndices[i * 3 + 1];
		unsigned short index2 = CubeIndices[i * 3 + 2];

		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		DirectX::XMVECTOR p0 = XMLoadFloat3(&CubeVertices[index0].pos);
		DirectX::XMVECTOR p1 = XMLoadFloat3(&CubeVertices[index1].pos);
		DirectX::XMVECTOR p2 = XMLoadFloat3(&CubeVertices[index2].pos);

		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
		DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(p1, p0);
		DirectX::XMVECTOR v2 = DirectX::XMVectorSubtract(p2, p0);

		//�O�ς͗������琂���ȃx�N�g��
		DirectX::XMVECTOR normal = DirectX::XMVector3Cross(v1, v2);

		//���K��
		normal = DirectX::XMVector3Normalize(normal);

		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&CubeVertices[index0].normal, normal);
		XMStoreFloat3(&CubeVertices[index1].normal, normal);
		XMStoreFloat3(&CubeVertices[index2].normal, normal);
	}

	//UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * CubeVertices.size());
	//UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * (vertices.size()));

	/*���_�o�b�t�@�̊m��*/
	/*GPU���̃��������m�ۂ��āA���_�f�[�^�p�̗̈��ݒ�B
	���_�o�b�t�@�pGPU���\�[�X�̐���*/

	//���_�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapprop{};

	//GPU�ւ̓]���p
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	//�[�x�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResDesc{};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResDesc.Width = WINDOW_WIDTH;                            //�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResDesc.Height = WINDOW_HEIGHT;                          //�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResDesc.DepthOrArraySize = 1;
	depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;                  //�[�x�l�t�H�[�}�b�g
	depthResDesc.SampleDesc.Count = 1;
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; //�f�v�X�X�e���V��

	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;      //�[�x�l1.0f�i�ő�l�j�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; //�[�x�l�t�H�[�}�b�g

	//���\�[�X�ݒ�
	ID3D12Resource* depthBuffer = nullptr;
	result = DirectXImportant::dev->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, //�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuffer));

	//�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	ID3D12DescriptorHeap* dsvHeap = nullptr;
	result = DirectXImportant::dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	//�[�x�X�e���V���r���[�̍쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DirectXImportant::dev->CreateDepthStencilView(
		depthBuffer,
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	//���_��񂪓��镪�̃T�C�Y
	resdesc.Width = sizeof(CubeVertices);
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//GPU���\�[�X�̐���
	result = DirectXImportant::dev->CreateCommittedResource(
		//�q�[�v�ݒ�
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		//���\�[�X�ݒ�
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	/*�C���f�b�N�X�o�b�t�@�̊m��*/
	//�C���f�b�N�X��񂪓��镪�̃T�C�Y
	resdesc.Width = sizeof(CubeIndices);

	//GPU���\�[�X�̐���
	result = DirectXImportant::dev->CreateCommittedResource(
		//�q�[�v�ݒ�
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		//���\�[�X�ݒ�
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	Vertex* vertMap = nullptr;
	unsigned short* indexMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	for (int i = 0; i < CubeVertices.size(); i++) {
		vertMap[i] = CubeVertices[i];
	}
	vertBuff->Unmap(0, nullptr);

	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	for (int i = 0; i < CubeIndices.size(); i++) {
		indexMap[i] = CubeIndices[i];
	}
	indexBuff->Unmap(0, nullptr);

	/*���_�o�b�t�@�r���[�̍쐬*/
	//���_�o�b�t�@���Z�b�g���邽�߂�GPU�R�}���h�u���_�o�b�t�@�r���[���쐬����

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * CubeVertices.size());
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * CubeIndices.size());

	//���_�o�b�t�@�r���[�̍쐬
	//D3D12_VERTEX_BUFFER_VIEW vbView{};
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);
	//vbView.SizeInBytes = vertices.size();
	//vbView.StrideInBytes = sizeof(XMFLOAT3);

	/*�C���f�b�N�X�o�b�t�@�r���[�̍쐬�ƃZ�b�g*/
	//D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
	//ibView.SizeInBytes = indices.size();

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> gsBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicVS.hlsl",
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

	result = D3DCompileFromFile(
		//�V�F�[�_�t�@�C����
		L"BasicGeometryShader.hlsl",
		nullptr,
		//�C���N���[�h�\�ɂ���
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		//�G���g���[�|�C���g���A�V�F�[�_���f���w��
		"GSmain", "gs_5_0",
		//�f�o�b�O�w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&gsBlob, &errorBlob);

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		//�V�F�[�_�t�@�C����
		L"BasicPS.hlsl",
		nullptr,
		//�C���N���[�h�\�ɂ���
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		//�G���g���[�|�C���g���A�V�F�[�_���f���w��
		"PSmain", "ps_5_0",
		//�f�o�b�O�w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);

	/*�V�F�[�_�̃G���[���e��\��*/
	/*�E���_�V�F�[�_�̓ǂݍ���
	�E�s�N�Z���V�F�[�_�̓ǂݍ���
	�̂��Ƃɂ��ꂼ��v2�񁫂��̏�����������B

	�����hlsl�t�@�C���ɕ��@�I��
	��肪�������ꍇ�A
	�u�o�́v�E�B���h�E��
	�G���[���e���\�������B*/

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

	/*���_�V�F�[�_�ɓn�����߂̒��_�f�[�^�𐮂���*/
	/*�O���t�B�b�N�X�p�C�v���C����
	���_1���̃f�[�^�ɉ���
	�������邩������
	���߂邱�Ƃ��o����B

	�����ł͍ŏ�����3D���W������
	�Z�b�g���Ă���B
	����K�v�ɂȂ邲�Ƃɍs��ǉ����Ă����B*/
	//���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};

	/*�O���t�B�b�N�X�p�C�v���C���̊e�X�e�[�W�̐ݒ������\���̂�p�ӂ���B*/
	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	/*���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�*/
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	/*�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�*/

	//�W���ݒ�
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//�J�����O���Ȃ�
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//�|���S�����h��Ԃ�
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	//�g���̂�
	/*���C���[�t���[�����`�悪�y���A����������*/
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	//�[�x�N���b�s���O��L����
	gpipeline.RasterizerState.DepthClipEnable = true;

	/*�u�����h�X�e�[�g��ݒ�*/
	/*gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;*/
		//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�i8���邪����1�����g��Ȃ��j

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState.DepthEnable = true;                          //�[�x�e�X�g���s��
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; //�������݋���
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;      //��������΍��i
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;                             //�[�x�l�t�H�[�}�b�g

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

	/*�萔�o�b�t�@�̏�����*/
	//�萔�o�b�t�@�p�f�[�^�\����
	//struct ConstBufferData {
	//	//�F�iRGBA�j
	//	XMFLOAT4 color;
	//	XMMATRIX mat;   //3D�ϊ��s��
	//};

	/*�o�b�t�@���m�ۂ���̂ŁA
	�V���ɐ�p�̃f�X�N���v�^�q�[�v���쐬����B*/
	//ID3D12DescriptorHeap* basicDescHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};                   //�ݒ�\����
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //�V�F�[�_�[���猩����
	descHeapDesc.NumDescriptors = 2;                                //�o�b�t�@��2��

	result = DirectXImportant::dev->CreateDescriptorHeap(
		&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));               //����

	//�f�X�N���v�^�q�[�v�̐擪�n���h�����擾���Ă���
	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle =
		basicDescHeap->GetCPUDescriptorHandleForHeapStart();

	/*�萔�o�b�t�@�̍쐬*/
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbheapprop{};
	//GPU�ւ̓]���p
	cbheapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbresdesc{};
	cbresdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//256�o�C�g�A���C�������g
	cbresdesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbresdesc.Height = 1;
	cbresdesc.DepthOrArraySize = 1;
	cbresdesc.MipLevels = 1;
	cbresdesc.SampleDesc.Count = 1;
	cbresdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//GPU���\�[�X�̐���
	/*ID3D12Resource* constBuff = nullptr;*/
	result = DirectXImportant::dev->CreateCommittedResource(
		//�q�[�v�ݒ�
		&cbheapprop,
		D3D12_HEAP_FLAG_NONE,
		//���\�[�X�ݒ�
		&cbresdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));


	/*�萔�o�b�t�@�Ƀf�[�^��]������*/
	/*ConstBufferData* constMap = nullptr;*/

	//�}�b�s���O
	result = constBuff->Map(0, nullptr, (void**)&constMap);

	//RGBA
	constMap->color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//���s���e�ϊ�
	/*constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, WINDOW_WIDTH,
		WINDOW_HEIGHT, 0.0f,
		0.0f, 1.0f
	);*/

	//���[���h�ϊ��s��
	matWorld = DirectX::XMMatrixIdentity();

	/*�X�P�[�����O�i�g�k�j*/
	//XMMATRIX matScale;
	matScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	/*��]*/
	//XMMATRIX matRot; //��]�s��
	matRot = DirectX::XMMatrixIdentity();
	matRot *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.z)); //Z������45�x��]
	matRot *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.x)); //X������45�x��]
	matRot *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.y)); //Y������45�x��]

	/*���s�ړ�*/
	//XMMATRIX matTrans;
	matTrans = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	DirectX::XMMATRIX matView;
	DirectX::XMMATRIX matPerspective;
	DirectX::XMFLOAT3 eye;
	DirectX::XMFLOAT3 target;
	DirectX::XMFLOAT3 up;
	float fov;

	eye = { 45,0,-100 };
	target = { 0,0,0 };
	up = { 0,1,0 };
	fov = 60.0f;
	matView = DirectX::XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));
	matPerspective = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(fov),
		(float)WINDOW_WIDTH / WINDOW_HEIGHT,
		0.1f, 1000.0f); //�O�[�A���[

	//Camera camera;
	//constMap->mat = matWorld * camera.ViewMatrix() * camera.PerspectiveMatrix();
	constMap->mat = matWorld * Camera::ViewMatrix() * Camera::PerspectiveMatrix();
	//constMap->mat = matWorld * matView * matPerspective;

	//�}�b�s���O����
	constBuff->Unmap(0, nullptr);

	/*�萔�o�b�t�@�r���[�̍쐬*/
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constBuff->GetDesc().Width;

	DirectXImportant::dev->CreateConstantBufferView(
		&cbvDesc, basicHeapHandle);
	/*�����_�o�b�t�@�r���[�ƈႢ�ACreate����K�v������B��*/

	//������������A�h���X�����炷
	basicHeapHandle.ptr +=
		DirectXImportant::dev->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		);

	/*�V�F�[�_�[���\�[�X�r���[�ݒ�*/
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};             //�ݒ�\����
	//srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;       //RGBA
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	/*�q�[�v��2�ԖڂɃV�F�[�_�[���\�[�X�r���[�쐬*/
	DirectXImportant::dev->CreateShaderResourceView(
		texbuff,  //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		basicHeapHandle
	);

	D3D12_DESCRIPTOR_RANGE descRangeCBV{};                    //�萔�p
	descRangeCBV.NumDescriptors = 1;                          //�萔��1��
	descRangeCBV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV; //��ʂ͒萔
	descRangeCBV.BaseShaderRegister = 0;                      //0�ԃX���b�g����
	descRangeCBV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descRangeSRV{};                    //�e�N�X�`���p
	descRangeSRV.NumDescriptors = 1;                          //�e�N�X�`��1��
	descRangeSRV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //��ʂ̓e�N�X�`��
	descRangeSRV.BaseShaderRegister = 0;                      //0�ԃX���b�g����
	descRangeSRV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootparams[2] = {};
	/*�萔�p*/
	rootparams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; //���
	rootparams[0].DescriptorTable.pDescriptorRanges = &descRangeCBV;
	rootparams[0].DescriptorTable.NumDescriptorRanges = 1;                    //�f�X�N���v�^�����W��
	rootparams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;             //�S�ẴV�F�[�_���猩����
	/*�e�N�X�`���p*/
	rootparams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; //���
	rootparams[1].DescriptorTable.pDescriptorRanges = &descRangeSRV;
	rootparams[1].DescriptorTable.NumDescriptorRanges = 1;                    //�f�X�N���v�^�����W��
	rootparams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;             //�S�ẴV�F�[�_���猩����

	/*�f�X�N���v�^�e�[�u���̏�����*/
	/*�萔�o�b�t�@�̏���CPU/GPU�ԂŐ�������ׂ̐ݒ���s���B*/

	/*�f�X�N���v�^�����W�̐ݒ�*/
	D3D12_DESCRIPTOR_RANGE descTblRange{};
	//�萔��1��
	descTblRange.NumDescriptors = 1;
	//��ʂ͒萔
	descTblRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	//0�ԃX���b�g����
	descTblRange.BaseShaderRegister = 0;
	//�W��
	descTblRange.OffsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	/*���[�g�p�����[�^�̐ݒ�*/
	D3D12_ROOT_PARAMETER rootparam = {};
	//���
	rootparam.ParameterType =
		D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//�f�X�N���v�^�����W
	rootparam.DescriptorTable.pDescriptorRanges = &descTblRange;
	//�f�X�N���v�^�����W��
	rootparam.DescriptorTable.NumDescriptorRanges = 1;
	//�S�ẴV�F�[�_���猩����
	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	/*���_���C�A�E�g�̐ݒ�*/
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	/*�}�`�̌`����O�p�`�ɐݒ�*/
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	/*���̑��̐ݒ�*/
	//�`��Ώۂ�1��
	gpipeline.NumRenderTargets = 1;
	//0~255�w���RGBA
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	//1�s�N�Z���ɂ�1��T���v�����O
	gpipeline.SampleDesc.Count = 1;

	/*���[�g�V�O�l�`���̐���*/
	//ID3D12RootSignature* rootsignature;

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparams;             //���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootparams); //���[�g�p�����[�^��

	//D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;
	/*�萔�o�b�t�@�r���[�i�擪�j*/
	gpuDescHandle = basicDescHeap->GetGPUDescriptorHandleForHeapStart();
	//DirectXImportant::DirectXImportant::cmdList->SetGraphicsRootDescriptorTable(0, gpuDescHandle);

	/*D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle2;*/
	//DirectXImportant::DirectXImportant::cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                //���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                //�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                //���J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK; //�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;                   //��Ԃ��Ȃ��i�j�A���X�g�l�C�o�[�j
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                                //�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;                                             //�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;          //�s�N�Z���V�F�[�_����̂݉�

	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ID3D10Blob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);
	result = DirectXImportant::dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootsignature));
	rootSigBlob->Release();


	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = rootsignature.Get();

	/*���[�g�V�O�l�`���́A�e�N�X�`����萔�o�b�t�@�Ȃ�
	�V�F�[�_�ɓn�����\�[�X�̏�񂦂��܂Ƃ߂��I�u�W�F�N�g�B

	�����_�ł͊Y�����郊�\�[�X�͎g��Ȃ��̂ŁA�f�t�H���g�ݒ�ō쐬����B*/

	/*�p�C�v���C���X�e�[�g�̐���*/
	//ID3D12PipelineState* pipelinestate = nullptr;
	result = DirectXImportant::dev->CreateGraphicsPipelineState(
		&gpipeline,
		IID_PPV_ARGS(&pipelinestate));
}

void Shape::DrawCube()
{
	HRESULT result = S_OK;

	matScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.z));        //Z����]
	matRot *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.x));        //X����]
	matRot *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.y));        //Y����]
	matTrans = DirectX::XMMatrixTranslation(position.x, position.y, position.z); //���s�ړ��s��Čv�Z

	matWorld = DirectX::XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	//XMFLOAT3 Rotation = { rotation.x,rotation.y,rotation.z };

	if (Input::isKey(DIK_1)) {
		matWorld *= Camera::BillboardMatrix();
	}
	else if (Input::isKey(DIK_2)) {
		matWorld *= Camera::BillboardYMatrix();
	}

	//constMap->mat = matWorld * Camera::FollowCamera(50, Rotation, matRot) * Camera::PerspectiveMatrix();
	constMap->mat = matWorld * Camera::ViewMatrix() * Camera::PerspectiveMatrix();

	Vertex* vertMap = nullptr;
	unsigned short* indexMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	for (int i = 0; i < CubeVertices.size(); i++) {
		vertMap[i] = CubeVertices[i];
	}
	vertBuff->Unmap(0, nullptr);

	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	for (int i = 0; i < CubeIndices.size(); i++) {
		indexMap[i] = CubeIndices[i];
	}
	indexBuff->Unmap(0, nullptr);

	DirectXImportant::cmdList->SetPipelineState(pipelinestate.Get());
	DirectXImportant::cmdList->SetGraphicsRootSignature(rootsignature.Get());

	/*�萔�o�b�t�@�̃f�X�N���v�^�q�[�v���Z�b�g����R�}���h*/
	//�f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { basicDescHeap.Get() };
	DirectXImportant::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�q�[�v�̐擪���萔�o�b�t�@
	DirectXImportant::cmdList->SetGraphicsRootDescriptorTable(
		0, basicDescHeap->GetGPUDescriptorHandleForHeapStart());

	//DirectXImportant::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	DirectXImportant::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/*���_�o�b�t�@�̐ݒ�R�}���h*/
	DirectXImportant::cmdList->IASetVertexBuffers(0, 1, &vbView);
	/*�C���f�b�N�X�o�b�t�@�̐ݒ�R�}���h*/
	DirectXImportant::cmdList->IASetIndexBuffer(&ibView);

	/*�萔�o�b�t�@�r���[�i�擪�j*/
	gpuDescHandle = basicDescHeap->GetGPUDescriptorHandleForHeapStart();

	DirectXImportant::cmdList->SetGraphicsRootDescriptorTable(0, gpuDescHandle);

	/*�V�F�[�_���\�[�X�r���[�i�萔�o�b�t�@�̎��j*/
	gpuDescHandle.ptr += DirectXImportant::dev->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DirectXImportant::cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);

	/*�`��R�}���h*/
	//���_�̐��A�C���X�^���X���A���_�f�[�^�̃I�t�Z�b�g�A�C���X�^���X�̃I�t�Z�b�g
	//DirectXImportant::cmdList->DrawIndexedInstanced(CubeIndices.size(), 1, 0, 0, 0);
	DirectXImportant::cmdList->DrawIndexedInstanced(3, 1, 0, 0, 0);
}

void Shape::CreateSphere(float r)
{
	HRESULT result = S_OK;

	const int m_iUMax = 24;
	const int m_iVMax = 12;
	const int m_iVertexNum = m_iUMax * (m_iVMax + 1);
	const int m_iIndexNum = 2 * m_iVMax * (m_iUMax + 1);

	for (int v = 0; v <= m_iVMax; v++)
	{
		for (int u = 0; u < m_iUMax; u++)
		{
			double dTheta = DirectX::XMConvertToRadians(180.0f * v / m_iVMax);
			double dPhi = DirectX::XMConvertToRadians(360.0f * u / m_iUMax);
			float fX = static_cast<FLOAT>(sin(dTheta) * cos(dPhi)) * r;
			float fY = static_cast<FLOAT>(cos(dTheta)) * r;
			float fZ = static_cast<FLOAT>(sin(dTheta) * sin(dPhi)) * r;
			SphereVertices.push_back({ {fX, fY, fZ}, {}, {} });
		}
	}

	/*�C���f�b�N�X�f�[�^*/
	int i = 0;
	for (int v = 0; v < m_iVMax; v++)
	{
		for (int u = 0; u <= m_iUMax; u++)
		{
			if (u == m_iUMax)
			{
				SphereIndices[i] = v * m_iUMax;
				i++;
				SphereIndices[i] = (v + 1) * m_iUMax;
				i++;
			}
			else
			{
				SphereIndices[i] = (v * m_iUMax) + u;
				i++;
				SphereIndices[i] = SphereIndices[i - 1] + m_iUMax;
				i++;
			}
		}
	}

	/*WIC�e�N�X�`���̃��[�h*/
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};

	result = LoadFromWICFile(
		L"Resources/Slime2.png",
		DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const DirectX::Image* img = scratchImg.GetImage(0, 0, 0); //���f�[�^���o

	/*�e�N�X�`���o�b�t�@�̐���*/
	D3D12_HEAP_PROPERTIES texHeapProp{};                       //�e�N�X�`���q�[�v�ݒ�
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//D3D12_RESOURCE_DESC texresDesc{};                          //���\�[�X�ݒ�
	//texresDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; //2D�e�N�X�`���p
	//texresDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;        //RGBA�t�H�[�}�b�g
	//texresDesc.Width = texWidth;
	//texresDesc.Height = texWidth;
	//texresDesc.DepthOrArraySize = 1;
	//texresDesc.MipLevels = 1;
	//texresDesc.SampleDesc.Count = 1;

	D3D12_RESOURCE_DESC texresDesc{};
	texresDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	texresDesc.Format = metadata.format;
	texresDesc.Width = metadata.width;
	texresDesc.Height = (UINT)metadata.height;
	texresDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texresDesc.MipLevels = (UINT16)metadata.mipLevels;
	texresDesc.SampleDesc.Count = 1;

	ID3D12Resource* texbuff = nullptr;
	result = DirectXImportant::dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texbuff));

	////�e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
	//result = texbuff->WriteToSubresource(
	//	0,
	//	nullptr,                        //�S�̈�փR�s�[
	//	texturedata,                    //���f�[�^�A�h���X
	//	sizeof(XMFLOAT4) * texWidth,    //1���C���T�C�Y
	//	sizeof(XMFLOAT4) * texDataCount //�S�T�C�Y
	//);

	//�e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
	result = texbuff->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch
	);

	//delete[] texturedata;               //���f�[�^���

	//�@���̌v�Z
	for (int i = 0; i < SphereIndices.size() / 3; i++)
	{
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = SphereIndices[i * 3 + 0];
		unsigned short index1 = SphereIndices[i * 3 + 1];
		unsigned short index2 = SphereIndices[i * 3 + 2];

		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		DirectX::XMVECTOR p0 = XMLoadFloat3(&SphereVertices[index0].pos);
		DirectX::XMVECTOR p1 = XMLoadFloat3(&SphereVertices[index1].pos);
		DirectX::XMVECTOR p2 = XMLoadFloat3(&SphereVertices[index2].pos);

		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
		DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(p1, p0);
		DirectX::XMVECTOR v2 = DirectX::XMVectorSubtract(p2, p0);

		//�O�ς͗������琂���ȃx�N�g��
		DirectX::XMVECTOR normal = DirectX::XMVector3Cross(v1, v2);

		//���K��
		normal = DirectX::XMVector3Normalize(normal);

		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&SphereVertices[index0].normal, normal);
		XMStoreFloat3(&SphereVertices[index1].normal, normal);
		XMStoreFloat3(&SphereVertices[index2].normal, normal);
	}

	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * SphereVertices.size());
	//UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * (vertices.size()));

	/*���_�o�b�t�@�̊m��*/
	/*GPU���̃��������m�ۂ��āA���_�f�[�^�p�̗̈��ݒ�B
	���_�o�b�t�@�pGPU���\�[�X�̐���*/

	//���_�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapprop{};

	//GPU�ւ̓]���p
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	//�[�x�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResDesc{};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResDesc.Width = WINDOW_WIDTH;                            //�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResDesc.Height = WINDOW_HEIGHT;                          //�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResDesc.DepthOrArraySize = 1;
	depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;                  //�[�x�l�t�H�[�}�b�g
	depthResDesc.SampleDesc.Count = 1;
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; //�f�v�X�X�e���V��

	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;      //�[�x�l1.0f�i�ő�l�j�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; //�[�x�l�t�H�[�}�b�g

	//���\�[�X�ݒ�
	ID3D12Resource* depthBuffer = nullptr;
	result = DirectXImportant::dev->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, //�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuffer));

	//�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	ID3D12DescriptorHeap* dsvHeap = nullptr;
	result = DirectXImportant::dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	//�[�x�X�e���V���r���[�̍쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DirectXImportant::dev->CreateDepthStencilView(
		depthBuffer,
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	//���_��񂪓��镪�̃T�C�Y
	resdesc.Width = sizeof(SphereVertices);
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//GPU���\�[�X�̐���
	//ID3D12Resource* vertBuff = nullptr;
	result = DirectXImportant::dev->CreateCommittedResource(
		//�q�[�v�ݒ�
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		//���\�[�X�ݒ�
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	/*�C���f�b�N�X�o�b�t�@�̊m��*/
	//ID3D12Resource* indexBuff = nullptr;
	//�C���f�b�N�X��񂪓��镪�̃T�C�Y
	resdesc.Width = sizeof(SphereIndices);

	//GPU���\�[�X�̐���
	result = DirectXImportant::dev->CreateCommittedResource(
		//�q�[�v�ݒ�
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		//���\�[�X�ݒ�
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	Vertex* vertMap = nullptr;
	unsigned short* indexMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	for (int i = 0; i < SphereVertices.size(); i++) {
		vertMap[i] = SphereVertices[i];
	}
	vertBuff->Unmap(0, nullptr);

	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	for (int i = 0; i < SphereIndices.size(); i++) {
		indexMap[i] = SphereIndices[i];
	}
	indexBuff->Unmap(0, nullptr);

	/*���_�o�b�t�@�r���[�̍쐬*/
	//���_�o�b�t�@���Z�b�g���邽�߂�GPU�R�}���h�u���_�o�b�t�@�r���[���쐬����v

	//���_�o�b�t�@�r���[�̍쐬
	//D3D12_VERTEX_BUFFER_VIEW vbView{};
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = SphereVertices.size() * sizeof(Vertex);
	vbView.StrideInBytes = sizeof(Vertex);
	//vbView.SizeInBytes = vertices.size();
	//vbView.StrideInBytes = sizeof(XMFLOAT3);

	/*�C���f�b�N�X�o�b�t�@�r���[�̍쐬�ƃZ�b�g*/
	//D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = SphereIndices.size() * sizeof(unsigned short);
	//ibView.SizeInBytes = indices.size();

	ID3DBlob* vsBlob = nullptr;
	ID3DBlob* psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicVS.hlsl",
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

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		//�V�F�[�_�t�@�C����
		L"BasicPS.hlsl",
		nullptr,
		//�C���N���[�h�\�ɂ���
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		//�G���g���[�|�C���g���A�V�F�[�_���f���w��
		"PSmain", "ps_5_0",
		//�f�o�b�O�w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);

	/*�V�F�[�_�̃G���[���e��\��*/
	/*�E���_�V�F�[�_�̓ǂݍ���
	�E�s�N�Z���V�F�[�_�̓ǂݍ���
	�̂��Ƃɂ��ꂼ��v2�񁫂��̏�����������B

	�����hlsl�t�@�C���ɕ��@�I��
	��肪�������ꍇ�A
	�u�o�́v�E�B���h�E��
	�G���[���e���\�������B*/

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

	/*���_�V�F�[�_�ɓn�����߂̒��_�f�[�^�𐮂���*/
	/*�O���t�B�b�N�X�p�C�v���C����
	���_1���̃f�[�^�ɉ���
	�������邩������
	���߂邱�Ƃ��o����B

	�����ł͍ŏ�����3D���W������
	�Z�b�g���Ă���B
	����K�v�ɂȂ邲�Ƃɍs��ǉ����Ă����B*/
	//���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};

	/*�O���t�B�b�N�X�p�C�v���C���̊e�X�e�[�W�̐ݒ������\���̂�p�ӂ���B*/
	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	/*���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�*/
	gpipeline.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	gpipeline.VS.BytecodeLength = vsBlob->GetBufferSize();
	gpipeline.PS.pShaderBytecode = psBlob->GetBufferPointer();
	gpipeline.PS.BytecodeLength = psBlob->GetBufferSize();

	/*�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�*/

	//�W���ݒ�
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//�J�����O���Ȃ�
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	//�|���S�����h��Ԃ�
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	//�g���̂�
	/*���C���[�t���[�����`�悪�y���A����������*/
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	//�[�x�N���b�s���O��L����
	gpipeline.RasterizerState.DepthClipEnable = true;

	/*�u�����h�X�e�[�g��ݒ�*/
	/*gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;*/
		//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�i8���邪����1�����g��Ȃ��j

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState.DepthEnable = true;                          //�[�x�e�X�g���s��
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; //�������݋���
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;      //��������΍��i
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;                             //�[�x�l�t�H�[�}�b�g

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

	/*�萔�o�b�t�@�̏�����*/
	//�萔�o�b�t�@�p�f�[�^�\����
	//struct ConstBufferData {
	//	//�F�iRGBA�j
	//	XMFLOAT4 color;
	//	XMMATRIX mat;   //3D�ϊ��s��
	//};

	/*�o�b�t�@���m�ۂ���̂ŁA
	�V���ɐ�p�̃f�X�N���v�^�q�[�v���쐬����B*/
	//ID3D12DescriptorHeap* basicDescHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};                   //�ݒ�\����
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //�V�F�[�_�[���猩����
	descHeapDesc.NumDescriptors = 2;                                //�o�b�t�@��2��

	result = DirectXImportant::dev->CreateDescriptorHeap(
		&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));               //����

	//�f�X�N���v�^�q�[�v�̐擪�n���h�����擾���Ă���
	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle =
		basicDescHeap->GetCPUDescriptorHandleForHeapStart();

	/*�萔�o�b�t�@�̍쐬*/
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbheapprop{};
	//GPU�ւ̓]���p
	cbheapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbresdesc{};
	cbresdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//256�o�C�g�A���C�������g
	cbresdesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbresdesc.Height = 1;
	cbresdesc.DepthOrArraySize = 1;
	cbresdesc.MipLevels = 1;
	cbresdesc.SampleDesc.Count = 1;
	cbresdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//GPU���\�[�X�̐���
	//ID3D12Resource* constBuff = nullptr;
	result = DirectXImportant::dev->CreateCommittedResource(
		//�q�[�v�ݒ�
		&cbheapprop,
		D3D12_HEAP_FLAG_NONE,
		//���\�[�X�ݒ�
		&cbresdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));


	/*�萔�o�b�t�@�Ƀf�[�^��]������*/
	//ConstBufferData* constMap = nullptr;

	//�}�b�s���O
	result = constBuff->Map(0, nullptr, (void**)&constMap);

	//RGBA
	constMap->color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//���s���e�ϊ�
	/*constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, WINDOW_WIDTH,
		WINDOW_HEIGHT, 0.0f,
		0.0f, 1.0f
	);*/

	//���[���h�ϊ��s��
	matWorld = DirectX::XMMatrixIdentity();

	/*�X�P�[�����O�i�g�k�j*/
	//XMMATRIX matScale;
	matScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	/*��]*/
	//XMMATRIX matRot; //��]�s��
	matRot = DirectX::XMMatrixIdentity();
	matRot *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.z)); //Z������45�x��]
	matRot *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.x)); //X������45�x��]
	matRot *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.y)); //Y������45�x��]

	/*���s�ړ�*/
	//XMMATRIX matTrans;
	matTrans = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	//->mat = matWorld * Camera::ViewMatrix() * Camera::PerspectiveMatrix();

	//�}�b�s���O����
	constBuff->Unmap(0, nullptr);

	/*�萔�o�b�t�@�r���[�̍쐬*/
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constBuff->GetDesc().Width;

	DirectXImportant::dev->CreateConstantBufferView(
		&cbvDesc, basicHeapHandle);
	/*�����_�o�b�t�@�r���[�ƈႢ�ACreate����K�v������B��*/

	//������������A�h���X�����炷
	basicHeapHandle.ptr +=
		DirectXImportant::dev->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		);

	/*�V�F�[�_�[���\�[�X�r���[�ݒ�*/
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};             //�ݒ�\����
	//srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;       //RGBA
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	/*�q�[�v��2�ԖڂɃV�F�[�_�[���\�[�X�r���[�쐬*/
	DirectXImportant::dev->CreateShaderResourceView(
		texbuff,  //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		basicHeapHandle
	);

	D3D12_DESCRIPTOR_RANGE descRangeCBV{};                    //�萔�p
	descRangeCBV.NumDescriptors = 1;                          //�萔��1��
	descRangeCBV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV; //��ʂ͒萔
	descRangeCBV.BaseShaderRegister = 0;                      //0�ԃX���b�g����
	descRangeCBV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descRangeSRV{};                    //�e�N�X�`���p
	descRangeSRV.NumDescriptors = 1;                          //�e�N�X�`��1��
	descRangeSRV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //��ʂ̓e�N�X�`��
	descRangeSRV.BaseShaderRegister = 0;                      //0�ԃX���b�g����
	descRangeSRV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootparams[2] = {};
	/*�萔�p*/
	rootparams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; //���
	rootparams[0].DescriptorTable.pDescriptorRanges = &descRangeCBV;
	rootparams[0].DescriptorTable.NumDescriptorRanges = 1;                    //�f�X�N���v�^�����W��
	rootparams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;             //�S�ẴV�F�[�_���猩����
	/*�e�N�X�`���p*/
	rootparams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; //���
	rootparams[1].DescriptorTable.pDescriptorRanges = &descRangeSRV;
	rootparams[1].DescriptorTable.NumDescriptorRanges = 1;                    //�f�X�N���v�^�����W��
	rootparams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;             //�S�ẴV�F�[�_���猩����

	/*�f�X�N���v�^�e�[�u���̏�����*/
	/*�萔�o�b�t�@�̏���CPU/GPU�ԂŐ�������ׂ̐ݒ���s���B*/

	/*�f�X�N���v�^�����W�̐ݒ�*/
	D3D12_DESCRIPTOR_RANGE descTblRange{};
	//�萔��1��
	descTblRange.NumDescriptors = 1;
	//��ʂ͒萔
	descTblRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	//0�ԃX���b�g����
	descTblRange.BaseShaderRegister = 0;
	//�W��
	descTblRange.OffsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	/*���[�g�p�����[�^�̐ݒ�*/
	D3D12_ROOT_PARAMETER rootparam = {};
	//���
	rootparam.ParameterType =
		D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//�f�X�N���v�^�����W
	rootparam.DescriptorTable.pDescriptorRanges = &descTblRange;
	//�f�X�N���v�^�����W��
	rootparam.DescriptorTable.NumDescriptorRanges = 1;
	//�S�ẴV�F�[�_���猩����
	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	/*���_���C�A�E�g�̐ݒ�*/
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	/*�}�`�̌`����O�p�`�ɐݒ�*/
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	/*���̑��̐ݒ�*/
	//�`��Ώۂ�1��
	gpipeline.NumRenderTargets = 1;
	//0~255�w���RGBA
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	//1�s�N�Z���ɂ�1��T���v�����O
	gpipeline.SampleDesc.Count = 1;

	/*���[�g�V�O�l�`���̐���*/
	//ID3D12RootSignature* rootsignature;

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparams;             //���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootparams); //���[�g�p�����[�^��

	//D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;
	/*�萔�o�b�t�@�r���[�i�擪�j*/
	gpuDescHandle = basicDescHeap->GetGPUDescriptorHandleForHeapStart();
	//DirectXImportant::DirectXImportant::cmdList->SetGraphicsRootDescriptorTable(0, gpuDescHandle);

	/*D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle2;*/
	//DirectXImportant::DirectXImportant::cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                //���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                //�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                //���J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK; //�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;                   //��Ԃ��Ȃ��i�j�A���X�g�l�C�o�[�j
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                                //�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;                                             //�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;          //�s�N�Z���V�F�[�_����̂݉�

	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ID3D10Blob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);
	result = DirectXImportant::dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootsignature));
	rootSigBlob->Release();


	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = rootsignature.Get();

	/*���[�g�V�O�l�`���́A�e�N�X�`����萔�o�b�t�@�Ȃ�
	�V�F�[�_�ɓn�����\�[�X�̏�񂦂��܂Ƃ߂��I�u�W�F�N�g�B

	�����_�ł͊Y�����郊�\�[�X�͎g��Ȃ��̂ŁA�f�t�H���g�ݒ�ō쐬����B*/

	/*�p�C�v���C���X�e�[�g�̐���*/
	//ID3D12PipelineState* pipelinestate = nullptr;
	result = DirectXImportant::dev->CreateGraphicsPipelineState(
		&gpipeline,
		IID_PPV_ARGS(&pipelinestate));
}

void Shape::DrawSphere()
{
	HRESULT result = S_OK;

	matScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.z));        //Z����]
	matRot *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.x));        //X����]
	matRot *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.y));        //Y����]
	matTrans = DirectX::XMMatrixTranslation(position.x, position.y, position.z); //���s�ړ��s��Čv�Z

	matWorld = DirectX::XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	constMap->mat = matWorld * Camera::ViewMatrix() * Camera::PerspectiveMatrix();

	Vertex* vertMap = nullptr;
	unsigned short* indexMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	for (int i = 0; i < SphereVertices.size(); i++) {
		vertMap[i] = SphereVertices[i];
	}
	vertBuff->Unmap(0, nullptr);

	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	for (int i = 0; i < SphereIndices.size(); i++) {
		indexMap[i] = SphereIndices[i];
	}
	indexBuff->Unmap(0, nullptr);

	DirectXImportant::cmdList->SetPipelineState(pipelinestate.Get());
	DirectXImportant::cmdList->SetGraphicsRootSignature(rootsignature.Get());

	/*�萔�o�b�t�@�̃f�X�N���v�^�q�[�v���Z�b�g����R�}���h*/
	//�f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { basicDescHeap.Get() };
	DirectXImportant::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�q�[�v�̐擪���萔�o�b�t�@
	DirectXImportant::cmdList->SetGraphicsRootDescriptorTable(
		0, basicDescHeap->GetGPUDescriptorHandleForHeapStart());

	DirectXImportant::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//DirectXImportant::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	/*���_�o�b�t�@�̐ݒ�R�}���h*/
	DirectXImportant::cmdList->IASetVertexBuffers(0, 1, &vbView);
	/*�C���f�b�N�X�o�b�t�@�̐ݒ�R�}���h*/
	DirectXImportant::cmdList->IASetIndexBuffer(&ibView);

	/*�萔�o�b�t�@�r���[�i�擪�j*/
	gpuDescHandle = basicDescHeap->GetGPUDescriptorHandleForHeapStart();

	DirectXImportant::cmdList->SetGraphicsRootDescriptorTable(0, gpuDescHandle);

	/*�V�F�[�_���\�[�X�r���[�i�萔�o�b�t�@�̎��j*/
	gpuDescHandle.ptr += DirectXImportant::dev->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DirectXImportant::cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);

	/*�`��R�}���h*/
	//���_�̐��A�C���X�^���X���A���_�f�[�^�̃I�t�Z�b�g�A�C���X�^���X�̃I�t�Z�b�g
	DirectXImportant::cmdList->DrawIndexedInstanced(SphereIndices.size(), 1, 0, 0, 0);
}

void Shape::CreateGeometry(const wchar_t* filename)
{
	HRESULT result;

	/*PointVertices.push_back({ {
			0.0f,
			0.0f,
			0.0f
		} });*/

	for (int i = 0; i < MAX_VERTEX_COUNT; i++) {
		const float md_width = 10.0f;
		PointVertices.push_back({ {
				(float)rand() / RAND_MAX * md_width - md_width / 2.0f,
				(float)rand() / RAND_MAX * md_width - md_width / 2.0f,
				(float)rand() / RAND_MAX * md_width - md_width / 2.0f
			},
			{0},
			{}
			});
	}

	/*for (int i = 0; i < 10; i++) {
		const float md_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;

		const float md_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;

		const float md_acc = 0.001f;
		XMFLOAT3 acc{};
		acc.y = (float)rand() / RAND_MAX * md_acc;
		Add(600, pos, vel, acc);
	}*/

	particles.resize(10);

	/*WIC�e�N�X�`���̃��[�h*/
	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename,
		DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const DirectX::Image* img = scratchImg.GetImage(0, 0, 0); //���f�[�^���o

	/*�e�N�X�`���o�b�t�@�̐���*/
	D3D12_HEAP_PROPERTIES texHeapProp{};                       //�e�N�X�`���q�[�v�ݒ�
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	//D3D12_RESOURCE_DESC texresDesc{};                          //���\�[�X�ݒ�
	//texresDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; //2D�e�N�X�`���p
	//texresDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;        //RGBA�t�H�[�}�b�g
	//texresDesc.Width = metadata.width;
	//texresDesc.Height = metadata.width;
	//texresDesc.DepthOrArraySize = 1;
	//texresDesc.MipLevels = 1;
	//texresDesc.SampleDesc.Count = 1;

	D3D12_RESOURCE_DESC texresDesc{};
	texresDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	texresDesc.Format = metadata.format;
	texresDesc.Width = metadata.width;
	texresDesc.Height = (UINT)metadata.height;
	texresDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texresDesc.MipLevels = (UINT16)metadata.mipLevels;
	texresDesc.SampleDesc.Count = 1;

	ID3D12Resource* texbuff = nullptr;
	result = DirectXImportant::dev->CreateCommittedResource(
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texbuff));

	////�e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
	//result = texbuff->WriteToSubresource(
	//	0,
	//	nullptr,                        //�S�̈�փR�s�[
	//	texturedata,                    //���f�[�^�A�h���X
	//	sizeof(XMFLOAT4) * metadata.width,    //1���C���T�C�Y
	//	sizeof(XMFLOAT4) * texDataCount //�S�T�C�Y
	//);

	//�e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
	result = texbuff->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch
	);

	//delete[] texturedata;               //���f�[�^���

	//UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * CubeVertices.size());
	//UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * (vertices.size()));

	/*���_�o�b�t�@�̊m��*/
	/*GPU���̃��������m�ۂ��āA���_�f�[�^�p�̗̈��ݒ�B
	���_�o�b�t�@�pGPU���\�[�X�̐���*/

	//���_�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapprop{};

	//GPU�ւ̓]���p
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	//�[�x�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResDesc{};
	depthResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResDesc.Width = WINDOW_WIDTH;                            //�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResDesc.Height = WINDOW_HEIGHT;                          //�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResDesc.DepthOrArraySize = 1;
	depthResDesc.Format = DXGI_FORMAT_D32_FLOAT;                  //�[�x�l�t�H�[�}�b�g
	depthResDesc.SampleDesc.Count = 1;
	depthResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; //�f�v�X�X�e���V��

	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;      //�[�x�l1.0f�i�ő�l�j�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT; //�[�x�l�t�H�[�}�b�g

	//���\�[�X�ݒ�
	ID3D12Resource* depthBuffer = nullptr;
	result = DirectXImportant::dev->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, //�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuffer));

	//�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	ID3D12DescriptorHeap* dsvHeap = nullptr;
	result = DirectXImportant::dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	//�[�x�X�e���V���r���[�̍쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DirectXImportant::dev->CreateDepthStencilView(
		depthBuffer,
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	//���_��񂪓��镪�̃T�C�Y
	resdesc.Width = sizeof(PointVertices);
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//GPU���\�[�X�̐���
	result = DirectXImportant::dev->CreateCommittedResource(
		//�q�[�v�ݒ�
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		//���\�[�X�ݒ�
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	GSVertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	for (int i = 0; i < PointVertices.size(); i++) {
		vertMap[i] = PointVertices[i];
	}
	vertBuff->Unmap(0, nullptr);

	/*���_�o�b�t�@�r���[�̍쐬*/
	//���_�o�b�t�@���Z�b�g���邽�߂�GPU�R�}���h�u���_�o�b�t�@�r���[���쐬����
	UINT sizeVB = static_cast<UINT>(sizeof(GSVertex) * PointVertices.size());

	//���_�o�b�t�@�r���[�̍쐬
	//D3D12_VERTEX_BUFFER_VIEW vbView{};
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(GSVertex);
	//vbView.SizeInBytes = vertices.size();
	//vbView.StrideInBytes = sizeof(XMFLOAT3);

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> gsBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicVS.hlsl",
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

	result = D3DCompileFromFile(
		//�V�F�[�_�t�@�C����
		L"BasicGeometryShader.hlsl",
		nullptr,
		//�C���N���[�h�\�ɂ���
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		//�G���g���[�|�C���g���A�V�F�[�_���f���w��
		"GSmain", "gs_5_0",
		//�f�o�b�O�w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&gsBlob, &errorBlob);

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		//�V�F�[�_�t�@�C����
		L"BasicPS.hlsl",
		nullptr,
		//�C���N���[�h�\�ɂ���
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		//�G���g���[�|�C���g���A�V�F�[�_���f���w��
		"PSmain", "ps_5_0",
		//�f�o�b�O�w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);

	/*�V�F�[�_�̃G���[���e��\��*/
	/*�E���_�V�F�[�_�̓ǂݍ���
	�E�s�N�Z���V�F�[�_�̓ǂݍ���
	�̂��Ƃɂ��ꂼ��v2�񁫂��̏�����������B

	�����hlsl�t�@�C���ɕ��@�I��
	��肪�������ꍇ�A
	�u�o�́v�E�B���h�E��
	�G���[���e���\�������B*/

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

	/*���_�V�F�[�_�ɓn�����߂̒��_�f�[�^�𐮂���*/
	/*�O���t�B�b�N�X�p�C�v���C����
	���_1���̃f�[�^�ɉ���
	�������邩������
	���߂邱�Ƃ��o����B

	�����ł͍ŏ�����3D���W������
	�Z�b�g���Ă���B
	����K�v�ɂȂ邲�Ƃɍs��ǉ����Ă����B*/
	//���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	};

	/*�O���t�B�b�N�X�p�C�v���C���̊e�X�e�[�W�̐ݒ������\���̂�p�ӂ���B*/
	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	/*���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�*/
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	/*�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�*/

	//�W���ݒ�
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//�J�����O���Ȃ�
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//�|���S�����h��Ԃ�
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	//�g���̂�
	/*���C���[�t���[�����`�悪�y���A����������*/
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	//�[�x�N���b�s���O��L����
	gpipeline.RasterizerState.DepthClipEnable = true;

	/*�u�����h�X�e�[�g��ݒ�*/
	/*gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;*/
		//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�i8���邪����1�����g��Ȃ��j

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState.DepthEnable = true;                          //�[�x�e�X�g���s��
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; //�������݋���
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;      //��������΍��i
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;                             //�[�x�l�t�H�[�}�b�g

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
	//���Z
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//�\�[�X�̒l��100%�g��
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//�f�X�g�̒l��0%�g��
	blenddesc.DestBlend = D3D12_BLEND_ONE;

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
	////���Z
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	////�\�[�X�̃A���t�@�l
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	////1.0f - �\�[�X�̃A���t�@�l
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//�u�����h�X�e�[�g�ɐݒ肷��
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	/*�萔�o�b�t�@�̏�����*/
	//�萔�o�b�t�@�p�f�[�^�\����
	//struct ConstBufferData {
	//	//�F�iRGBA�j
	//	XMFLOAT4 color;
	//	XMMATRIX mat;   //3D�ϊ��s��
	//};

	/*�o�b�t�@���m�ۂ���̂ŁA
	�V���ɐ�p�̃f�X�N���v�^�q�[�v���쐬����B*/
	//ID3D12DescriptorHeap* basicDescHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};                   //�ݒ�\����
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //�V�F�[�_�[���猩����
	descHeapDesc.NumDescriptors = 2;                                //�o�b�t�@��2��

	result = DirectXImportant::dev->CreateDescriptorHeap(
		&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));               //����

	//�f�X�N���v�^�q�[�v�̐擪�n���h�����擾���Ă���
	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle =
		basicDescHeap->GetCPUDescriptorHandleForHeapStart();

	/*�萔�o�b�t�@�̍쐬*/
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbheapprop{};
	//GPU�ւ̓]���p
	cbheapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbresdesc{};
	cbresdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//256�o�C�g�A���C�������g
	cbresdesc.Width = (sizeof(GSConstBufferData) + 0xff) & ~0xff;
	cbresdesc.Height = 1;
	cbresdesc.DepthOrArraySize = 1;
	cbresdesc.MipLevels = 1;
	cbresdesc.SampleDesc.Count = 1;
	cbresdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//GPU���\�[�X�̐���
	/*ID3D12Resource* constBuff = nullptr;*/
	result = DirectXImportant::dev->CreateCommittedResource(
		//�q�[�v�ݒ�
		&cbheapprop,
		D3D12_HEAP_FLAG_NONE,
		//���\�[�X�ݒ�
		&cbresdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));


	/*�萔�o�b�t�@�Ƀf�[�^��]������*/
	//ConstBufferData* constMap = nullptr;

	//�}�b�s���O
	//result = constBuff->Map(0, nullptr, (void**)&GSconstMap);

	//�}�b�s���O����
	//constBuff->Unmap(0, nullptr);

	/*�萔�o�b�t�@�r���[�̍쐬*/
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constBuff->GetDesc().Width;

	DirectXImportant::dev->CreateConstantBufferView(
		&cbvDesc, basicHeapHandle);
	/*�����_�o�b�t�@�r���[�ƈႢ�ACreate����K�v������B��*/

	//������������A�h���X�����炷
	basicHeapHandle.ptr +=
		DirectXImportant::dev->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
		);

	/*�V�F�[�_�[���\�[�X�r���[�ݒ�*/
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};             //�ݒ�\����
	//srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;       //RGBA
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	/*�q�[�v��2�ԖڂɃV�F�[�_�[���\�[�X�r���[�쐬*/
	DirectXImportant::dev->CreateShaderResourceView(
		texbuff,  //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		basicHeapHandle
	);

	D3D12_DESCRIPTOR_RANGE descRangeCBV{};                    //�萔�p
	descRangeCBV.NumDescriptors = 1;                          //�萔��1��
	descRangeCBV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV; //��ʂ͒萔
	descRangeCBV.BaseShaderRegister = 0;                      //0�ԃX���b�g����
	descRangeCBV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descRangeSRV{};                    //�e�N�X�`���p
	descRangeSRV.NumDescriptors = 1;                          //�e�N�X�`��1��
	descRangeSRV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //��ʂ̓e�N�X�`��
	descRangeSRV.BaseShaderRegister = 0;                      //0�ԃX���b�g����
	descRangeSRV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootparams[2] = {};
	/*�萔�p*/
	rootparams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; //���
	rootparams[0].DescriptorTable.pDescriptorRanges = &descRangeCBV;
	rootparams[0].DescriptorTable.NumDescriptorRanges = 1;                    //�f�X�N���v�^�����W��
	rootparams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;             //�S�ẴV�F�[�_���猩����
	/*�e�N�X�`���p*/
	rootparams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; //���
	rootparams[1].DescriptorTable.pDescriptorRanges = &descRangeSRV;
	rootparams[1].DescriptorTable.NumDescriptorRanges = 1;                    //�f�X�N���v�^�����W��
	rootparams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;             //�S�ẴV�F�[�_���猩����

	/*�f�X�N���v�^�e�[�u���̏�����*/
	/*�萔�o�b�t�@�̏���CPU/GPU�ԂŐ�������ׂ̐ݒ���s���B*/

	/*�f�X�N���v�^�����W�̐ݒ�*/
	D3D12_DESCRIPTOR_RANGE descTblRange{};
	//�萔��1��
	descTblRange.NumDescriptors = 1;
	//��ʂ͒萔
	descTblRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	//0�ԃX���b�g����
	descTblRange.BaseShaderRegister = 0;
	//�W��
	descTblRange.OffsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	/*���[�g�p�����[�^�̐ݒ�*/
	D3D12_ROOT_PARAMETER rootparam = {};
	//���
	rootparam.ParameterType =
		D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//�f�X�N���v�^�����W
	rootparam.DescriptorTable.pDescriptorRanges = &descTblRange;
	//�f�X�N���v�^�����W��
	rootparam.DescriptorTable.NumDescriptorRanges = 1;
	//�S�ẴV�F�[�_���猩����
	rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	/*���_���C�A�E�g�̐ݒ�*/
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	/*�}�`�̌`���Point�ɐݒ�*/
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	/*���̑��̐ݒ�*/
	//�`��Ώۂ�1��
	gpipeline.NumRenderTargets = 1;
	//0~255�w���RGBA
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	//1�s�N�Z���ɂ�1��T���v�����O
	gpipeline.SampleDesc.Count = 1;

	/*���[�g�V�O�l�`���̐���*/
	//ID3D12RootSignature* rootsignature;

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparams;             //���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootparams); //���[�g�p�����[�^��

	//D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;
	/*�萔�o�b�t�@�r���[�i�擪�j*/
	gpuDescHandle = basicDescHeap->GetGPUDescriptorHandleForHeapStart();
	//DirectXImportant::DirectXImportant::cmdList->SetGraphicsRootDescriptorTable(0, gpuDescHandle);

	/*D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle2;*/
	//DirectXImportant::DirectXImportant::cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                //���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                //�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                //���J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK; //�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;                   //��Ԃ��Ȃ��i�j�A���X�g�l�C�o�[�j
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                                //�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;                                             //�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;          //�s�N�Z���V�F�[�_����̂݉�

	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ID3D10Blob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);
	result = DirectXImportant::dev->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootsignature));
	rootSigBlob->Release();


	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = rootsignature.Get();

	/*���[�g�V�O�l�`���́A�e�N�X�`����萔�o�b�t�@�Ȃ�
	�V�F�[�_�ɓn�����\�[�X�̏�񂦂��܂Ƃ߂��I�u�W�F�N�g*/

	/*�p�C�v���C���X�e�[�g�̐���*/
	//ID3D12PipelineState* pipelinestate = nullptr;
	result = DirectXImportant::dev->CreateGraphicsPipelineState(
		&gpipeline,
		IID_PPV_ARGS(&pipelinestate));
}

void Shape::DrawGeometry()
{
	HRESULT result = S_OK;

	matScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.z));        //Z����]
	matRot *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.x));        //X����]
	matRot *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.y));        //Y����]
	matTrans = DirectX::XMMatrixTranslation(position.x, position.y, position.z); //���s�ړ��s��Čv�Z

	matWorld = DirectX::XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	if (Input::isKey(DIK_1)) {
		matWorld *= Camera::BillboardMatrix();
	}
	else if (Input::isKey(DIK_2)) {
		matWorld *= Camera::BillboardYMatrix();
	}

	//�������s�����p�[�e�B�N�����폜
	particles.remove_if(
		[](Particle& x) {
			return x.frame >= x.num_frame;
		}
	);

	//�p�[�e�B�N���X�V
	for (std::forward_list<Particle>::iterator it = particles.begin();
		it != particles.end();
		it++) {
		it->frame++;
		it->vel = it->vel + it->accel;
		it->pos = it->pos + it->vel;

		float f = (float)it->num_frame / it->frame;
		it->scale = (it->e_scale - it->s_scale) / f;
		it->scale += it->s_scale;
	}

	//���_�o�b�t�@�փf�[�^�]��
	GSVertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		for (std::forward_list<Particle>::iterator it = particles.begin();
			it != particles.end();
			it++) {
			vertMap->pos = it->pos;
			vertMap->scale = it->scale;
			vertMap++;
		}
		vertBuff->Unmap(0, nullptr);
	}

	//�萔�o�b�t�@�փf�[�^�]��
	GSConstBufferData* GSconstMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&GSconstMap);
	DirectX::XMMATRIX camera = Camera::ViewMatrix();
	DirectX::XMMATRIX shisuidai = Camera::PerspectiveMatrix();
	/*GSconstMap->mat = Camera::ViewMatrix() * Camera::PerspectiveMatrix();*/
	GSconstMap->mat = camera * shisuidai;
	GSconstMap->matBillBoard = Camera::BillboardMatrix();
	constBuff->Unmap(0, nullptr);

	/*GSVertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	for (int i = 0; i < PointVertices.size(); i++) {
		vertMap[i] = PointVertices[i];
	}
	vertBuff->Unmap(0, nullptr);*/

	DirectXImportant::cmdList->SetPipelineState(pipelinestate.Get());
	DirectXImportant::cmdList->SetGraphicsRootSignature(rootsignature.Get());

	/*�萔�o�b�t�@�̃f�X�N���v�^�q�[�v���Z�b�g����R�}���h*/
	//�f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { basicDescHeap.Get() };
	DirectXImportant::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�q�[�v�̐擪���萔�o�b�t�@
	DirectXImportant::cmdList->SetGraphicsRootDescriptorTable(
		0, basicDescHeap->GetGPUDescriptorHandleForHeapStart());

	//DirectXImportant::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	DirectXImportant::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	/*���_�o�b�t�@�̐ݒ�R�}���h*/
	DirectXImportant::cmdList->IASetVertexBuffers(0, 1, &vbView);
	/*�C���f�b�N�X�o�b�t�@�̐ݒ�R�}���h*/
	//DirectXImportant::cmdList->IASetIndexBuffer(&ibView);

	/*�萔�o�b�t�@�r���[�i�擪�j*/
	gpuDescHandle = basicDescHeap->GetGPUDescriptorHandleForHeapStart();

	DirectXImportant::cmdList->SetGraphicsRootDescriptorTable(0, gpuDescHandle);

	/*�V�F�[�_���\�[�X�r���[�i�萔�o�b�t�@�̎��j*/
	gpuDescHandle.ptr += DirectXImportant::dev->GetDescriptorHandleIncrementSize(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DirectXImportant::cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);

	/*�`��R�}���h*/
	//���_�̐��A�C���X�^���X���A���_�f�[�^�̃I�t�Z�b�g�A�C���X�^���X�̃I�t�Z�b�g
	//DirectXImportant::cmdList->DrawInstanced(PointVertices.size(), 1, 0, 0);
	DirectXImportant::cmdList->DrawInstanced((UINT)std::distance(particles.begin(), particles.end()), 1, 0, 0);
}

void Shape::Add(int life, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, DirectX::XMFLOAT3 accel, float start_scale, float end_scale)
{
	particles.emplace_front();
	Particle& p = particles.front();
	p.pos = position;
	p.vel = velocity;
	p.accel = accel;
	p.num_frame = life;
}

Shape* Shape::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Shape* shape = new Shape();
	if (shape == nullptr) {
		return nullptr;
	}

	return shape;
}
