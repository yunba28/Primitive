#pragma once
#include "eagle.hpp"

class Button;

enum class WidgetDirection {
	Horizontal,
	Vertical
};

class ButtonWidget : public Eagle::Behavior {
public:
	ButtonWidget();
	~ButtonWidget();

	void Add(Button* _button);
	void Remove(int idx);

	void SetListDirection(WidgetDirection _direction);

private:
	Eagle::List<Button*> button;
	Button* current;
	Eagle::Vector2 oldMousePos;
	WidgetDirection direction;
	int currentIdx;

	void Update()override;
	bool VerticalUpdate();
	bool HorizontalUpdate();
};