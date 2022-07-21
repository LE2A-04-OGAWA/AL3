//#include "FbxModels.h"
//#include <fstream>
//#include <sstream>
//#include <algorithm>
//#include <cassert>
//
//FbxModels::~FbxModels()
//{
//	for (auto m : meshes) {
//		delete m;
//	}
//	meshes.clear();
//
//	for (auto m : materials) {
//		delete m.second;
//	}
//	materials.clear();
//
//	fbxScene->Destroy();
//}
//
//void FbxModels::StaticInit(ID3D12Device* dev)
//{
//	assert(!FbxModels::device);
//
//	FbxModels::device = dev;
//	FbxMeshes::StaticInit(dev);
//}
//
//FbxModels* FbxModels::CreateFromFbx(const std::string& modelname, bool smoothing)
//{
//	FbxModels* instance = new FbxModels();
//	//std::shared_ptr<Model> instance = std::make_shared<Model>();
//	instance->Init(modelname, smoothing);
//
//	return instance;
//}
//
////Load
//void FbxModels::Init(const std::string& modelname, bool smoothing)
//{
//	//Test-----
//	const std::string filename = modelname + ".fbx";
//	const std::string directoryPath = baseDirectory + modelname + "/";
//	const std::string path = directoryPath + filename;
//
//	FbxManager* fbx_manager = fbxsdk::FbxManager::Create();
//	assert(fbx_manager == nullptr);
//
//	FbxImporter* fbx_importer = FbxImporter::Create(fbx_manager, "");
//	if (fbx_importer == nullptr)
//	{
//		fbx_manager->Destroy();
//		assert(0);
//	}
//
//	fbxScene = FbxScene::Create(fbx_manager, "");
//	if (fbxScene == nullptr)
//	{
//		fbx_importer->Destroy();
//		fbx_manager->Destroy();
//		assert(0);
//	}
//
//	fbx_importer->Initialize(path.c_str());
//
//	fbx_importer->Import(fbxScene);
//
//	FbxGeometryConverter converter(fbx_manager);
//	converter.SplitMeshesPerMaterial(fbxScene, true);
//	converter.Triangulate(fbxScene, true);
//
//	int material_num = fbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();
//	for (int i = 0; i < material_num; i++)
//	{
//		LoadMaterial(directoryPath, filename, fbxScene->GetSrcObject<FbxSurfaceMaterial>(i));
//	}
//
//	int mesh_num = fbxScene->GetSrcObjectCount<FbxMesh>();
//	for (int i = 0; i < mesh_num; i++)
//	{
//		CreateMesh(fbxScene->GetSrcObject<FbxMesh>(i));
//	}
//
//	int texture_num = fbxScene->GetSrcObjectCount<FbxFileTexture>();
//	for (int i = 0; i < texture_num; i++)
//	{
//		FbxFileTexture* texture = fbxScene->GetSrcObject<FbxFileTexture>(i);
//		if (texture)
//		{
//			const char* file_name01 = texture->GetFileName();
//			const char* file_name02 = texture->GetRelativeFileName();
//			int tex = texture->GetSrcObjectCount< FbxSurfaceMaterial>();
//		}
//	}
//
//	fbx_importer->Destroy();
//	//fbx_scene->Destroy();
//	fbx_manager->Destroy();
//
//	for (auto& m : meshes)
//	{
//		if (m->GetMaterial() == nullptr)
//		{
//			if (defaultMaterial == nullptr)
//			{
//				defaultMaterial = FbxMaterial::Create();
//				defaultMaterial->SetName("no material");
//				materials.emplace(defaultMaterial->GetName(), defaultMaterial);
//			}
//			m->SetMaterial(defaultMaterial);
//		}
//	}
//
//	for (auto& m : meshes)
//	{
//		m->CreateBuffers();
//	}
//
//	for (auto& m : materials) {
//		m.second->Update();
//	}
//
//	CreateDescriptorHeap();
//
//	LoadTextures();
//
//	//Test-----
//	const std::string filename = modelname + ".obj";
//	const std::string directoryPath = baseDirectory + modelname + "/";
//
//	// �t�@�C���X�g���[��
//	std::ifstream file;
//	// .obj�t�@�C�����J��
//	file.open(directoryPath + filename);
//	// �t�@�C���I�[�v�����s���`�F�b�N
//	if (file.fail()) {
//		assert(0);
//	}
//
//	name = modelname;
//
//	// ���b�V������
//	Mesh* mesh = new Mesh;
//	int indexCountTex = 0;
//	int indexCountNoTex = 0;
//
//	std::vector<DirectX::XMFLOAT3> positions;	// ���_���W
//	std::vector<DirectX::XMFLOAT3> normals;	// �@���x�N�g��
//	std::vector<DirectX::XMFLOAT2> texcoords;	// �e�N�X�`��UV
//	// 1�s���ǂݍ���
//	std::string line;
//	while (getline(file, line)) {
//
//		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
//		std::istringstream line_stream(line);
//
//		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
//		std::string key;
//		getline(line_stream, key, ' ');
//
//		//�}�e���A��
//		if (key == "mtllib")
//		{
//			// �}�e���A���̃t�@�C�����ǂݍ���
//			std::string filename;
//			line_stream >> filename;
//			// �}�e���A���ǂݍ���
//			LoadMaterial(directoryPath, filename);
//		}
//		// �擪������g�Ȃ�O���[�v�̊J�n
//		if (key == "g") {
//			// �J�����g���b�V���̏�񂪑����Ă���Ȃ�
//			if (mesh->GetName().size() > 0 && mesh->GetVertexCount() > 0) {
//				//���_�@���̕��ςɂ��G�b�W�̕�����
//				if (smoothing) {
//					mesh->CalculateSmoothedVertexNormals();
//				}
//				// �R���e�i�ɓo�^
//				meshes.emplace_back(mesh);
//				// ���̃��b�V������
//				mesh = new Mesh;
//				indexCountTex = 0;
//			}
//
//			// �O���[�v���ǂݍ���
//			std::string groupName;
//			line_stream >> groupName;
//
//			// ���b�V���ɖ��O���Z�b�g
//			mesh->SetName(groupName);
//		}
//		// �擪������v�Ȃ璸�_���W
//		if (key == "v") {
//			// X,Y,Z���W�ǂݍ���
//			DirectX::XMFLOAT3 position{};
//			line_stream >> position.x;
//			line_stream >> position.y;
//			line_stream >> position.z;
//			positions.emplace_back(position);
//
//			//�T�C�Y�̔�r
//			if (position.x > size.x) { size.x = position.x; }
//			if (position.y > size.y) { size.y = position.y; }
//			if (position.z > size.z) { size.z = position.z; }
//		}
//		// �擪������vt�Ȃ�e�N�X�`��
//		if (key == "vt")
//		{
//			// U,V�����ǂݍ���
//			DirectX::XMFLOAT2 texcoord{};
//			line_stream >> texcoord.x;
//			line_stream >> texcoord.y;
//			// V�������]
//			texcoord.y = 1.0f - texcoord.y;
//			// �e�N�X�`�����W�f�[�^�ɒǉ�
//			texcoords.emplace_back(texcoord);
//		}
//		// �擪������vn�Ȃ�@���x�N�g��
//		if (key == "vn") {
//			// X,Y,Z�����ǂݍ���
//			DirectX::XMFLOAT3 normal{};
//			line_stream >> normal.x;
//			line_stream >> normal.y;
//			line_stream >> normal.z;
//			// �@���x�N�g���f�[�^�ɒǉ�
//			normals.emplace_back(normal);
//		}
//		// �擪������usemtl�Ȃ�}�e���A�������蓖�Ă�
//		if (key == "usemtl")
//		{
//			if (mesh->GetMaterial() == nullptr) {
//				// �}�e���A���̖��ǂݍ���
//				std::string materialName;
//				line_stream >> materialName;
//
//				// �}�e���A�����Ō������A�}�e���A�������蓖�Ă�
//				auto itr = materials.find(materialName);
//				if (itr != materials.end()) {
//					mesh->SetMaterial(itr->second);
//				}
//			}
//		}
//		// �擪������f�Ȃ�|���S���i�O�p�`�j
//		if (key == "f")
//		{
//			int faceIndexCount = 0;
//			// ���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
//			std::string index_string;
//			while (getline(line_stream, index_string, ' ')) {
//				// ���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
//				std::istringstream index_stream(index_string);
//				unsigned short indexPosition, indexNormal, indexTexcoord;
//				// ���_�ԍ�
//				index_stream >> indexPosition;
//
//				Material* material = mesh->GetMaterial();
//				index_stream.seekg(1, std::ios_base::cur); // �X���b�V�����΂�
//				// �}�e���A���A�e�N�X�`��������ꍇ
//				if (material && material->textureFilename.size() > 0) {
//					index_stream >> indexTexcoord;
//					index_stream.seekg(1, std::ios_base::cur); // �X���b�V�����΂�
//					index_stream >> indexNormal;
//					// ���_�f�[�^�̒ǉ�
//					Mesh::VertexPosNormalUv vertex{};
//					vertex.pos = positions[indexPosition - 1];
//					vertex.normal = normals[indexNormal - 1];
//					vertex.uv = texcoords[indexTexcoord - 1];
//					mesh->AddVertex(vertex);
//					//�G�b�W�������p�̃f�[�^��ǉ�
//					if (smoothing) {
//						//v�L�[�i���W�f�[�^�j�̔ԍ��ƁA�S�č����������_�̃C���f�b�N�X���Z�b�g�œo�^����
//						mesh->AddSmoothData(indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
//					}
//				}
//				else {
//					char c;
//					index_stream >> c;
//					// �X���b�V��2�A���̏ꍇ�A���_�ԍ��̂�
//					if (c == '/') {
//						// ���_�f�[�^�̒ǉ�
//						Mesh::VertexPosNormalUv vertex{};
//						vertex.pos = positions[indexPosition - 1];
//						vertex.normal = { 0, 0, 1 };
//						vertex.uv = { 0, 0 };
//						mesh->AddVertex(vertex);
//						//�G�b�W�������p�̃f�[�^��ǉ�
//						if (smoothing) {
//							//v�L�[�i���W�f�[�^�j�̔ԍ��ƁA�S�č����������_�̃C���f�b�N�X���Z�b�g�œo�^����
//							mesh->AddSmoothData(indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
//						}
//					}
//					else {
//						index_stream.seekg(-1, std::ios_base::cur); // 1�����߂�
//						index_stream >> indexTexcoord;
//						index_stream.seekg(1, std::ios_base::cur); // �X���b�V�����΂�
//						index_stream >> indexNormal;
//						// ���_�f�[�^�̒ǉ�
//						Mesh::VertexPosNormalUv vertex{};
//						vertex.pos = positions[indexPosition - 1];
//						vertex.normal = normals[indexNormal - 1];
//						vertex.uv = { 0, 0 };
//						mesh->AddVertex(vertex);
//						//�G�b�W�������p�̃f�[�^��ǉ�
//						if (smoothing) {
//							//v�L�[�i���W�f�[�^�j�̔ԍ��ƁA�S�č����������_�̃C���f�b�N�X���Z�b�g�œo�^����
//							mesh->AddSmoothData(indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
//						}
//					}
//				}
//				// �C���f�b�N�X�f�[�^�̒ǉ�
//				if (faceIndexCount >= 3) {
//					// �l�p�`�|���S����4�_�ڂȂ̂ŁA
//					// �l�p�`��0,1,2,3�̓� 2,3,0�ŎO�p�`���\�z����
//					mesh->AddIndex(indexCountTex - 1);
//					mesh->AddIndex(indexCountTex);
//					mesh->AddIndex(indexCountTex - 3);
//				}
//				else
//				{
//					mesh->AddIndex(indexCountTex);
//				}
//				indexCountTex++;
//				faceIndexCount++;
//			}
//		}
//	}
//	file.close();
//
//	//���_�@���̕��ςɂ��G�b�W�̕�����
//	if (smoothing) {
//		mesh->CalculateSmoothedVertexNormals();
//	}
//
//	// �R���e�i�ɓo�^
//	meshes.emplace_back(mesh);
//
//	// ���b�V���̃}�e���A���`�F�b�N
//	for (auto& m : meshes) {
//		// �}�e���A���̊��蓖�Ă��Ȃ�
//		if (m->GetMaterial() == nullptr) {
//			if (defaultMaterial == nullptr) {
//				// �f�t�H���g�}�e���A���𐶐�
//				defaultMaterial = Material::Create();
//				defaultMaterial->name = "no material";
//				materials.emplace(defaultMaterial->name, defaultMaterial);
//			}
//			// �f�t�H���g�}�e���A�����Z�b�g
//			m->SetMaterial(defaultMaterial);
//		}
//	}
//
//	// ���b�V���̃o�b�t�@����
//	for (auto& m : meshes) {
//		m->CreateBuffers();
//	}
//
//	// �}�e���A���̐��l��萔�o�b�t�@�ɔ��f
//	for (auto& m : materials) {
//		m.second->Update();
//	}
//
//	// �f�X�N���v�^�q�[�v����
//	CreateDescriptorHeap();
//
//	// �e�N�X�`���̓ǂݍ���
//	LoadTextures();
//}
//
//void FbxModels::Draw(ID3D12GraphicsCommandList* cmdList)
//{
//	if (descHeap) {
//		ID3D12DescriptorHeap* ppHeaps[] = { descHeap };
//		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
//	}
//
//	for (auto& mesh : meshes) {
//		mesh->Draw(cmdList);
//	}
//}
//
////Load
//void FbxModels::LoadMaterial(const std::string& directoryPath, const std::string& filename, FbxSurfaceMaterial* fbx_material)
//{
//	//Test-----
//	FbxMaterial* l_material = FbxMaterial::Create();
//	FbxMaterial::ConstBufferDataB1 entry_material;
//	enum class MaterialOrder
//	{
//		Ambient,
//		Diffuse,
//		Specular,
//		MaxOrder,
//	};
//
//	FbxDouble3 colors[(int)MaterialOrder::MaxOrder];
//	FbxDouble factors[(int)MaterialOrder::MaxOrder];
//	FbxProperty prop = fbx_material->FindProperty(FbxSurfaceMaterial::sAmbient);
//
//	if (fbx_material->GetClassId().Is(FbxSurfaceLambert::ClassId))
//	{
//		const char* element_check_list[] =
//		{
//			FbxSurfaceMaterial::sAmbient,
//			FbxSurfaceMaterial::sDiffuse,
//		};
//
//		const char* factor_check_list[] =
//		{
//			FbxSurfaceMaterial::sAmbientFactor,
//			FbxSurfaceMaterial::sDiffuseFactor,
//		};
//
//		for (int i = 0; i < 2; i++)
//		{
//			prop = fbx_material->FindProperty(element_check_list[i]);
//			if (prop.IsValid()) { colors[i] = prop.Get<FbxDouble3>(); }
//			else { colors[i] = FbxDouble3(1.0, 1.0, 1.0); }
//
//			prop = fbx_material->FindProperty(factor_check_list[i]);
//			if (prop.IsValid()) { factors[i] = prop.Get<FbxDouble>(); }
//			else { factors[i] = 1.0; }
//		}
//	}
//
//	FbxDouble3 color = colors[(int)MaterialOrder::Ambient];
//	FbxDouble factor = factors[(int)MaterialOrder::Ambient];
//	entry_material.ambient = DirectX::XMFLOAT3((float)color[0], (float)color[1], (float)color[2]);
//
//	color = colors[(int)MaterialOrder::Diffuse];
//	factor = factors[(int)MaterialOrder::Diffuse];
//	entry_material.diffuse = DirectX::XMFLOAT3((float)color[0], (float)color[1], (float)color[2]);
//
//	color = colors[(int)MaterialOrder::Specular];
//	factor = factors[(int)MaterialOrder::Specular];
//	entry_material.specular = DirectX::XMFLOAT3((float)color[0], (float)color[1], (float)color[2]);
//
//	l_material->SetMaterial(entry_material);
//
//	if (l_material) { AddMaterial(l_material); }
//
//	//Test-----
//
//	//�t�@�C���X�g���[��
//	std::ifstream file;
//	//�}�e���A���t�@�C�����J��
//	file.open(directoryPath + filename);
//	//�t�@�C���I�[�v�����s���`�F�b�N
//	if (file.fail()) {
//		assert(0);
//	}
//
//	FbxMaterial* material = nullptr;
//
//	//1�s���ǂݍ���
//	std::string line;
//	while (getline(file, line)) {
//
//		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
//		std::istringstream line_stream(line);
//
//		//���p�X�y�[�X��؂�ōs�̐擪��������擾
//		std::string key;
//		getline(line_stream, key, ' ');
//
//		//�擪�̃^�u�����͖�������
//		if (key[0] == '\t') {
//			key.erase(key.begin()); // �擪�̕������폜
//		}
//
//		//�擪������newmtl�Ȃ�}�e���A����
//		if (key == "newmtl") {
//
//			//���Ƀ}�e���A���������
//			if (material) {
//				//�}�e���A�����R���e�i�ɓo�^
//				AddMaterial(material);
//			}
//
//			//�V�����}�e���A���𐶐�
//			material = FbxMaterial::Create();
//			//�}�e���A�����ǂݍ���
//			line_stream >> material->GetName();
//		}
//		//�擪������Ka�Ȃ�A���r�G���g�F
//		if (key == "Ka") {
//			line_stream >> material->ambient.x;
//			line_stream >> material->ambient.y;
//			line_stream >> material->ambient.z;
//		}
//		//�擪������Kd�Ȃ�f�B�t���[�Y�F
//		if (key == "Kd") {
//			line_stream >> material->diffuse.x;
//			line_stream >> material->diffuse.y;
//			line_stream >> material->diffuse.z;
//		}
//		//�擪������Ks�Ȃ�X�y�L�����[�F
//		if (key == "Ks") {
//			line_stream >> material->specular.x;
//			line_stream >> material->specular.y;
//			line_stream >> material->specular.z;
//		}
//		//�擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
//		if (key == "map_Kd") {
//			//�e�N�X�`���̃t�@�C�����ǂݍ���
//			line_stream >> material->textureFilename;
//
//			//�t���p�X����t�@�C���������o��
//			size_t pos1;
//			pos1 = material->textureFilename.rfind('\\');
//			if (pos1 != std::string::npos) {
//				material->textureFilename = material->textureFilename.substr(pos1 + 1, material->textureFilename.size() - pos1 - 1);
//			}
//
//			pos1 = material->textureFilename.rfind('/');
//			if (pos1 != std::string::npos) {
//				material->textureFilename = material->textureFilename.substr(pos1 + 1, material->textureFilename.size() - pos1 - 1);
//			}
//		}
//	}
//	//�t�@�C�������
//	file.close();
//
//	if (material) {
//		//�}�e���A����o�^
//		AddMaterial(material);
//	}
//}
//
//void FbxModels::AddMaterial(FbxMaterial* material)
//{
//	materials.emplace(material->GetName(), material);
//}
//
//void FbxModels::CreateDescriptorHeap()
//{
//	HRESULT result = S_FALSE;
//
//	//�}�e���A���̐�
//	size_t count = materials.size();
//
//	//�f�X�N���v�^�q�[�v����(1�ɂ܂Ƃ߂������y�I0~100���萔�o�b�t�@,101~200��SRV...)��cmdList���Ăԉ񐔂�����
//	if (count > 0) {
//		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
//		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
//		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
//		descHeapDesc.NumDescriptors = (UINT)count;
//		result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));
//		if (FAILED(result)) {
//			assert(0);
//		}
//	}
//
//	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//}
//
//void FbxModels::LoadTextures()
//{
//	int textureIndex = 0;
//	std::string directoryPath = baseDirectory + name + "/";
//
//	for (auto& m : materials) {
//		FbxMaterial* material = m.second;
//
//		//�e�N�X�`������
//		if (material->GetTextureFilename().size() > 0) {
//			CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize);
//			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize);
//			//�}�e���A���Ƀe�N�X�`���ǂݍ���
//			material->LoadTexture(directoryPath, cpuDescHandleSRV, gpuDescHandleSRV);
//			textureIndex++;
//		}
//		//�e�N�X�`���Ȃ�
//		else {
//			CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize);
//			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize);
//			//�}�e���A���Ƀe�N�X�`���ǂݍ���
//			material->LoadTexture(baseDirectory, cpuDescHandleSRV, gpuDescHandleSRV);
//			textureIndex++;
//		}
//	}
//}
//
//void FbxModels::CreateMesh(FbxMesh* fbx_mesh)
//{
//	FbxMeshes* mesh = new FbxMeshes;
//
//	LoadIndices(mesh, fbx_mesh);
//	LoadVertices(mesh, fbx_mesh);
//	LoadNormals(mesh, fbx_mesh);
//	LoadUV(mesh, fbx_mesh);
//	LoadColors(mesh, fbx_mesh);
//	SetMaterialName(mesh, fbx_mesh);
//
//	meshes.push_back(mesh);
//}
//
//void FbxModels::LoadIndices(FbxMeshes* mesh_data, FbxMesh* mesh)
//{
//	int polygon_count = mesh->GetPolygonCount();
//
//	for (int i = 0; i < polygon_count; i++)
//	{
//		mesh_data->AddIndex(i * 3 + 2);
//		mesh_data->AddIndex(i * 3 + 1);
//		mesh_data->AddIndex(i * 3);
//	}
//}
//
//void FbxModels::LoadVertices(FbxMeshes* mesh_data, FbxMesh* mesh)
//{
//	FbxVector4* vertices = mesh->GetControlPoints();
//	int* indices = mesh->GetPolygonVertices();
//	int polygon_vertex_count = mesh->GetPolygonVertexCount();
//
//	for (int i = 0; i < polygon_vertex_count; i++)
//	{
//		FbxMeshes::VertexPosNormalUv vertex;
//		int index = indices[i];
//
//		vertex.pos.x = (float)-vertices[index][0];
//		vertex.pos.y = (float)vertices[index][1];
//		vertex.pos.z = (float)vertices[index][2];
//
//		mesh_data->AddVertex(vertex);
//	}
//}
//
//void FbxModels::LoadNormals(FbxMeshes* mesh_data, FbxMesh* mesh)
//{
//	FbxArray<FbxVector4> normals;
//	mesh->GetPolygonVertexNormals(normals);
//
//	for (int i = 0; i < normals.Size(); i++)
//	{
//		mesh_data->GetVertices()[i].normal.x = (float)-normals[i][0];
//		mesh_data->GetVertices()[i].normal.y = (float)normals[i][1];
//		mesh_data->GetVertices()[i].normal.z = (float)normals[i][2];
//	}
//}
//
//void FbxModels::LoadUV(FbxMeshes* mesh_data, FbxMesh* mesh)
//{
//	FbxStringList uvset_names;
//	mesh->GetUVSetNames(uvset_names);
//
//	FbxArray<FbxVector2> uv_buffer;
//	mesh->GetPolygonVertexUVs(uvset_names.GetStringAt(0), uv_buffer);
//
//	for (int i = 0; i < uv_buffer.Size(); i++)
//	{
//		FbxVector2& uv = uv_buffer[i];
//
//		mesh_data->GetVertices()[i].uv.x = (float)uv[0];
//		mesh_data->GetVertices()[i].uv.y = (float)(1.0 - uv[1]);
//	}
//}
//
//void FbxModels::LoadColors(FbxMeshes* mesh_data, FbxMesh* mesh)
//{
//	int color_count = mesh->GetElementVertexColorCount();
//	if (color_count == 0) { return; }
//
//	FbxGeometryElementVertexColor* vertex_colors = mesh->GetElementVertexColor(0);
//	if (vertex_colors == nullptr) { return; }
//
//	FbxLayerElement::EMappingMode mapping_mode = vertex_colors->GetMappingMode();
//	FbxLayerElement::EReferenceMode reference_mode = vertex_colors->GetReferenceMode();
//	if (mapping_mode == FbxLayerElement::eByPolygonVertex)
//	{
//		if (reference_mode == FbxLayerElement::eIndexToDirect)
//		{
//			FbxLayerElementArrayTemplate<FbxColor>& colors = vertex_colors->GetDirectArray();
//			FbxLayerElementArrayTemplate<int>& indeces = vertex_colors->GetIndexArray();
//			for (int i = 0; i < indeces.GetCount(); i++)
//			{
//				int id = indeces.GetAt(i);
//				FbxColor color = colors.GetAt(id);
//				mesh_data->GetVertices()[i].color.x = (float)color.mRed;
//				mesh_data->GetVertices()[i].color.y = (float)color.mGreen;
//				mesh_data->GetVertices()[i].color.z = (float)color.mBlue;
//				mesh_data->GetVertices()[i].color.w = (float)color.mAlpha;
//			}
//		}
//	}
//}
//
//void FbxModels::SetMaterialName(FbxMeshes* mesh_data, FbxMesh* mesh)
//{
//	if (mesh->GetElementMaterialCount() == 0)
//	{
//		mesh_data->SetName("");
//		return;
//	}
//
//	FbxLayerElementMaterial* material = mesh->GetElementMaterial(0);
//	int index = material->GetIndexArray().GetAt(0);
//	FbxSurfaceMaterial* surface_material = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);
//	if (surface_material != nullptr)
//	{
//		mesh_data->SetName(surface_material->GetName());
//	}
//	else
//	{
//		mesh_data->SetName("");
//	}
//}
