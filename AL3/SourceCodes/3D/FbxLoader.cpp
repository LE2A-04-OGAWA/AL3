﻿#include "FbxLoader.h"
#include "../DirectX/DirectXImportant.h"
#include <cassert>

//using namespace DirectX;

const std::string FbxLoader::baseDirectory = "Resources/";
const std::string FbxLoader::defaultTextureFileName = "white1x1.png";

FbxLoader* FbxLoader::GetInstance()
{
	static FbxLoader instance;
	return &instance;
}

void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src)
{
	//行
	for (int i = 0; i < 4; i++) {
		//列
		for (int j = 0; j < 4; j++) {
			//1要素コピー
			dst->r[i].m128_f32[j] = (float)src.Get(i, j);
		}
	}
}

void FbxLoader::Init(ID3D12Device* device)
{
	//再初期化チェック
	assert(fbxManager == nullptr);
	//引数からメンバ変数に代入
	this->device = device;
	//FBXマネージャーの生成
	fbxManager = FbxManager::Create();
	//FBXマネージャーの入出力設定
	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);
	//FBXインポーターの生成
	fbxImporter = FbxImporter::Create(fbxManager, "");
}

void FbxLoader::Finalize()
{
	//各種FBXインスタンスの破棄
	fbxImporter->Destroy();
	fbxManager->Destroy();
}

FbxModel* FbxLoader::LoadModelFromFile(const string& modelName)
{
	//モデルと同じ名前のフォルダから読み込む
	const string directotyPath = baseDirectory + modelName + "/";
	//拡張子,fbxを付与
	const string fileName = modelName + ".fbx";
	//連結してフルパスを得る
	const string fullPath = directotyPath + fileName;

	//ファイル名を指定してFBXファイルを読み込む
	if (!fbxImporter->Initialize(
		fullPath.c_str(), -1, fbxManager->GetIOSettings())) {
		assert(0);
	}

	//シーン生成
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");

	//ファイルからロードしたFBXの情報をシーンにインポート
	fbxImporter->Import(fbxScene);

	//メッシュに使われているマテリアル単位でメッシュを分割する
	FbxGeometryConverter converter(fbxManager);
	bool isConvert = converter.SplitMeshesPerMaterial(fbxScene, true);

	//モデル生成
	FbxModel* model = new FbxModel();
	//std::shared_ptr<FbxModel> model = std::make_shared<FbxModel>();
	model->name = modelName;

	//FBXノードの数を取得
	int nodeCount = fbxScene->GetNodeCount();

	//あらかじめ必要数分のメモリを確保することで、アドレスがずれるのを予防←マテリアルとメッシュの数を予め確保する??(これから)
	model->nodes.reserve(nodeCount);	//vectorの要素数を予約

	// 複数マテリアル対応版
	// GetSrcObjectCount<FbxSurfaceMaterial>の数分マテリアルをロード(情報保存)
	// GetSrcObjectCount<FbxMesh>の数分メッシュを生成(情報保存)
	// IndexBuff,VertexBuff,InputLayout生成
	// メッシュの数だけ描画を回す

	model->fbxScene = fbxScene;

	//ルートノードから順に解析してモデルに流し込む
	ParseNodeRecursive(model, fbxScene->GetRootNode());

	//FBXシーン解放
	//fbxScene->Destroy();

	//バッファ生成
	model->CreateBuffers(DirectXImportant::dev.Get());

	return model;
}

