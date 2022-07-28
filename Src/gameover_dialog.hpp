#pragma once
#include "eagle.hpp"

class BoxDrawer;
class Text;
class Button;

class GameOverDialog : public Eagle::Behavior {
private:
	struct ButtonList {
		Button* retry = nullptr;
		Button* quit = nullptr;

		void SetEnable(bool flag);
	};
public:
	GameOverDialog();

private:
	BoxDrawer* dialogBox;
	Text* titleText;
	Eagle::FixedArray<Text*, 2> scoreText;
	ButtonList button;

	void Awake()override;
	void Start()override;
	void OnEnable()override;
	void OnDisable()override;
};