#pragma once
#include "game_utility.hpp"

class Text;
class BoxText;
class BoxDrawer;

namespace Eagle {
	class Tween;
}

class Button : public Eagle::Behavior {
public:
	Button();
	~Button();

	void SetOnPressed(std::function<void(void)> func);
	void SetStayColor(Eagle::Color _color);
	void SetHoverColor(Eagle::Color _color);
	void SetUseFadeColor(bool flag);
	void SetUseFadeScale(bool flag);

	void OnFocus();
	void OffFocus();

	Text* GetText()const noexcept;
	BoxDrawer* GetBoxDrawer()const noexcept;

private:
	Text* text;
	BoxDrawer* box;
	Eagle::BoxCollider2D* collider;
	std::function<void(void)> onPressed;
	Eagle::Vector2 oldMousePos;
	struct {
		Eagle::Color stay = 0xffffff;
		Eagle::Color hover = 0xff0000;
	}color;
	//0b01 //use fade color
	//0b10 //use fade scale
	int hoverActiveFlag;
	bool isFocus;

	friend Button* MakeButton(
		Eagle::String objectName,
		Eagle::String drawText,
		Eagle::String fontName,
		unsigned int fontSize,
		float boxWidth,
		float boxHeight
	);

	void Awake()override;
	void Start()override;
	void Update()override;

	void OnPress();
};

//Buttonを生成するためのユーティリティ関数
Button* MakeButton(
	Eagle::String objectName,
	Eagle::String drawText,
	Eagle::String fontName,
	unsigned int fontSize,
	float boxWidth,
	float boxHeight
);