#include "key_config_input.hpp"
#include "text.hpp"
#include "button_widget.hpp"

KeyConfigInput::KeyConfigInput(Input::Key initialKey)
	: Behavior(ID::LateUpdate)
	, currentText(nullptr)
	, output([](Input::Key key) {(void)key; })
	, currentKey(initialKey)
	, widget(nullptr)
{
}

KeyConfigInput::~KeyConfigInput()
{
}

void KeyConfigInput::SetOutputFunction(std::function<void(Input::Key)> _output)
{
	output = _output;
}

void KeyConfigInput::SetDisabledColor(Eagle::Color _color)
{
	color.disabled = _color;
	currentText->SetColor(color.disabled);
}

void KeyConfigInput::SetEnabledColor(Eagle::Color _color)
{
	color.enabled = _color;
	currentText->SetColor(color.enabled);
}

void KeyConfigInput::SetWidget(ButtonWidget* _widget)
{
	widget = _widget;
}

Text* KeyConfigInput::GetText() const noexcept
{
	return currentText;
}

Input::Key KeyConfigInput::GetCurrentKey() const noexcept
{
	return currentKey;
}

void KeyConfigInput::Awake()
{
	currentText = GetGameObject()->AttachComponent<Text>(Input::GetKeyName(currentKey));
	currentText->SetFont("Consolas", 24);
}

void KeyConfigInput::Start()
{
	SetEnable(false);
}

void KeyConfigInput::LateUpdate()
{
	Input::Key input = Input::GetPressedKey();

	if (widget)
	{
		widget->SetEnable(false);
	}

	if (input == Input::Key::None)
		return;

	if (widget)
	{
		widget->SetEnable(true);
	}

	currentKey = input;
	output(currentKey);
	currentText->SetText(Input::GetKeyName(currentKey));
	SetEnable(false);
}

void KeyConfigInput::OnEnable()
{
	currentText->DoColor(color.enabled, 0.1f);
}

void KeyConfigInput::OnDisable()
{
	currentText->DoColor(color.disabled, 0.1f);
}