void FbxLoader::ParseNodeRecursive(FbxModel* model, FbxNode* fbxNode, Node* parent)
{
	//ノード名を取得
	string name = fbxNode->GetName();

	/*モデルにノードを追加*/
	model->nodes.emplace_back();
	Node& node = model->nodes.back();

	//ノード名を取得
	node.name = fbxNode->GetName();

	//FBXノードのローカル移動情報
	FbxDouble3 rotation = fbxNode->LclRotation.Get();
	FbxDouble3 scaling = fbxNode->LclScaling.Get();
	FbxDouble3 translation = fbxNode->LclTranslation.Get();

	//形式変換して代入
	node.rotation = { (float)rotation[0],(float)rotation[1],(float)rotation[2],0.0f };
	node.scaling = { (float)scaling[0],(float)scaling[1],(float)scaling[2],0.0f };
	node.translation = { (float)translation[0],(float)translation[1],(float)translation[2],1.0f };

	//回転角をDegree(度)からラジアンに変換
	node.rotation.m128_f32[0] = DirectX::XMConvertToRadians(node.rotation.m128_f32[0]);
	node.rotation.m128_f32[1] = DirectX::XMConvertToRadians(node.rotation.m128_f32[1]);
	node.rotation.m128_f32[2] = DirectX::XMConvertToRadians(node.rotation.m128_f32[2]);

	//スケール、回転、平行移動行列の計算
	DirectX::XMMATRIX matScaling, matRotation, matTranslaton;
	matScaling = DirectX::XMMatrixScalingFromVector(node.scaling);
	matRotation = DirectX::XMMatrixRotationRollPitchYawFromVector(node.rotation);
	matTranslaton = DirectX::XMMatrixTranslationFromVector(node.translation);

	//ローカル変形行列の計算
	node.transform = DirectX::XMMatrixIdentity();
	node.transform *= matScaling;
	node.transform *= matRotation;
	node.transform *= matTranslaton;

	//グローバル変形行列の計算
	node.globalTransform = node.transform;
	if (parent) {
		node.parent = parent;
		//親の変形を乗算
		node.globalTransform *= parent->globalTransform;
	}

	/*FBXノードの情報を解析してノードに記録*/
	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

	if (fbxNodeAttribute) {
		if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
			model->meshNode = &node;
			ParseMesh(model, fbxNode);
		}
	}

	/*FBXノードのメッシュ情報を解析(Todo)*/

	//子ノードに対して再帰呼び出し
	for (int i = 0; i < fbxNode->GetChildCount(); i++) {
		ParseNodeRecursive(model, fbxNode->GetChild(i), &node);
	}
}

void FbxLoader::ParseMesh(FbxModel* model, FbxNode* fbxNode)
{
	//ノードのメッシュを取得
	FbxMesh* fbxMesh = fbxNode->GetMesh();
	FbxModel::MeshData meshdata;

	//for分で回す必要あり
	//頂点情報読み取り←LoadVertices
	int texture_num = model->fbxScene->GetSrcObjectCount<FbxFileTexture>();
	for (int i = 0; i < texture_num; i++)
	{
		ParseMeshVertices(model, fbxMesh, meshdata);
		//面を構成するデータの読み取り←LoadIndices...
		ParseMeshFaces(model, fbxMesh, meshdata);
		model->m_MeshList.push_back(meshdata);
	}

	//マテリアルの読み取り←LoadMaterial(中でマテリアルの分だけ回してます)
	ParseMaterial(model, fbxNode);
	//スキニング情報の読み取り
	ParseSkin(model, fbxMesh);
}

void FbxLoader::ParseMeshVertices(FbxModel* model, FbxMesh* fbxMesh, FbxModel::MeshData& meshdata)
{
	auto& vertices = model->vertices;

	//頂点座標データの数
	const int controlPointCount =
		fbxMesh->GetControlPointsCount();

	//必要な数だけ頂点データ配列を確保
	FbxModel::VertexPosNormalUvSkin vert{};
	model->vertices.resize(controlPointCount, vert);

	//FBXメッシュの頂点座標配列を取得
	FbxVector4* pCoord = fbxMesh->GetControlPoints();

	//FBXメッシュの全頂点座標をモデル内の配列にコピーする
	for (int i = 0; i < controlPointCount; i++) {
		FbxModel::VertexPosNormalUvSkin& vertex = vertices[i];
		//座標のコピー
		vertex.pos.x = (float)pCoord[i][0];
		vertex.pos.y = (float)pCoord[i][1];
		vertex.pos.z = (float)pCoord[i][2];
	}

	//Test
	FbxVector4* l_vertices = fbxMesh->GetControlPoints();
	int* l_indices = fbxMesh->GetPolygonVertices();
	int l_polygon_vertex_count = fbxMesh->GetPolygonVertexCount();
	for (int i = 0; i < l_polygon_vertex_count; i++)
	{
		int index = l_indices[i];

		DirectX::XMFLOAT3 pos;
		pos.x = (float)pCoord[index][0];
		pos.y = (float)pCoord[index][1];
		pos.z = (float)pCoord[index][2];

		meshdata.m_Pos.push_back(pos);
	}
}

