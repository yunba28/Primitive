#pragma once
#include "game_utility.hpp"

//[summary]
//Title�V�[���̏������s���R���|�[�l���g
class TitleProcessor final : public Eagle::Behavior {
public:
	TitleProcessor() :Behavior(ID::NoBehave) {};
private:
	void Awake()override final;
	void Start()override final;
};