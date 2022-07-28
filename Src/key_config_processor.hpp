#pragma once
#include "game_utility.hpp"
#include <fstream>

class Button;
class KeyConfigInput;

//[summary]
//KeyConfigシーンの処理を行うコンポーネント
class KeyConfigProcessor final : public Eagle::Behavior {
private:
	struct Node{
		Button* button = nullptr;
		KeyConfigInput* inputer = nullptr;
	};

public:
	KeyConfigProcessor();

private:
	Eagle::FixedArray<Node, 6> config;

	void Awake()override final;
};