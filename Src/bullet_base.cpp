#include "bullet_base.hpp"
#include "bullet_commander.hpp"

BulletBase::BulletBase()
	: Behavior()
	, bulletCommander(0)
	, velocity(Eagle::Vector3::up)
{
}

void BulletBase::OnDestroy()
{
	bulletCommander->OnRemove(this);
}

Eagle::Vector3 BulletBase::GetVelocity() const noexcept
{
	return velocity;
}

void BulletBase::SetVelocity(const Eagle::Vector3& _velocity) noexcept
{
	velocity = _velocity;
}
