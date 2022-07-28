#pragma once
#include "game_utility.hpp"

class Button;
class Text;

//[summary]
//Titleシーンの処理を行うコンポーネント
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