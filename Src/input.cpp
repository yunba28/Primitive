#include "input.hpp"

enum class InputState {
	Neutral = 0b000,
	Down    = 0b001,
	Stay    = 0b010,
	Up      = 0b100
};

InputState operator | (const InputState a, const InputState b)noexcept
{
	return InputState((int)a | (int)b);
}

InputState operator & (const InputState a, const InputState b)noexcept
{
	return InputState((int)a & (int)b);
}

//enum classと整数値をキャストするための関数
template<typename Int,typename Enum>
Int ECast(Enum val)
{
	return static_cast<Int>(val);
}

//InputStateの状態が押下であるか
bool IsPressed(InputState state)
{
	return ECast<unsigned>(state) & (ECast<unsigned>(InputState::Down) | ECast<unsigned>(InputState::Stay));
}

//ボタンが押されたときの処理
void DownSequence(InputState& state)
{
	state = IsPressed(state) ? InputState::Stay : InputState::Down;
}

//ボタンが話されたときの処理
void UpSequence(InputState& state)
{
	state = IsPressed(state) ? InputState::Up : InputState::Neutral;
}

Input::Input()
{

}

Input* Input::Inst()
{
	static Input input;
	return &input;
}

Input::~Input()
{

}

//[summary]
//キーボードの入力情報を保持、更新する
class KeyState {
public:
	KeyState() :status() { Awake(); }
	~KeyState() = default;

	void Awake();
	void Update();

	InputState GetState(Input::Key code)const noexcept;

	const std::unordered_map<Input::Key, InputState>& GetStateMap()const noexcept;

private:
	std::unordered_map<Input::Key, InputState> status;

	bool ExistCode(unsigned codeNum);
};

void KeyState::Awake()
{
	status = {
		{Input::Key::A,InputState::Neutral},
		{Input::Key::B,InputState::Neutral},
		{Input::Key::C,InputState::Neutral},
		{Input::Key::D,InputState::Neutral},
		{Input::Key::E,InputState::Neutral},
		{Input::Key::F,InputState::Neutral},
		{Input::Key::G,InputState::Neutral},
		{Input::Key::H,InputState::Neutral},
		{Input::Key::I,InputState::Neutral},
		{Input::Key::J,InputState::Neutral},
		{Input::Key::K,InputState::Neutral},
		{Input::Key::L,InputState::Neutral},
		{Input::Key::M,InputState::Neutral},
		{Input::Key::N,InputState::Neutral},
		{Input::Key::O,InputState::Neutral},
		{Input::Key::P,InputState::Neutral},
		{Input::Key::Q,InputState::Neutral},
		{Input::Key::R,InputState::Neutral},
		{Input::Key::S,InputState::Neutral},
		{Input::Key::T,InputState::Neutral},
		{Input::Key::U,InputState::Neutral},
		{Input::Key::V,InputState::Neutral},
		{Input::Key::W,InputState::Neutral},
		{Input::Key::X,InputState::Neutral},
		{Input::Key::Y,InputState::Neutral},
		{Input::Key::Z,InputState::Neutral},

		{Input::Key::N1,InputState::Neutral},
		{Input::Key::N2,InputState::Neutral},
		{Input::Key::N3,InputState::Neutral},
		{Input::Key::N4,InputState::Neutral},
		{Input::Key::N5,InputState::Neutral},
		{Input::Key::N6,InputState::Neutral},
		{Input::Key::N7,InputState::Neutral},
		{Input::Key::N8,InputState::Neutral},
		{Input::Key::N9,InputState::Neutral},
		{Input::Key::N0,InputState::Neutral},

		{Input::Key::F1,InputState::Neutral},
		{Input::Key::F2,InputState::Neutral},
		{Input::Key::F3,InputState::Neutral},
		{Input::Key::F4,InputState::Neutral},
		{Input::Key::F5,InputState::Neutral},
		{Input::Key::F6,InputState::Neutral},
		{Input::Key::F7,InputState::Neutral},
		{Input::Key::F8,InputState::Neutral},
		{Input::Key::F9,InputState::Neutral},
		{Input::Key::F10,InputState::Neutral},
		{Input::Key::F11,InputState::Neutral},
		{Input::Key::F12,InputState::Neutral},

		{Input::Key::Escape,InputState::Neutral},
		{Input::Key::Return,InputState::Neutral},
		{Input::Key::End,InputState::Neutral},
		{Input::Key::Delete,InputState::Neutral},
		{Input::Key::Space,InputState::Neutral},
		{Input::Key::BackSpace,InputState::Neutral},
		{Input::Key::LeftCtrl,InputState::Neutral},
		{Input::Key::RightCtrl,InputState::Neutral},
		{Input::Key::LeftShift,InputState::Neutral},
		{Input::Key::RightShift,InputState::Neutral},
		{Input::Key::LeftAlt,InputState::Neutral},
		{Input::Key::RightAlt,InputState::Neutral},

		{Input::Key::Up,InputState::Neutral},
		{Input::Key::Down,InputState::Neutral},
		{Input::Key::Left,InputState::Neutral},
		{Input::Key::Right,InputState::Neutral}
	};
}

