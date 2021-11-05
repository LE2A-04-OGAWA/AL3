#include "Input.h"

BYTE Input::keys[256] = { 0 };
BYTE Input::oldKeys[256] = { 0 };
IDirectInputDevice8* Input::devkeyboard = nullptr;

Input::Input()
{
	devkeyboard = nullptr;
}

void Input::Init(HWND hwnd)
{
	HRESULT result;

	IDirectInput8* dinput = nullptr;
	result = DirectInput8Create(
		GetModuleHandle(nullptr),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&dinput,
		nullptr);

	//�L�[�{�[�h�f�o�C�X�̐���
	result = dinput->CreateDevice(
		GUID_SysKeyboard,
		&devkeyboard,
		NULL);

	//���̓f�[�^�`���̃Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);

	//�r���I���䃌�x���̃Z�b�g
	result = devkeyboard->SetCooperativeLevel(
		hwnd,
		//��ʂ���O�ɂ���ꍇ�̂ݓ��͂��󂯕t����
		DISCL_FOREGROUND |
		//�f�o�C�X�����̃A�v�������Ő�L���Ȃ�
		DISCL_NONEXCLUSIVE |
		//Windows�L�[�𖳌��ɂ���
		DISCL_NOWINKEY);
}

void Input::Update()
{
	HRESULT result;

	result = devkeyboard->Acquire();

	//�O�t���[���̃L�[����ۑ�����B
	for (int i = 0; i < 256; i++) {
		oldKeys[i] = keys[i];
	}

	//�S�L�[�̓��͏�Ԃ��擾����
	result = devkeyboard->GetDeviceState(sizeof(keys), keys);
}

bool Input::isKey(int key)
{
	return keys[key];
}

bool Input::isKeyTrigger(int key)
{
	return keys[key] && !oldKeys[key];
}

bool Input::isKeyRelease(int key)
{
	return !keys[key] && oldKeys[key];
}
