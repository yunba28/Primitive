#pragma once
#include "eagle.hpp"

class PlayerMove;
class PlayerShot;
class CircleDrawer;
class BoxDrawer;
class PlayerArmorGauge;
class ControlSlowTime;
class Gauge;
class Interval;

//[summary]
//プレイヤーに関するコンポーネントの管理を行う
//主にPause（一時停止）とResume（再開）
//生成にはPlayer::Create関数を使用してください
class Player : public Eagle::Behavior, public Eagle::PauseAndResume {
private:
	using CircleCollider2D = Eagle::CircleCollider2D;
public:
	Player();
	~Player();

	void Pause()override;
	void Resume()override;

	void OnDamage(float damage = 10.f);

	//1.fで被弾を100%カット
	void SetDamageCutRate(float _cutRate);

	CircleDrawer* GetCircleDrawer()const noexcept;

	CircleCollider2D* GetCircleCollider()const noexcept;

	float GetArmor()const noexcept;

	static Player* Create(Eagle::String name, float speed, float radius, Eagle::Color color, float shotInterval = 0.1f);

private:
	CircleDrawer* circleDrawer;
	CircleCollider2D* circleCollider;
	ControlSlowTime* controlSlowTime;
	BoxDrawer* damageFrameBox;
	UNIQUE(Interval) damageInterval;
	float armor;
	float damageCutRate;

	void Start()override;
	void Update()override;

	void SetSlowTimeState(float _timeSpeed, float _radiusFactor);
};