void KeyState::Update()
{
	constexpr unsigned KeyCodeMax = 256;

	char keyBuf[KeyCodeMax]{};

	DxLib::GetHitKeyStateAll(keyBuf);

	for (unsigned idx = 0;idx < KeyCodeMax; ++idx)
	{
		if (!ExistCode(idx))
			continue;

		(keyBuf[idx] == 1) 
			? DownSequence(status[ECast<Input::Key>(idx)])
			: UpSequence(status[ECast<Input::Key>(idx)]);
	}
}

InputState KeyState::GetState(Input::Key code) const noexcept
{
	return status.at(code);
}

const std::unordered_map<Input::Key, InputState>& KeyState::GetStateMap() const noexcept
{
	return status;
}

bool KeyState::ExistCode(unsigned codeNum)
{
	return status.find(ECast<Input::Key>(codeNum)) != status.end();
}

//[summary]
//マウスの入力情報を保持、更新する
class MouseState {
public:
	MouseState() :status(), position() { Awake(); }
	~MouseState() = default;

	void Awake();
	void Update();

	InputState GetState(Input::Mouse code)const noexcept;

	const std::unordered_map<Input::Mouse, InputState>& GetStateMap()const noexcept;

	Eagle::Vector2 GetPosition()const noexcept;

private:
	std::unordered_map<Input::Mouse, InputState> status;
	Eagle::Vector2 position;

	//定義は残すがMouseでは使わない
	bool ExistCode(unsigned codeNum);
};

void MouseState::Awake()
{
	status = {
		{Input::Mouse::Left,InputState::Neutral},
		{Input::Mouse::Right,InputState::Neutral},
		{Input::Mouse::Middle,InputState::Neutral},
		{Input::Mouse::Button4,InputState::Neutral},
		{Input::Mouse::Button5,InputState::Neutral}
	};
}

void MouseState::Update()
{
	//本来は８まであるがこのゲームではButton5までしか扱わないため
	//最高値は0b10000
	constexpr unsigned MouseFlagsMax = 5;

	int input = GetMouseInput();

	unsigned code = 0;

	for (unsigned idx = 0;idx < MouseFlagsMax;++idx)
	{
		code = 1 << idx;
		(input & code) ? DownSequence(status[ECast<Input::Mouse>(code)]) : UpSequence(status[ECast<Input::Mouse>(code)]);
	}

	position = [] {
		Eagle::Int2 mPos(0, 0);
		GetMousePoint(&mPos.x, &mPos.y);
		return Eagle::Vector2(mPos.x, mPos.y);
	}();
}

InputState MouseState::GetState(Input::Mouse code) const noexcept
{
	return status.at(code);
}

const std::unordered_map<Input::Mouse, InputState>& MouseState::GetStateMap() const noexcept
{
	return status;
}

Eagle::Vector2 MouseState::GetPosition() const noexcept
{
	return position;
}

bool MouseState::ExistCode(unsigned codeNum)
{
	return status.find(ECast<Input::Mouse>(codeNum)) != status.end();
}

