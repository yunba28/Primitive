#pragma once
#include "enemy_act_base.hpp"

class Interval;

class EnemyActCurve : public EnemyActBase {
public:
	EnemyActCurve();
	~EnemyActCurve();

	void OnShootDown()override;

private:
	Eagle::FixedArray<Eagle::Vector3, 4> point;
	UNIQUE(Interval) shotInterval;
	float deltaSum;
	float timeFuctor;

	void Start()override;
	void Update()override;

	void MakePointFromTop();
	void MakePointFromLeft();
	void MakePointFromRight();
};