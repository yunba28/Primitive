#pragma once
#include "game_utility.hpp"

//[summary]
//Resultシーンの処理を行うコンポーネント
class ResultProcessor final : public Eagle::Behavior {
public:
	ResultProcessor() :Behavior(ID::NoBehave) {};
private:
	void Awake()override final;
	void Start()override final;
};