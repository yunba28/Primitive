#pragma once
#include "game_utility.hpp"

class Gauge;
class Player;
class CircleDrawer;

class ControlSlowTime : public Eagle::Behavior{
public:
	ControlSlowTime();

	static void SetSlowKey(Input::Key key);

	Gauge* GetGauge()const noexcept;

	bool IsStart()const noexcept;
	bool IsEnd()const noexcept;

private:
	enum class State{
		Neutral = 0b000,
		Down = 0b001,
		Stay = 0b010,
		Up = 0b100,

		Pressed = 0b011
	};

private:
	Gauge* gauge;
	int stateFlag;
	bool canSlowTime;

	static Input::Key actionKey;

	void Awake()override;
	void Update()override;

	bool GetSlowTimePressedAction();
	bool GetSlowTimeLeftAction();
};