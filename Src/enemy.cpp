#include "enemy.hpp"
#include "enemy_spawner.hpp"
#include "drawer.hpp"
#include "enemy_act_base.hpp"
#include "effect_bomb.hpp"

Enemy::Enemy()
	: Behavior(ID::Update)
	, PauseAndResume()
	, enemySpawner(nullptr)
	, enemyMove(nullptr)
	, armor(1)
{
}

Enemy::~Enemy()
{
}

void Enemy::Pause()
{
	enemyMove->SetEnable(false);
	SetEnable(false);
}

void Enemy::Resume()
{
	enemyMove->SetEnable(true);
	SetEnable(true);
}

void Enemy::SetArmor(int _armor) noexcept
{
	armor = _armor;
}

void Enemy::SetActor(EnemyActBase* actor) noexcept
{
	enemyMove = actor;
}

int Enemy::GetArmor() const noexcept
{
	return armor;
}

Enemy* Enemy::Create(EnemySpawner* enemySpawner, float radius, Eagle::Color color, float shotInterval)
{
	auto enemyObj = Eagle::GameObject::Create();
	auto enemyCmt = enemyObj->AttachComponent<Enemy>();
	enemyCmt->enemySpawner = enemySpawner;
	enemyObj->AttachComponent<CircleDrawer>(radius, color);

	enemyObj->AttachComponent<Eagle::CircleCollider2D>(radius);

	enemyObj->SetTag("Enemy");

	return enemyCmt;
}

void Enemy::Update()
{
	if (enemyMove->IsMoveEnd())
	{
		enemySpawner->OnDeath(this);
	}
}

void Enemy::OnCollisionEnter(const Eagle::Collision& collision)
{
	armor -= 1;

	if (armor > 0)
		return;

	enemyMove->OnShootDown();

	enemySpawner->OnDeath(this);

	Pause();

	SetEnable(false);
}