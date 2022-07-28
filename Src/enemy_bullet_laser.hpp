#pragma once
#include "bullet_base.hpp"

class Player;

class EnemyBulletLaser : public BulletBase {
public:
	EnemyBulletLaser();
	EnemyBulletLaser(const Eagle::Vector3& _velocity);
	~EnemyBulletLaser();

	static EnemyBulletLaser* Create(
		BulletCommander* commander, 
		const Eagle::Vector3& velocity, 
		Eagle::Color color,
		float laserLength,
		float laserThickness
	);

private:
	Player* player;
	Eagle::CapsuleCollider2D* capsuleCollider;

	void Start()override;
	void Update()override;
};