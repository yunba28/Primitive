#pragma once
#include "game_utility.hpp"

//[summary]
//Result�V�[���̏������s���R���|�[�l���g
class ResultProcessor final : public Eagle::Behavior {
public:
	ResultProcessor() :Behavior(ID::NoBehave) {};
private:
	void Awake()override final;
	void Start()override final;
};