//[summary]
//パッドの入力情報を保持、更新する
class PadState {
public:
	PadState() :status(), inputState(), deadZone(0.25f), trigger(), leftStick(), rightStick(), connectedPad(false) { Awake(); }
	~PadState() = default;

	void Awake();
	void Update();

	InputState GetState(Input::Pad code)const;

	const std::unordered_map<Input::Pad, InputState>& GetStateMap()const noexcept;

	Eagle::Vector2 GetLStick()const;
	Eagle::Vector2 GetRStick()const;
	float GetLTrigger()const;
	float GetRTrigger()const;

	void CheckConnectingPad();
	bool ConnectedPad()const;

private:
	std::unordered_map<Input::Pad, InputState> status;
	XINPUT_STATE inputState;
	float deadZone;
	Eagle::Float2 trigger;
	Eagle::Float2 leftStick;
	Eagle::Float2 rightStick;
	bool connectedPad;

	bool ExistCode(unsigned codeNum);
};

void PadState::Awake()
{
	status = {
		{Input::Pad::DigitalUp,InputState::Neutral},
		{Input::Pad::DigitalDown,InputState::Neutral},
		{Input::Pad::DigitalLeft,InputState::Neutral},
		{Input::Pad::DigitalRight,InputState::Neutral},

		{Input::Pad::Start,InputState::Neutral},
		{Input::Pad::Back,InputState::Neutral},

		{Input::Pad::LeftThumb,InputState::Neutral},
		{Input::Pad::RightThumb,InputState::Neutral},
		{Input::Pad::LeftShoulder,InputState::Neutral},
		{Input::Pad::RightShoulder,InputState::Neutral},

		{Input::Pad::A,InputState::Neutral},
		{Input::Pad::B,InputState::Neutral},
		{Input::Pad::X,InputState::Neutral},
		{Input::Pad::Y,InputState::Neutral},

		{Input::Pad::LeftThumbUp,InputState::Neutral},
		{Input::Pad::LeftThumbDown,InputState::Neutral},
		{Input::Pad::LeftThumbLeft,InputState::Neutral},
		{Input::Pad::LeftThumbRight,InputState::Neutral},
		{Input::Pad::RightThumbUp,InputState::Neutral},
		{Input::Pad::RightThumbDown,InputState::Neutral},
		{Input::Pad::RightThumbLeft,InputState::Neutral},
		{Input::Pad::RightThumbRight,InputState::Neutral}
	};

	connectedPad = (DxLib::GetJoypadNum() > 0);
}

