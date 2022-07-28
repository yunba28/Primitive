#include "enemy_act_base.hpp"

EnemyActBase::EnemyActBase()
	: Behavior()
	, bulletCommander(nullptr)
	, isMoveEnd(false)
{
}

bool EnemyActBase::IsMoveEnd() const noexcept
{
	return isMoveEnd;
}

void EnemyActBase::OnMoveEnd() noexcept
{
	isMoveEnd = true;
}