void FbxLoader::ParseMeshFaces(FbxModel* model, FbxMesh* fbxMesh, FbxModel::MeshData& meshdata)
{
	auto& vertices = model->vertices;
	auto& indices = model->indices;

	//1ファイルに複数メッシュのモデルは非対応
	//assert(indices.size() == 0);

	//面の数
	const int polygonCount = fbxMesh->GetPolygonCount();

	//UVデータの数
	const int textureUVCount = fbxMesh->GetTextureUVCount();

	//UV名リスト
	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);

	//面ごとの情報読み取り
	for (int i = 0; i < polygonCount; i++) {
		//面を構成する頂点の数を取得(3なら三角形ポリゴン)
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		assert(polygonSize <= 4);

		//1頂点ずつ処理
		for (int j = 0; j < polygonSize; j++) {
			//FBX頂点配列のインデックス
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);

			//頂点法線読み込み
			FbxModel::VertexPosNormalUvSkin& vertex = vertices[index];
			FbxVector4 normal;
			if (fbxMesh->GetPolygonVertexNormal(i, j, normal)) {
				vertex.normal.x = (float)normal[0];
				vertex.normal.y = (float)normal[1];
				vertex.normal.z = (float)normal[2];
			}

			//テクスチャUV読み込み
			if (textureUVCount > 0) {
				FbxVector2 uvs;
				bool lUnmappedUV;

				//0番決め打ちで読み込み
				if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV)) {
					vertex.uv.x = (float)uvs[0];
					vertex.uv.y = (float)uvs[1];
				}
			}

			//インデックス配列に頂点インデックス追加
			//3頂点目までなら
			if (j < 3) {
				//1点追加し、他の2点と三角形を構築する
				indices.push_back(index);
			}

			//4頂点目
			else {
				//3点追加し、4角形の0,1,2,3の内 2,3,0で三角形を構築する
				int index2 = indices[indices.size() - 1];
				int index3 = index;
				int index0 = indices[indices.size() - 3];
				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index0);
			}
		}
	}


	//Test
	int m_polygon_count = fbxMesh->GetPolygonCount();
	for (int i = 0; i < m_polygon_count; i++)
	{
		meshdata.m_Indices.push_back(i * 3 + 2);
		meshdata.m_Indices.push_back(i * 3 + 1);
		meshdata.m_Indices.push_back(i * 3);
	}

	FbxArray<FbxVector4> l_normals;
	fbxMesh->GetPolygonVertexNormals(l_normals);
	for (int i = 0; i < l_normals.Size(); i++)
	{
		DirectX::XMFLOAT3 normal;
		normal.x = (float)-l_normals[i][0];
		normal.y = (float)l_normals[i][1];
		normal.z = (float)l_normals[i][2];
		meshdata.m_Normal.push_back(normal);
	}

	FbxStringList l_uvset_names;
	fbxMesh->GetUVSetNames(l_uvset_names);
	FbxArray<FbxVector2> l_uv_buffer;
	fbxMesh->GetPolygonVertexUVs(l_uvset_names.GetStringAt(0), l_uv_buffer);
	for (int i = 0; i < l_uv_buffer.Size(); i++)
	{
		FbxVector2& uv = l_uv_buffer[i];
		DirectX::XMFLOAT2 l_uv;
		l_uv.x = (float)uv[0];
		l_uv.y = (float)(1.0 - uv[1]);
		meshdata.m_Uv.push_back(l_uv);
	}

	int l_color_count = fbxMesh->GetElementVertexColorCount();
	if (l_color_count == 0) { return; }
	FbxGeometryElementVertexColor* l_vertex_colors = fbxMesh->GetElementVertexColor(0);
	if (l_vertex_colors == nullptr) { return; }
	FbxLayerElement::EMappingMode l_mapping_mode = l_vertex_colors->GetMappingMode();
	FbxLayerElement::EReferenceMode l_reference_mode = l_vertex_colors->GetReferenceMode();
	if (l_mapping_mode == FbxLayerElement::eByPolygonVertex)
	{
		if (l_reference_mode == FbxLayerElement::eIndexToDirect)
		{
			FbxLayerElementArrayTemplate<FbxColor>& l_colors =
				l_vertex_colors->GetDirectArray();
			FbxLayerElementArrayTemplate<int>& l_indeces =
				l_vertex_colors->GetIndexArray();
			for (int i = 0; i < l_indeces.GetCount(); i++)
			{
				int id = l_indeces.GetAt(i);
				FbxColor color = l_colors.GetAt(id);
				DirectX::XMFLOAT4 l_color;
				l_color.x = (float)color.mAlpha;
				l_color.x = (float)color.mRed;
				l_color.x = (float)color.mGreen;
				l_color.x = (float)color.mBlue;
				meshdata.m_Color.push_back(l_color);
			}
		}
	}

	if (fbxMesh->GetElementMaterialCount() == 0)
	{
		meshdata.m_MaterialName = "";
		return;
	}
	FbxLayerElementMaterial* l_material = fbxMesh->GetElementMaterial(0);
	int l_index = l_material->GetIndexArray().GetAt(0);
	FbxSurfaceMaterial* l_surface_material =
		fbxMesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(l_index);
	if (l_surface_material != nullptr)
	{
		meshdata.m_MaterialName = l_surface_material->GetName();
	}
	else
	{
		meshdata.m_MaterialName = "";
	}
}

