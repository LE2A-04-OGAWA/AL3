#include "DirectX/DirectXBase.h"
#include "../imgui/Dx12Wrapper.h"
#include "Users/GameScene.h"
#include "3D/Light.h"
#include "2D/PostEffect.h"
#include "3D/FbxLoader.h"

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//�E�B���h�E�^�C�g��
	LPCTSTR WindowTitle = L"OgwJ Engine";
	//�w�i�̐F
	float WindowColor[] = { 0.2f,0.2f,0.2f,1.0f };

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

	PostEffect* postEffect = nullptr;
	//Sprite::LoadTexture(100, L"Resources/white1280x720.png");
	//Sprite�̌p�����O���ēƗ�������
	postEffect = new PostEffect();
	postEffect->Init();

	/*----------�錾�@��������----------*/

	GameScene* Gamescene = nullptr;
	Gamescene = new GameScene();
	Gamescene->Init();

	/*----------�錾�@�����܂�----------*/

	while (Window::flag)
	{
		Window::Msg();
		Input::Update();

		Gamescene->Update();
		postEffect->PreDrawScene(DirectXImportant::cmdList.Get());
		Gamescene->Draw();
		postEffect->PostDrawScene(DirectXImportant::cmdList.Get());

		DirectXBase::BeforeDraw(WindowColor);
		//Gamescene->Draw();

		/*----------DirectX���t���[�������@��������----------*/

		postEffect->Draw(DirectXImportant::cmdList.Get());

		/*----------DirextX���t���[�������@�����܂�----------*/


		Dx12Wrapper::Draw(false);
		DirectXBase::AfterDraw();
		if (Input::isKeyTrigger(DIK_ESCAPE)) {
			break;
		}
	}

	FbxLoader::GetInstance()->Finalize();
	//�E�B���h�E�N���X��o�^����
	UnregisterClass(Window::windowClass.lpszClassName, Window::windowClass.hInstance);
}