void PadState::Update()
{
	if (GetJoypadNum() == 0)
		return;

	GetJoypadXInputState(DX_INPUT_PAD1, &inputState);

	constexpr unsigned PadButtonMax = 16;

	for (unsigned idx = 0;idx < PadButtonMax;++idx)
	{
		if (!ExistCode(idx))
			continue;

		(inputState.Buttons[idx] == 1U)
			? DownSequence(status[ECast<Input::Pad>(idx)])
			: UpSequence(status[ECast<Input::Pad>(idx)]);
	}

	auto adjust = [&](float value) {
		const bool sign = Eagle::Math::Signe(value);
		value = (Eagle::Math::Abs(value) <= deadZone) ? 0.f : Eagle::Math::Ceiling(value, 1.f);
		if (!sign)value *= -1.f;
		return value;
	};

	trigger.Set(
		adjust(static_cast<float>(inputState.LeftTrigger) * Eagle::Math::RECIPRO_T<unsigned char, float>),
		adjust(static_cast<float>(inputState.RightTrigger) * Eagle::Math::RECIPRO_T<unsigned char, float>)
	);

	leftStick.Set(
		adjust(static_cast<float>(inputState.ThumbLX) * Eagle::Math::RECIPRO_T<short, float>),
		adjust(static_cast<float>(inputState.ThumbLY) * Eagle::Math::RECIPRO_T<short, float>)
	);

	if (leftStick.x > 0.f)
	{
		DownSequence(status[Input::Pad::LeftThumbRight]);
		UpSequence(status[Input::Pad::LeftThumbLeft]);
	}
	else if (leftStick.x < 0.f)
	{
		DownSequence(status[Input::Pad::LeftThumbLeft]);
		UpSequence(status[Input::Pad::LeftThumbRight]);
	}

	if (leftStick.y > 0.f)
	{
		DownSequence(status[Input::Pad::LeftThumbDown]);
		UpSequence(status[Input::Pad::LeftThumbUp]);
	}
	else if (leftStick.y < 0.f)
	{
		DownSequence(status[Input::Pad::LeftThumbUp]);
		UpSequence(status[Input::Pad::LeftThumbDown]);
	}

	rightStick.Set(
		adjust(static_cast<float>(inputState.ThumbRX) * Eagle::Math::RECIPRO_T<short, float>),
		adjust(static_cast<float>(inputState.ThumbRY) * Eagle::Math::RECIPRO_T<short, float>)
	);

	if (rightStick.x > 0.f)
	{
		DownSequence(status[Input::Pad::RightThumbRight]);
		UpSequence(status[Input::Pad::RightThumbLeft]);
	}
	else if (rightStick.x < 0.f)
	{
		DownSequence(status[Input::Pad::RightThumbLeft]);
		UpSequence(status[Input::Pad::RightThumbRight]);
	}

	if (rightStick.y > 0.f)
	{
		DownSequence(status[Input::Pad::RightThumbDown]);
		UpSequence(status[Input::Pad::RightThumbUp]);
	}
	else if (rightStick.y < 0.f)
	{
		DownSequence(status[Input::Pad::RightThumbDown]);
		UpSequence(status[Input::Pad::RightThumbUp]);
	}
}

InputState PadState::GetState(Input::Pad code) const
{
	return status.at(code);
}

const std::unordered_map<Input::Pad, InputState>& PadState::GetStateMap() const noexcept
{
	return status;
}

Eagle::Vector2 PadState::GetLStick() const
{
	return Eagle::Vector2(leftStick);
}

Eagle::Vector2 PadState::GetRStick() const
{
	return Eagle::Vector2(rightStick);
}

float PadState::GetLTrigger() const
{
	return trigger.x;
}

float PadState::GetRTrigger() const
{
	return trigger.y;
}

void PadState::CheckConnectingPad()
{
	DxLib::ReSetupJoypad();
	connectedPad = (DxLib::GetJoypadNum() > 0);
}

bool PadState::ConnectedPad() const
{
	return DxLib::GetJoypadNum() > 0;
}

bool PadState::ExistCode(unsigned codeNum)
{
	return status.find(ECast<Input::Pad>(codeNum)) != status.end();;
}

static KeyState* keyState = nullptr;
static MouseState* mouseState = nullptr;
static PadState* padState = nullptr;

bool Input::GetDown(Key val)
{
	return keyState->GetState(val) == InputState::Down;
}

bool Input::GetDown(Mouse val)
{
	return mouseState->GetState(val) == InputState::Down;
}

bool Input::GetDown(Pad val)
{
	if (!padState)
		return false;
	return padState->GetState(val) == InputState::Down;
}

bool Input::GetStay(Key val)
{
	return keyState->GetState(val) == InputState::Stay;
}

bool Input::GetStay(Mouse val)
{
	return mouseState->GetState(val) == InputState::Stay;
}

bool Input::GetStay(Pad val)
{
	if (!padState)
		return false;
	return padState->GetState(val) == InputState::Stay;
}

bool Input::GetUp(Key val)
{
	return keyState->GetState(val) == InputState::Up;
}

bool Input::GetUp(Mouse val)
{
	return mouseState->GetState(val) == InputState::Up;
}

bool Input::GetUp(Pad val)
{
	if (!padState)
		return false;
	return padState->GetState(val) == InputState::Up;
}

