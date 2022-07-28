#pragma once
#include "enemy_act_base.hpp"

class EnemyActRain : public EnemyActBase {
public:
	EnemyActRain();
	EnemyActRain(float _speed);
	~EnemyActRain();

	void OnShootDown()override;

private:
	float speed;

	void Start()override;
	void Update()override;

	void MakeDropBullet(const Eagle::Vector3& pos, unsigned int num, float angleMin, float angleMax);

	void MakeRainBullet(const Eagle::Vector3& pos, unsigned int num);
};