void FbxLoader::ParseMaterial(FbxModel* model, FbxNode* fbxNode)
{
	const int materialCount = fbxNode->GetMaterialCount();
	if (materialCount > 0) {
		for (int i = 0; i < materialCount; i++)
		{
			//先頭のマテリアルを取得
			FbxSurfaceMaterial* material = fbxNode->GetMaterial(i);

			//テクスチャを読み込んだかどうかを表すフラグ
			bool textureLoaded = false;

			if (material) {
				//FbxSurfaceLambertクラスかどうかを調べる
				if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
					FbxSurfaceLambert* lambert =
						static_cast<FbxSurfaceLambert*>(material);

					//環境光係数
					FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
					model->ambient.x = (float)ambient.Get()[0];
					model->ambient.y = (float)ambient.Get()[1];
					model->ambient.z = (float)ambient.Get()[2];

					//拡散反射光係数
					FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
					model->diffuse.x = (float)diffuse.Get()[0];
					model->diffuse.y = (float)diffuse.Get()[1];
					model->diffuse.z = (float)diffuse.Get()[2];
				}

				//ディフューズテクスチャを取り出す
				const FbxProperty diffuseProperty =
					material->FindProperty(FbxSurfaceMaterial::sDiffuse);
				if (diffuseProperty.IsValid()) {
					const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
					if (texture) {
						const char* filepath = texture->GetFileName();

						//ファイルパスからファイル名抽出
						string path_str(filepath);
						string name = ExtractFileName(path_str);

						//テクスチャ読み込み
						LoadTexture(model, baseDirectory + model->name + "/" + name);
						textureLoaded = true;
					}
				}
			}

			//テクスチャがない場合は白テクスチャを貼る
			if (!textureLoaded) {
				LoadTexture(model, baseDirectory + defaultTextureFileName);
			}
		}
	}

	//Test
	FbxModel::Material entry_material;
	enum class MaterialOrder
	{
		Ambient,
		Diffuse,
		Specular,
		MaxOrder,
	};
	FbxDouble3 colors[(int)MaterialOrder::MaxOrder];
	FbxDouble factors[(int)MaterialOrder::MaxOrder];
	int material_num = model->fbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();
	for (int i = 0; i < material_num; i++)
	{
		FbxSurfaceMaterial* material =
			model->fbxScene->GetSrcObject<FbxSurfaceMaterial>(i);
		FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sAmbient);
		if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
		{
			const char* element_check_list[] =
			{
				FbxSurfaceMaterial::sAmbient,
				FbxSurfaceMaterial::sDiffuse,
			};

			const char* factor_check_list[] =
			{
				FbxSurfaceMaterial::sAmbientFactor,
				FbxSurfaceMaterial::sDiffuseFactor,
			};

			for (int i = 0; i < 2; i++)
			{
				prop = material->FindProperty(element_check_list[i]);
				if (prop.IsValid()) { colors[i] = prop.Get<FbxDouble3>(); }
				else { colors[i] = FbxDouble3(1.0, 1.0, 1.0); }
				prop = material->FindProperty(factor_check_list[i]);
				if (prop.IsValid()) { factors[i] = prop.Get<FbxDouble>(); }
				else { factors[i] = 1.0; }
			}
		}

		FbxDouble3 color = colors[(int)MaterialOrder::Ambient];
		FbxDouble factor = factors[(int)MaterialOrder::Ambient];
		entry_material.m_Ambient.x = (float)color[0];
		entry_material.m_Ambient.y = (float)color[1];
		entry_material.m_Ambient.z = (float)color[2];

		color = colors[(int)MaterialOrder::Diffuse];
		factor = factors[(int)MaterialOrder::Diffuse];
		entry_material.m_Ambient.x = (float)color[0];
		entry_material.m_Ambient.y = (float)color[1];
		entry_material.m_Ambient.z = (float)color[2];

		model->m_Materials[material->GetName()] = entry_material;

		prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
		FbxFileTexture* texture = nullptr;
		std::string keyword;
		int texture_num = prop.GetSrcObjectCount<FbxFileTexture>();
		if (texture_num > 0) { texture = prop.GetSrcObject<FbxFileTexture>(0); }
		else
		{
			int layer_num = prop.GetSrcObjectCount<FbxLayeredTexture>();
			if (layer_num > 0) { texture = prop.GetSrcObject<FbxFileTexture>(0); }
		}
		if (texture != nullptr &&
			LoadTexture(model, keyword) == true)
		{
			model->m_MaterialLinks[material->GetName()] = model->m_Textures[keyword];
		}
	}
}

