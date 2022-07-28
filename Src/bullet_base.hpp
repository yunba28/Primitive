#pragma once
#include "game_utility.hpp"

class BulletCommander;

class BulletBase : public Eagle::Behavior {
public:
	BulletBase();
	virtual ~BulletBase() = default;

protected:

	virtual void Start() = 0;

	virtual void Update() = 0;

	virtual void OnDestroy()override;

	Eagle::Vector3 GetVelocity()const noexcept;
	void SetVelocity(const Eagle::Vector3& _velocity)noexcept;

protected:
	BulletCommander* bulletCommander;
	Eagle::Vector3 velocity;
};