Eagle::String Input::GetKeyName(Key val) noexcept
{
	switch (val)
	{
	case Input::Key::None:
		return "None";
		break;
	case Input::Key::A:
		return "A";
		break;
	case Input::Key::B:
		return "B";
		break;
	case Input::Key::C:
		return "C";
		break;
	case Input::Key::D:
		return "D";
		break;
	case Input::Key::E:
		return "E";
		break;
	case Input::Key::F:
		return "F";
		break;
	case Input::Key::G:
		return "G";
		break;
	case Input::Key::H:
		return "H";
		break;
	case Input::Key::I:
		return "I";
		break;
	case Input::Key::J:
		return "J";
		break;
	case Input::Key::K:
		return "K";
		break;
	case Input::Key::L:
		return "L";
		break;
	case Input::Key::M:
		return "M";
		break;
	case Input::Key::N:
		return "N";
		break;
	case Input::Key::O:
		return "O";
		break;
	case Input::Key::P:
		return "P";
		break;
	case Input::Key::Q:
		return "Q";
		break;
	case Input::Key::R:
		return "R";
		break;
	case Input::Key::S:
		return "S";
		break;
	case Input::Key::T:
		return "T";
		break;
	case Input::Key::U:
		return "U";
		break;
	case Input::Key::V:
		return "V";
		break;
	case Input::Key::W:
		return "W";
		break;
	case Input::Key::X:
		return "X";
		break;
	case Input::Key::Y:
		return "Y";
		break;
	case Input::Key::Z:
		return "Z";
		break;
	case Input::Key::N1:
		return "1";
		break;
	case Input::Key::N2:
		return "2";
		break;
	case Input::Key::N3:
		return "3";
		break;
	case Input::Key::N4:
		return "4";
		break;
	case Input::Key::N5:
		return "5";
		break;
	case Input::Key::N6:
		return "6";
		break;
	case Input::Key::N7:
		return "7";
		break;
	case Input::Key::N8:
		return "8";
		break;
	case Input::Key::N9:
		return "9";
		break;
	case Input::Key::N0:
		return "0";
		break;
	case Input::Key::F1:
		return "F1";
		break;
	case Input::Key::F2:
		return "F2";
		break;
	case Input::Key::F3:
		return "F3";
		break;
	case Input::Key::F4:
		return "F4";
		break;
	case Input::Key::F5:
		return "F5";
		break;
	case Input::Key::F6:
		return "F6";
		break;
	case Input::Key::F7:
		return "F7";
		break;
	case Input::Key::F8:
		return "F8";
		break;
	case Input::Key::F9:
		return "F9";
		break;
	case Input::Key::F10:
		return "F10";
		break;
	case Input::Key::F11:
		return "F11";
		break;
	case Input::Key::F12:
		return "F12";
		break;
	case Input::Key::Escape:
		return "Esc";
		break;
	case Input::Key::Return:
		return "Return";
		break;
	case Input::Key::End:
		return "End";
		break;
	case Input::Key::Delete:
		return "Delete";
		break;
	case Input::Key::Space:
		return "Space";
		break;
	case Input::Key::BackSpace:
		return "BackSpace";
		break;
	case Input::Key::LeftCtrl:
		return "LeftCtrl";
		break;
	case Input::Key::RightCtrl:
		return "RightCtrl";
		break;
	case Input::Key::LeftShift:
		return "LeftShift";
		break;
	case Input::Key::RightShift:
		return "RightShift";
		break;
	case Input::Key::LeftAlt:
		return "LeftAlt";
		break;
	case Input::Key::RightAlt:
		return "RightAlt";
		break;
	case Input::Key::Up:
		return "Up";
		break;
	case Input::Key::Down:
		return "Down";
		break;
	case Input::Key::Left:
		return "Left";
		break;
	case Input::Key::Right:
		return "Right";
		break;
	default:
		break;
	}

	return "None";
}

Eagle::String Input::GetKeyName(Mouse val) noexcept
{
	switch (val)
	{
	case Input::Mouse::None:
		return "None";
		break;
	case Input::Mouse::Left:
		return "MLeft";
		break;
	case Input::Mouse::Right:
		return "MRight";
		break;
	case Input::Mouse::Middle:
		return "MMiddle";
		break;
	case Input::Mouse::Button4:
		return "MButton4";
		break;
	case Input::Mouse::Button5:
		return "MButton5";
		break;
	default:
		break;
	}

	return "None";
}