bool FbxLoader::LoadTexture(FbxModel* model, const std::string& fullpath)
{
	HRESULT result = S_FALSE;

	//WICテクスチャのロード
	DirectX::TexMetadata& metadata = model->metadata;
	DirectX::ScratchImage& scratchImg = model->scratchImg;

	//ユニコード文字列に変換
	wchar_t wfilepath[128];
	MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));
	result = LoadFromWICFile(
		wfilepath, DirectX::WIC_FLAGS_NONE,
		&metadata, scratchImg
	);
	if (FAILED(result)) { assert(0); }

	//map型に保管したい

	return true;
}

std::string FbxLoader::ExtractFileName(const std::string& path)
{
	size_t pos1;

	//区切り文字 '\\' が出てくる一番最後の部分を検索
	pos1 = path.rfind('\\');
	if (pos1 != string::npos) { return path.substr(pos1 + 1, path.size() - pos1 - 1); }

	//区切り文字 '//' が出てくる一番最後の部分を検索
	pos1 = path.rfind('/');
	if (pos1 != string::npos) { return path.substr(pos1 + 1, path.size() - pos1 - 1); }

	return path;
}

void FbxLoader::ParseSkin(FbxModel* model, FbxMesh* fbxMesh)
{
	//スキニング情報
	FbxSkin* fbxSkin =
		static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));

	//スキニング情報がなければ終了
	if (fbxSkin == nullptr) {

		//各頂点について処理
		for (int i = 0; i < model->vertices.size(); i++) {
			//最初のボーン(単位行列)の影響を100%にする
			model->vertices[i].boneIndex[0] = 0;
			model->vertices[i].boneWeight[0] = 1.0f;
		}

		return;
	}

	//ボーン配列の参照
	std::vector<FbxModel::Bone>& bones = model->bones;

	//ボーンの数
	int clusterCount = fbxSkin->GetClusterCount();
	bones.reserve(clusterCount);

	//全てのボーンについて
	for (int i = 0; i < clusterCount; i++) {
		//FBXボーン情報
		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

		//ボーン自体のノードの名前を取得
		const char* boneName = fbxCluster->GetLink()->GetName();

		//新しくボーンを追加し、追加したボーンの参照を得る
		bones.emplace_back(FbxModel::Bone(boneName));
		FbxModel::Bone& bone = bones.back();

		//自作ボーンとFBXのボーンを紐づける
		bone.fbxCluster = fbxCluster;

		//FBXから初期姿勢行列を取得する
		FbxAMatrix fbxMat;
		fbxCluster->GetTransformLinkMatrix(fbxMat);

		//XMMATRIX型に変換する
		DirectX::XMMATRIX initialPose;
		ConvertMatrixFromFbx(&initialPose, fbxMat);

		//初期姿勢行列の逆行列を得る
		bone.invInitialPose = DirectX::XMMatrixInverse(nullptr, initialPose);
	}

	//ボーン番号とスキンウェイトのペア
	struct  WeightSet
	{
		UINT index;
		float weight;
	};

	//二次元配列(ジャグ配列)
	//list:頂点が影響を受けるボーンの全リスト
	//vector:それを全頂点分
	std::vector<std::list<WeightSet>>
		weightLists(model->vertices.size());

	//全てのボーンについて
	for (int i = 0; i < clusterCount; i++) {
		//FBXボーン情報
		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

		//このボーンに影響を受ける頂点の数
		int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();

		//このボーンに影響を受ける頂点の配列
		int* controlPointIndices = fbxCluster->GetControlPointIndices();
		double* controlPointWeights = fbxCluster->GetControlPointWeights();

		//影響を受ける全頂点について
		for (int j = 0; j < controlPointIndicesCount; j++) {
			//頂点番号
			int vertIndex = controlPointIndices[j];

			//スキンウェイト
			float weight = (float)controlPointWeights[j];

			//その頂点の影響を受けるボーンリストに、ボーンとウェイトのペアを追加
			weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i,weight });
		}

		//頂点配列書き換え用の参照
		auto& vertices = model->vertices;

		//各頂点について処理
		for (int i = 0; i < vertices.size(); i++) {
			//頂点のウェイトから最も大きい4つを選択
			auto& weightList = weightLists[i];

			//大小比較用のラムダ式を指定して降順にソート
			weightList.sort(
				[](auto const& lhs, auto const& rhs) {
					//左の要素の方が大きければtrue,それでなければfalseを返す
					return lhs.weight > rhs.weight;
				});

			int weightArrayIndex = 0;

			//降順ソート済みのウェイトリストから
			for (auto& weightSet : weightList) {
				//頂点データに書き込み
				vertices[i].boneIndex[weightArrayIndex] = weightSet.index;
				vertices[i].boneWeight[weightArrayIndex] = weightSet.weight;

				//4つに達したら終了
				if (++weightArrayIndex >= FbxModel::MAX_BONE_INDICES) {
					float weight = 0.0f;

					//2番目移行のウェイトを合計
					for (int j = 1; j < FbxModel::MAX_BONE_INDICES; j++) {
						weight += vertices[i].boneWeight[j];
					}

					//合計で1.0f(100%)になるように調整
					vertices[i].boneWeight[0] = 1.0f - weight;
					break;
				}
			}
		}
	}
}
