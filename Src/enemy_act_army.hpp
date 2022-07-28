#pragma once
#include "enemy_act_base.hpp"

class Interval;

enum class ArmyType {
	Diffusion, //�f�t�H���g
	Trail //���Ђ�̂悤�Ȓe
};

class EnemyActArmy : public EnemyActBase {
public:
	EnemyActArmy(Eagle::Vector3* _point, float _t, float _duration, ArmyType _type = ArmyType::Diffusion);
	~EnemyActArmy();

	void OnShootDown()override;

private:
	Eagle::FixedArray<Eagle::Vector3, 4> point;
	UNIQUE(Interval) shotInterval;
	ArmyType type;
	float injectionAngle;
	float deltaSum;
	float timeFuctor;

	void Start()override;
	void Update()override;

	void DiffusionShot();
	void TrailShot(Eagle::Vector3 forwardAxis);
};