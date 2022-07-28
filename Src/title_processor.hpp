#pragma once
#include "game_utility.hpp"

//[summary]
//Titleシーンの処理を行うコンポーネント
class TitleProcessor final : public Eagle::Behavior {
public:
	TitleProcessor() :Behavior(ID::NoBehave) {};
private:
	void Awake()override final;
	void Start()override final;
};