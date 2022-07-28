#pragma once
#include "game_utility.hpp"

//[summary]
//プレイヤーが移動するためのコンポーネント
class PlayerMove : public Eagle::Behavior, public Eagle::PauseAndResume{
public:
	PlayerMove();
	PlayerMove(const float _speed);
	~PlayerMove();

	void Pause()override;
	void Resume()override;

	//プレイヤーが一秒に進む距離
	void SetSpeed(float speed);

	float GetSpeed()const;

	static void SetMoveUpKey(Input::Key _key);
	static void SetMoveDownKey(Input::Key _key);
	static void SetMoveLeftKey(Input::Key _key);
	static void SetMoveRightKey(Input::Key _key);
	static void SetMoveKey(Input::Key _up, Input::Key _down, Input::Key _left, Input::Key _right);

private:
	float speed;

	static Input::Key moveKey[4];

	void Update()override;
};