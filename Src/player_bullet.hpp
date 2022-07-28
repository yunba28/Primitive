#pragma once
#include "bullet_base.hpp"

class BulletCommander;

//[summary]
//ƒvƒŒƒCƒ„[‚ª”­Ë‚·‚é’e
class PlayerBullet : public BulletBase {
public:
	PlayerBullet();
	PlayerBullet(const Eagle::Vector3& _velocity);
	~PlayerBullet();

	static PlayerBullet* Create(BulletCommander* commander, const Eagle::Vector3& _velocity, float _radius, Eagle::Color _color = 0x0000ff);

private:

	void Start()override {}

	void Update()override;
};