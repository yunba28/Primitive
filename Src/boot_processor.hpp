#pragma once
#include "game_utility.hpp"

class Text;
class BoxDrawer;

//[summary]
//Boot�V�[���̏������s���R���|�[�l���g
class BootProcessor final : public Eagle::Behavior {
public:
	BootProcessor() :Behavior(ID::NoBehave), titleText(nullptr), fadeBox() {}
private:
	Text* titleText;
	Eagle::FixedArray<BoxDrawer*, 80> fadeBox;

	void Awake()override final;
	void Start()override final;
};