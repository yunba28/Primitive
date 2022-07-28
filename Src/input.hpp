#pragma once
#include <DxLib.h>
#include <eagle.hpp>

class GameConfig;

//[summary]
//���͊֌W�̃N���X
class Input {
private:
	Input();
	
	static Input* Inst();

public:
	~Input();

	Input(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator=(const Input&) = delete;
	Input& operator=(Input&&) = delete;

	enum class Key : int {
		None = -999,
		A = KEY_INPUT_A,
		B = KEY_INPUT_B,
		C = KEY_INPUT_C,
		D = KEY_INPUT_D,
		E = KEY_INPUT_E,
		F = KEY_INPUT_F,
		G = KEY_INPUT_G,
		H = KEY_INPUT_H,
		I = KEY_INPUT_I,
		J = KEY_INPUT_J,
		K = KEY_INPUT_K,
		L = KEY_INPUT_L,
		M = KEY_INPUT_M,
		N = KEY_INPUT_N,
		O = KEY_INPUT_O,
		P = KEY_INPUT_P,
		Q = KEY_INPUT_Q,
		R = KEY_INPUT_R,
		S = KEY_INPUT_S,
		T = KEY_INPUT_T,
		U = KEY_INPUT_U,
		V = KEY_INPUT_V,
		W = KEY_INPUT_W,
		X = KEY_INPUT_X,
		Y = KEY_INPUT_Y,
		Z = KEY_INPUT_Z,

		N1 = KEY_INPUT_1,
		N2 = KEY_INPUT_2,
		N3 = KEY_INPUT_3,
		N4 = KEY_INPUT_4,
		N5 = KEY_INPUT_5,
		N6 = KEY_INPUT_6,
		N7 = KEY_INPUT_7,
		N8 = KEY_INPUT_8,
		N9 = KEY_INPUT_9,
		N0 = KEY_INPUT_0,

		F1 = KEY_INPUT_F1,
		F2 = KEY_INPUT_F2,
		F3 = KEY_INPUT_F3,
		F4 = KEY_INPUT_F4,
		F5 = KEY_INPUT_F5,
		F6 = KEY_INPUT_F6,
		F7 = KEY_INPUT_F7,
		F8 = KEY_INPUT_F8,
		F9 = KEY_INPUT_F9,
		F10 = KEY_INPUT_F10,
		F11 = KEY_INPUT_F11,
		F12 = KEY_INPUT_F12,

		Escape = KEY_INPUT_ESCAPE,
		Return = KEY_INPUT_RETURN, //Enter
		End = KEY_INPUT_END,
		Delete = KEY_INPUT_DELETE,
		Space = KEY_INPUT_SPACE,
		BackSpace = KEY_INPUT_BACK,
		LeftCtrl = KEY_INPUT_LCONTROL,
		RightCtrl = KEY_INPUT_RCONTROL,
		LeftShift = KEY_INPUT_LSHIFT,
		RightShift = KEY_INPUT_RSHIFT,
		LeftAlt = KEY_INPUT_LALT,
		RightAlt = KEY_INPUT_RALT,

		Up = KEY_INPUT_UP,
		Down = KEY_INPUT_DOWN,
		Left = KEY_INPUT_LEFT,
		Right = KEY_INPUT_RIGHT
	};
	
	enum class Mouse : int {
		None = -999,
		Left = MOUSE_INPUT_LEFT,
		Right = MOUSE_INPUT_RIGHT,
		Middle = MOUSE_INPUT_MIDDLE,
		Button4 = MOUSE_INPUT_4,
		Button5 = MOUSE_INPUT_5
	};

	enum class Pad : int {
		None = -999,
		DigitalUp = XINPUT_BUTTON_DPAD_UP,
		DigitalDown = XINPUT_BUTTON_DPAD_DOWN,
		DigitalLeft = XINPUT_BUTTON_DPAD_LEFT,
		DigitalRight = XINPUT_BUTTON_DPAD_RIGHT,

		Start = XINPUT_BUTTON_START,
		Back = XINPUT_BUTTON_BACK,

		LeftThumb = XINPUT_BUTTON_LEFT_THUMB,
		RightThumb = XINPUT_BUTTON_RIGHT_THUMB,
		LeftShoulder = XINPUT_BUTTON_LEFT_SHOULDER,
		RightShoulder = XINPUT_BUTTON_RIGHT_SHOULDER,

		A = XINPUT_BUTTON_A,
		B = XINPUT_BUTTON_B,
		X = XINPUT_BUTTON_X,
		Y = XINPUT_BUTTON_Y,

		LeftThumbUp = 16,
		LeftThumbDown = 17,
		LeftThumbLeft = 18,
		LeftThumbRight = 19,
		RightThumbUp = 20,
		RightThumbDown = 21,
		RightThumbLeft = 22,
		RightThumbRight = 23,
	};

public:
	static bool GetDown(Key val); //�L�[�{�[�h�̉������u�Ԃ̔�����擾����
	static bool GetDown(Mouse val); //�}�E�X�{�^���̉������u�Ԃ̔�����擾����
	static bool GetDown(Pad val); //�p�b�h�{�^���̉������u�Ԃ̔�����擾����

	static bool GetStay(Key val); //�L�[�{�[�h�̉���������擾����
	static bool GetStay(Mouse val); //�}�E�X�{�^���̉���������擾����
	static bool GetStay(Pad val); //�p�b�h�{�^���̉���������擾����

	static bool GetUp(Key val); //�L�[�{�[�h�̗�����������擾����
	static bool GetUp(Mouse val); //�}�E�X�{�^���̗�����������擾����
	static bool GetUp(Pad val); //�p�b�h�{�^���̗�����������擾����

	static Eagle::String GetKeyName(Key val)noexcept;
	static Eagle::String GetKeyName(Mouse val)noexcept;
	static Eagle::String GetKeyName(Pad val)noexcept;

	static Key GetPressedKey()noexcept;
	static Mouse GetPressedMouse()noexcept;
	static Pad GetPressedPad()noexcept;

	//�}�E�X�J�[�\���̍��W���擾
	static Eagle::Vector2 GetMousePosition();

	static Eagle::Vector2 GetPadLeftThumb(); //�p�b�h�̍��X�e�B�b�N�̕������擾
	static Eagle::Vector2 GetPadRightThumb(); //�p�b�h�̉E�X�e�B�b�N�̕������擾
	static float GetPadLeftTrigger(); //�p�b�h�̍��g���K�[�̓��͒l�𓾂�
	static float GetPadRightTrigger(); //�p�b�h�̉E�g���K�[�̓��͒l�𓾂�

	static void CheckConnectingPad(); //�p�b�h�̐ڑ��󋵂��`�F�b�N����
	static bool ConnectedPad(); //�p�b�h���ڑ�����Ă��邩

private:
	friend class GameConfig;
	friend void MainProc(Eagle::Scene* _scene);

	static void Start();
	static void End();
	static void Update();
};