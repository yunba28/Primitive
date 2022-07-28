#include "bullet_commander.hpp"

#include "bullet_base.hpp"
#include "effect_bomb.hpp"

BulletCommander::BulletCommander()
	: Behavior(ID::NoBehave)
	, bulletList()
	, isActive(true)
{
}

BulletCommander::~BulletCommander()
{
}

void BulletCommander::Pause()
{
	if (!isActive)
		return;

	isActive = !isActive;

	for (auto& bullet : bulletList)
	{
		bullet->SetEnable(isActive);
	}
}

void BulletCommander::Resume()
{
	if (isActive)
		return;

	isActive = !isActive;

	for (auto& bullet : bulletList)
	{
		bullet->SetEnable(isActive);
	}
}

void BulletCommander::OnRemove(BulletBase* bullet)
{
	auto itr = std::find(bulletList.begin(), bulletList.end(), bullet);

	if (itr == bulletList.end())
		return;

	(*itr)->SetEnable(false);

	bulletList.Remove(itr);
}

void BulletCommander::AddBullet(BulletBase* bullet)
{
	bulletList.Add(bullet);
}

void BulletCommander::RemoveAll()
{
	for (auto& bullet : bulletList)
	{
		EffectBomb::Create(bullet->GetTransform()->GetWorldPosition(false), 10.f, Eagle::Color::Pallet::Orange);
		bullet->SetEnable(isActive);
		Eagle::GameObject::Destroy(bullet->GetGameObject());
	}
}

void BulletCommander::Awake()
{
	GetGameObject()->SetTag("BulletCommander");
}
