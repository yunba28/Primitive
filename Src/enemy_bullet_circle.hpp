#pragma once
#include "bullet_base.hpp"

class Player;

class CircleDrawer;

class EnemyBulletCircle : public BulletBase {
public:
	EnemyBulletCircle();
	EnemyBulletCircle(const Eagle::Vector3& _velocity);
	~EnemyBulletCircle();

	static EnemyBulletCircle* Create(
		BulletCommander* commander,
		const Eagle::Vector3& velocity,
		Eagle::Color color,
		float radius
	);

	CircleDrawer* GetCircleDrawer()const noexcept;

private:
	Player* player;
	CircleDrawer* circleDrawer;
	Eagle::CircleCollider2D* circleCollider;

	void Start()override;
	void Update()override;
};