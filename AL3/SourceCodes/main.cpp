#include "DirectX/DirectXBase.h"
#include "../imgui/Dx12Wrapper.h"
#include "../imgui/ImguiControl.h"
#include "Users/GameScene.h"
#include "3D/Light.h"
#include "2D/PostEffect.h"
#include "3D/FbxLoader.h"
#include "2D/RenderTarget.h"

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//�E�B���h�E�^�C�g��
	LPCTSTR WindowTitle = L"OgwJ Engine";
	//�w�i�̐F
	float WindowColor[] = {
		0.0f,
		0.0f,
		0.0f,
		0.0f };

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

	//�P�x���o�p
	SpriteInitData luminanceData;
	luminanceData.m_vsShaderName = L"Resources/Shaders/PostEffectTestVS.hlsl";
	luminanceData.m_psShaderName = L"Resources/Shaders/LuminancePS.hlsl";

	luminanceData.m_vsEntryPoint = "VSmain";
	luminanceData.m_psEntryPoint = "PSmain";

	luminanceData.m_width = WINDOW_WIDTH;
	luminanceData.m_height = WINDOW_HEIGHT;

	PostEffect* luminanceSprite = nullptr;
	luminanceSprite = new PostEffect();
	luminanceSprite->Init(luminanceData);

	//���u���[�p�̃X�v���C�g��������
	SpriteInitData xBlurSpriteInitData;
	xBlurSpriteInitData.m_vsShaderName = L"Resources/Shaders/PostEffectTestVS.hlsl";
	xBlurSpriteInitData.m_psShaderName = L"Resources/Shaders/PostEffectTestPS.hlsl";

	//xBlurSpriteInitData.m_vsShaderName = L"Resources/Shaders/SpriteVertexShader.hlsl";
	//xBlurSpriteInitData.m_psShaderName = L"Resources/Shaders/SpritePixelShader.hlsl";

	xBlurSpriteInitData.m_vsEntryPoint = "VSXmain";
	xBlurSpriteInitData.m_psEntryPoint = "PSBlur";

	xBlurSpriteInitData.m_gaussianSigma = 10.0f;

	//xBlurSpriteInitData.m_vsEntryPoint = "VSmain";
	//xBlurSpriteInitData.m_psEntryPoint = "PSmain";

	xBlurSpriteInitData.m_width = WINDOW_WIDTH;
	xBlurSpriteInitData.m_height = WINDOW_HEIGHT;

	PostEffect* xBlurSprite = nullptr;
	xBlurSprite = new PostEffect();
	xBlurSprite->Init(xBlurSpriteInitData);

	//�c�u���[�p�̃X�v���C�g��������
	SpriteInitData yBlurSpriteInitData;
	yBlurSpriteInitData.m_vsShaderName = L"Resources/Shaders/PostEffectTestVS.hlsl";
	yBlurSpriteInitData.m_psShaderName = L"Resources/Shaders/PostEffectTestPS.hlsl";

	//yBlurSpriteInitData.m_vsShaderName = L"Resources/Shaders/SpriteVertexShader.hlsl";
	//yBlurSpriteInitData.m_psShaderName = L"Resources/Shaders/SpritePixelShader.hlsl";

	yBlurSpriteInitData.m_vsEntryPoint = "VSYmain";
	yBlurSpriteInitData.m_psEntryPoint = "PSBlur";

	yBlurSpriteInitData.m_gaussianSigma = 10.0f;

	//yBlurSpriteInitData.m_vsEntryPoint = "VSmain";
	//yBlurSpriteInitData.m_psEntryPoint = "PSmain";

	yBlurSpriteInitData.m_width = WINDOW_WIDTH / 2;
	yBlurSpriteInitData.m_height = WINDOW_HEIGHT;

	PostEffect* yBlurSprite = nullptr;
	yBlurSprite = new PostEffect();
	yBlurSprite->Init(yBlurSpriteInitData);

	//�c���u���[���������G���t���[���o�b�t�@�ɓ\��t����ׂ̃X�v���C�g�̏�����
	SpriteInitData spriteInitData;

	spriteInitData.m_width = WINDOW_WIDTH;
	spriteInitData.m_height = WINDOW_HEIGHT;

	spriteInitData.m_vsShaderName = L"Resources/Shaders/SpriteVertexShader.hlsl";
	spriteInitData.m_psShaderName = L"Resources/Shaders/SpritePixelShader.hlsl";

	spriteInitData.m_vsEntryPoint = "VSmain";
	spriteInitData.m_psEntryPoint = "PSmain";

	spriteInitData.m_alphaBlendMode = AlphaBlendMode::ALPHA_BLENDMODE_ADD;

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
		//WindowColor[0] = ImguiControl::Imgui_backColor_r;
		//WindowColor[1] = ImguiControl::Imgui_backColor_g;
		//WindowColor[2] = ImguiControl::Imgui_backColor_b;
		//PostEffect::clearColor[0] = ImguiControl::Imgui_backColor_r;
		//PostEffect::clearColor[1] = ImguiControl::Imgui_backColor_g;
		//PostEffect::clearColor[2] = ImguiControl::Imgui_backColor_b;

		//����Ⴄ
		//xBlurSprite->SetClearColor(WindowColor);
		//yBlurSprite->SetClearColor(WindowColor);
		//copyToFrameBufferSprite->SetClearColor(WindowColor);

		Window::Msg();
		Input::Update();

		Gamescene->Update();
		//Gamescene->Draw();

		luminanceSprite->PreDrawScene(DirectXImportant::cmdList.Get(), luminanceData, WindowColor);
		Gamescene->Draw();
		luminanceSprite->PostDrawScene(DirectXImportant::cmdList.Get());

		xBlurSprite->PreDrawScene(DirectXImportant::cmdList.Get(), xBlurSpriteInitData, WindowColor);
		luminanceSprite->Draw(DirectXImportant::cmdList.Get());
		xBlurSprite->PostDrawScene(DirectXImportant::cmdList.Get());

		yBlurSprite->PreDrawScene(DirectXImportant::cmdList.Get(), yBlurSpriteInitData, WindowColor);
		xBlurSprite->Draw(DirectXImportant::cmdList.Get());
		yBlurSprite->PostDrawScene(DirectXImportant::cmdList.Get());

		copyToFrameBufferSprite->PreDrawScene(DirectXImportant::cmdList.Get(), spriteInitData, WindowColor);
		yBlurSprite->Draw(DirectXImportant::cmdList.Get());
		copyToFrameBufferSprite->PostDrawScene(DirectXImportant::cmdList.Get());

		DirectXBase::BeforeDraw(WindowColor);

		/*----------DirectX���t���[�������@��������----------*/

		//luminanceSprite->Draw(DirectXImportant::cmdList.Get());
		Gamescene->Draw();
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
