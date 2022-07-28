#pragma once
#include "game_utility.hpp"

class Text;
class ButtonWidget;

class KeyConfigInput : public Eagle::Behavior{
public:
	KeyConfigInput(Input::Key initialKey);
	~KeyConfigInput();

	void SetOutputFunction(std::function<void(Input::Key)> _output);
	void SetDisabledColor(Eagle::Color _color);
	void SetEnabledColor(Eagle::Color _color);
	void SetWidget(ButtonWidget* _widget);

	Text* GetText()const noexcept;
	Input::Key GetCurrentKey()const noexcept;

private:
	Text* currentText;
	std::function<void(Input::Key)> output;
	Input::Key currentKey;
	struct {
		Eagle::Color disabled = 0xffffff;
		Eagle::Color enabled = 0xff0000;
	}color;
	ButtonWidget* widget;

	void Awake()override;
	void Start()override;
	void LateUpdate()override;
	void OnEnable()override;
	void OnDisable()override;
};