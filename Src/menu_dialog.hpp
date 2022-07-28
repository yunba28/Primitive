#pragma once
#include "eagle.hpp"

class BoxDrawer;
class Button;
class ButtonWidget;

class MenuDialog : public Eagle::Behavior {
private:
	struct ButtonList{
		Button* close = nullptr;
		Button* toTitle = nullptr;

		void SetEnable(bool flag);
	};
public:
	MenuDialog();

private:
	BoxDrawer* dialogBox;
	ButtonWidget* buttonWidget;
	ButtonList button;
	
	void Awake()override;
	void OnEnable()override;
	void OnDisable()override;
};