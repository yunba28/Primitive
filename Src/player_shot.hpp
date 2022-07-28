#pragma once
#include "game_utility.hpp"

class PlayerMove;
class BulletCommander;
class Interval;

//[summary]
//プレイヤーが弾を発射するためのコンポーネント
class PlayerShot : public Eagle::Behavior, public Eagle::PauseAndResume {
public:
	PlayerShot();
	PlayerShot(float _interval, float _bulletSpeed, Eagle::Color _color = 0x0000ff);
	~PlayerShot() = default;

	void Pause()override;
	void Resume()override;

	static void SetShotKey(Input::Key _key);

private:
	BulletCommander* bulletCommander;
	UNIQUE(Interval) interval;
	float bulletSpeed;
	Eagle::Color bulletColor;

	static Input::Key shotKey;

	void Start()override;

	void Update()override;

	static bool GetShotAction()noexcept;
};