#pragma once
#include "game_utility.hpp"

class Button;
class Text;

//[summary]
//Title�V�[���̏������s���R���|�[�l���g
class LevelProcessor final : public Eagle::Behavior {
public:
	LevelProcessor() :Behavior(ID::Update), levelButton{}, debugModeText(nullptr), isDebugMode(false) {};
private:
	Eagle::FixedArray<Button*, 3> levelButton;
	Text* debugModeText;
	bool isDebugMode;

	void Awake()override final;
	void Start()override final;
	void Update()override final;
};