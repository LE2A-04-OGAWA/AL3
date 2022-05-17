#include "DirectX/DirectXBase.h"
#include "../imgui/Dx12Wrapper.h"
#include "../imgui/ImguiControl.h"
#include "Users/GameScene.h"
#include "3D/Light.h"
#include "2D/PostEffect.h"
#include "3D/FbxLoader.h"
#include "2D/RenderTarget.h"

void CalcWeightsTableFromGaussian(float* weightsTbl, int sizeOfWeightsTbl, float sigma);

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//�E�B���h�E�^�C�g��
	LPCTSTR WindowTitle = L"OgwJ Engine";
	//�w�i�̐F
	float WindowColor[] = {
		ImguiControl::Imgui_backColor_r,
		ImguiControl::Imgui_backColor_g,
		ImguiControl::Imgui_backColor_b,
		1.0f };

#ifdef _DEBUG
	Window::Debuglayer();
#endif

	HWND hwnd = Window::Init(WindowTitle);
	DirectXBase::Init(hwnd);
	Input::Init(hwnd);
	Dx12Wrapper::ImguiInit();
	Dx12Wrapper::WindowsInit(hwnd);
	Dx12Wrapper::DirectXInit();
	Object::StaticInit(DirectXImportant::dev.Get());
	Light::StaticInit(DirectXImportant::dev.Get());
	if (!Sprite::StaticInitialize(DirectXImportant::dev.Get(), WINDOW_WIDTH, WINDOW_HEIGHT)) {
		assert(0);
		return 1;
	}
	FbxLoader::GetInstance()->Init(DirectXImportant::dev.Get());

	GameScene* Gamescene = nullptr;
	Gamescene = new GameScene();
	Gamescene->Init();

	//1,GameScene�`��p�̃����_�[�^�[�Q�b�g���쐬
	RenderTarget mainRenderTarget;
	mainRenderTarget.Create(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT
	);

	//2,�K�E�X�u���[�p�̏d�݃e�[�u�����v�Z����
	const int NUM_HEIGHT = 8;

	//�e�[�u���̃T�C�Y��8
	float weights[NUM_HEIGHT];

	//�d�݃e�[�u�����v�Z����
	CalcWeightsTableFromGaussian(
		weights,		//�i�[��
		NUM_HEIGHT,		//�d�݃e�[�u���̃T�C�Y
		8.0f			//�{�P�
	);

	//3,���u���[�p�̃����_�����O�^�[�Q�b�g���쐬
	RenderTarget xBlurRenderTarget;
	xBlurRenderTarget.Create(
		WINDOW_WIDTH / 2.0f,
		WINDOW_HEIGHT,
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT
	);

	//4,���u���[�p�̃X�v���C�g��������
	SpriteInitData xBlurSpriteInitData;
	xBlurSpriteInitData.m_vsShaderName = L"Resources/Shaders/PostEffectTestVS.hlsl";
	xBlurSpriteInitData.m_psShaderName = L"Resources/Shaders/PostEffectTestPS.hlsl";

	xBlurSpriteInitData.m_vsEntryPoint = "VSXmain";
	xBlurSpriteInitData.m_psEntryPoint = "PSXmain";

	xBlurSpriteInitData.m_vsEntryPoint = "VSmain";
	xBlurSpriteInitData.m_psEntryPoint = "PSmain";

	xBlurSpriteInitData.m_width = WINDOW_WIDTH;
	xBlurSpriteInitData.m_height = WINDOW_HEIGHT;

	//xBlurSpriteInitData.m_textures.push_back(&mainRenderTarget.GetRenderTargetTexture());
	//xBlurSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();	//���������g���ĂȂ�

	//xBlurSpriteInitData.m_expandConstantBuffer = &weights;
	//xBlurSpriteInitData.m_expandConstantBufferSize = sizeof(weights);

	//PostEffect* xBlurSprite = nullptr;
	//xBlurSprite = new PostEffect();
	//xBlurSprite->Init(xBlurSpriteInitData);

	//5,�c�u���[�p�̃����_�����O�^�[�Q�b�g���쐬
	RenderTarget yBlurRenderTarget;
	yBlurRenderTarget.Create(
		WINDOW_WIDTH,
		WINDOW_HEIGHT / 2.0f,
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT
	);

	//6,�c�u���[�p�̃X�v���C�g��������
	SpriteInitData yBlurSpriteInitData;
	yBlurSpriteInitData.m_vsShaderName = L"Resources/Shaders/PostEffectTestVS.hlsl";
	yBlurSpriteInitData.m_psShaderName = L"Resources/Shaders/PostEffectTestPS.hlsl";

	yBlurSpriteInitData.m_vsEntryPoint = "VSYmain";
	yBlurSpriteInitData.m_psEntryPoint = "PSYmain";

	yBlurSpriteInitData.m_vsEntryPoint = "VSmain";
	yBlurSpriteInitData.m_psEntryPoint = "PSmain";

	yBlurSpriteInitData.m_width = WINDOW_WIDTH;
	yBlurSpriteInitData.m_height = WINDOW_HEIGHT;

	//yBlurSpriteInitData.m_textures.push_back(&xBlurRenderTarget.GetRenderTargetTexture());
	//yBlurSpriteInitData.m_textures[0] = &xBlurRenderTarget.GetRenderTargetTexture();	//���������g���ĂȂ�

	yBlurSpriteInitData.m_expandConstantBuffer = &weights;
	yBlurSpriteInitData.m_expandConstantBufferSize = sizeof(weights);

	PostEffect* yBlurSprite = nullptr;
	yBlurSprite = new PostEffect();
	yBlurSprite->Init(yBlurSpriteInitData);

	//7,�c���u���[���������G���t���[���o�b�t�@�ɓ\��t����ׂ̃X�v���C�g�̏�����
	SpriteInitData spriteInitData;
	//spriteInitData.m_textures.push_back(&yBlurRenderTarget.GetRenderTargetTexture());
	//spriteInitData.m_textures[0] = &yBlurRenderTarget.GetRenderTargetTexture();

	spriteInitData.m_width = WINDOW_WIDTH;
	spriteInitData.m_height = WINDOW_HEIGHT;

	spriteInitData.m_vsShaderName = L"Resources/Shaders/SpriteVertexShader.hlsl";
	spriteInitData.m_psShaderName = L"Resources/Shaders/SpritePixelShader.hlsl";

	spriteInitData.m_vsEntryPoint = "VSmain";
	spriteInitData.m_psEntryPoint = "PSmain";

	PostEffect* copyToFrameBufferSprite = nullptr;
	copyToFrameBufferSprite = new PostEffect();
	copyToFrameBufferSprite->Init(spriteInitData);


	/*----------�錾�@��������----------*/

	/*GameScene* Gamescene = nullptr;
	Gamescene = new GameScene();
	Gamescene->Init();*/

	/*----------�錾�@�����܂�----------*/

	while (Window::flag)
	{
		WindowColor[0] = ImguiControl::Imgui_backColor_r;
		WindowColor[1] = ImguiControl::Imgui_backColor_g;
		WindowColor[2] = ImguiControl::Imgui_backColor_b;

		Window::Msg();
		Input::Update();

		Gamescene->Update();
		//xBlurSprite->PreDrawScene(DirectXImportant::cmdList.Get());
		//Gamescene->Draw();
		//xBlurSprite->PostDrawScene(DirectXImportant::cmdList.Get());

		//yBlurSprite->PreDrawScene(DirectXImportant::cmdList.Get());
		//xBlurSprite->Draw(DirectXImportant::cmdList.Get());
		//yBlurSprite->PostDrawScene(DirectXImportant::cmdList.Get());

		//copyToFrameBufferSprite->PreDrawScene(DirectXImportant::cmdList.Get());
		//yBlurSprite->Draw(DirectXImportant::cmdList.Get());
		//copyToFrameBufferSprite->PostDrawScene(DirectXImportant::cmdList.Get());

		yBlurSprite->PreDrawScene(DirectXImportant::cmdList.Get());
		xBlurSprite->Draw(DirectXImportant::cmdList.Get());
		yBlurSprite->PostDrawScene(DirectXImportant::cmdList.Get());

		copyToFrameBufferSprite->PreDrawScene(DirectXImportant::cmdList.Get());
		yBlurSprite->Draw(DirectXImportant::cmdList.Get());
		copyToFrameBufferSprite->PostDrawScene(DirectXImportant::cmdList.Get());

		DirectXBase::BeforeDraw(WindowColor);

		/*----------DirectX���t���[�������@��������----------*/

		//8,�����_�����O�^�[�Q�b�g��mainRenderTarget�ɕύX����(HLSL�̖������̕��@�͓�)

		//9,mainRenderTarget�Ɋe�탂�f����`�悷��

		//10,mainRenderTarget�ɕ`�悳�ꂽ�摜�ɉ��u���[��������

		//11,�c�u���[���s��

		//12,mainRenderTarget�̊G���t���[���o�b�t�@�ɃR�s�[

		// �� HLSL�̉���...


		//Gamescene->Draw();
		//xBlurSprite->Draw(DirectXImportant::cmdList.Get());
		//yBlurSprite->Draw(DirectXImportant::cmdList.Get());
		copyToFrameBufferSprite->Draw(DirectXImportant::cmdList.Get());

		/*----------DirextX���t���[�������@�����܂�----------*/


		Dx12Wrapper::Draw(true);
		DirectXBase::AfterDraw();
		if (Input::isKeyTrigger(DIK_ESCAPE)) {
			break;
		}
	}

	FbxLoader::GetInstance()->Finalize();
	//�E�B���h�E�N���X��o�^����
	UnregisterClass(Window::windowClass.lpszClassName, Window::windowClass.hInstance);
}

void CalcWeightsTableFromGaussian(float* weightsTbl, int sizeOfWeightsTbl, float sigma)
{
	// �d�݂̍��v���L�^����ϐ����`����
	float total = 0;

	// ��������K�E�X�֐���p���ďd�݂��v�Z���Ă���
	// ���[�v�ϐ���x����e�N�Z������̋���
	for (int x = 0; x < sizeOfWeightsTbl; x++)
	{
		weightsTbl[x] = expf(-0.5f * (float)(x * x) / sigma);
		total += 2.0f * weightsTbl[x];
	}

	// �d�݂̍��v�ŏ��Z���邱�ƂŁA�d�݂̍��v��1�ɂ��Ă���
	for (int i = 0; i < sizeOfWeightsTbl; i++)
	{
		weightsTbl[i] /= total;
	}
}
