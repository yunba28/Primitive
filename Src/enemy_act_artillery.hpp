#pragma once
#include "enemy_act_base.hpp"

class Interval;
class BoxDrawer;
class CircleDrawer;
class CapsuleDrawer;
class Enemy;
class Player;
class ScreenShaker;

namespace Internal {
	class MirrorEnemy;
	class ArtilleryBit;
}

class EnemyActArtillery : public EnemyActBase {
public:
	EnemyActArtillery();
	~EnemyActArtillery();

	void OnShootDown()override;

private:
	UNIQUE(Interval) openInterval;
	UNIQUE(Interval) beamInterval;
	Eagle::FixedArray<Internal::MirrorEnemy*, 2> gardMirror;
	Eagle::FixedArray<Internal::ArtilleryBit*, 2> gardBit;
	CircleDrawer* circleDrawer;
	Enemy* myEnemy;
	Player* player;
	Eagle::TweenerCore<Eagle::MoveAttribute>* callbackTween;
	int intervalFlag;

	void Awake()override;
	void Start()override;
	void Update()override;
	void OnCollisionEnter(const Eagle::Collision& collision)override;

	void LastPhase();
};