Eagle::String Input::GetKeyName(Pad val) noexcept
{
	switch (val)
	{
	case Input::Pad::None:
		return "None";
		break;
	case Input::Pad::DigitalUp:
		return "PadDUp";
		break;
	case Input::Pad::DigitalDown:
		return "PadDDown";
		break;
	case Input::Pad::DigitalLeft:
		return "PadDLeft";
		break;
	case Input::Pad::DigitalRight:
		return "PadDRight";
		break;
	case Input::Pad::Start:
		return "PadStart";
		break;
	case Input::Pad::Back:
		return "PadBack";
		break;
	case Input::Pad::LeftThumb:
		return "PadLeftThumb";
		break;
	case Input::Pad::RightThumb:
		return "PadRightThumb";
		break;
	case Input::Pad::LeftShoulder:
		return "PadLeftShoulder";
		break;
	case Input::Pad::RightShoulder:
		return "PadRightShoulder";
		break;
	case Input::Pad::A:
		return "PadA";
		break;
	case Input::Pad::B:
		return "PadB";
		break;
	case Input::Pad::X:
		return "PadX";
		break;
	case Input::Pad::Y:
		return "PadY";
		break;
	case Input::Pad::LeftThumbUp:
		return "PadleftThumbUp";
		break;
	case Input::Pad::LeftThumbDown:
		return "PadLeftThumbDown";
		break;
	case Input::Pad::LeftThumbLeft:
		return "PadLeftThumbLeft";
		break;
	case Input::Pad::LeftThumbRight:
		return "PadLeftThumbRight";
		break;
	case Input::Pad::RightThumbUp:
		return "PadRightThumbUp";
		break;
	case Input::Pad::RightThumbDown:
		return "PadRightThumbDown";
		break;
	case Input::Pad::RightThumbLeft:
		return "PadRightThumbLeft";
		break;
	case Input::Pad::RightThumbRight:
		return "PadRightThumbRight";
		break;
	default:
		break;
	}

	return "None";
}

Input::Key Input::GetPressedKey() noexcept
{
	const auto& stMap = keyState->GetStateMap();

	for (const auto& st : stMap)
	{
		if ((bool)(st.second & (InputState::Down | InputState::Stay)))
		{
			return st.first;
		}
	}

	return Key::None;
}

Input::Mouse Input::GetPressedMouse() noexcept
{
	const auto& stMap = mouseState->GetStateMap();

	for (const auto& st : stMap)
	{
		if ((bool)(st.second & (InputState::Down | InputState::Stay)))
		{
			return st.first;
		}
	}

	return Mouse::None;
}

Input::Pad Input::GetPressedPad() noexcept
{
	const auto& stMap = padState->GetStateMap();

	for (const auto& st : stMap)
	{
		if ((bool)(st.second & (InputState::Down | InputState::Stay)))
		{
			return st.first;
		}
	}

	return Pad::None;
}

Eagle::Vector2 Input::GetMousePosition()
{
	return mouseState->GetPosition();
}

Eagle::Vector2 Input::GetPadLeftThumb()
{
	return padState->GetLStick();
}

Eagle::Vector2 Input::GetPadRightThumb()
{
	return padState->GetRStick();
}

float Input::GetPadLeftTrigger()
{
	return padState->GetLTrigger();
}

float Input::GetPadRightTrigger()
{
	return padState->GetRTrigger();
}

void Input::Start()
{
	if (!keyState)
		keyState = new KeyState;

	if (!mouseState)
		mouseState = new MouseState;

	if (!padState)
		padState = new PadState;
}

void Input::End()
{
	if (keyState)
		delete keyState;

	if (mouseState)
		delete mouseState;

	if (padState)
		delete padState;
}

void Input::Update()
{
	//更新時はifチェックをしない

	keyState->Update();
	mouseState->Update();

	padState->Update();
}

void Input::CheckConnectingPad()
{
	padState->CheckConnectingPad();
}

bool Input::ConnectedPad()
{
	return padState->ConnectedPad();
}
