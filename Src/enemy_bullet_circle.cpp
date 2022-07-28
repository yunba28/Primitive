#include "enemy_bullet_circle.hpp"
#include "player.hpp"
#include "drawer.hpp"
#include "bullet_commander.hpp"

#include "sound_manager.hpp"

EnemyBulletCircle::EnemyBulletCircle()
	: BulletBase()
	, player(nullptr)
	, circleCollider(nullptr)
	, circleDrawer(nullptr)
{
}

EnemyBulletCircle::EnemyBulletCircle(const Eagle::Vector3& _velocity)
	: BulletBase()
	, player(nullptr)
	, circleCollider(nullptr)
	, circleDrawer(nullptr)
{
	SetVelocity(_velocity);
}

EnemyBulletCircle::~EnemyBulletCircle()
{
}

EnemyBulletCircle* EnemyBulletCircle::Create(BulletCommander* commander, const Eagle::Vector3& velocity, Eagle::Color color, float radius)
{
	auto shotObj = Eagle::GameObject::Create();
	auto shotCmt = shotObj->AttachComponent<EnemyBulletCircle>(velocity);
	shotCmt->circleDrawer = shotObj->AttachComponent<CircleDrawer>();
	shotCmt->circleCollider = shotObj->AttachComponent<Eagle::CircleCollider2D>();
	shotObj->SetTag("EnemyShot");
	shotCmt->bulletCommander = commander;
	shotCmt->bulletCommander->AddBullet(shotCmt);
	shotCmt->player = Eagle::GameObject::FindWithTag("Player")->GetComponent<Player>();

	shotCmt->circleDrawer->SetRadius(radius);
	shotCmt->circleDrawer->SetColor(color);
	shotCmt->circleCollider->SetRadius(radius);

	return shotCmt;
}

CircleDrawer* EnemyBulletCircle::GetCircleDrawer() const noexcept
{
	return circleDrawer;
}

void EnemyBulletCircle::Start()
{
}

void EnemyBulletCircle::Update()
{
	Eagle::Vector3 newPos = GetTransform()->GetWorldPosition(false) + (velocity * Eagle::Time::DeltaTimeF());

	if (
		!Eagle::Math::IsBetween(newPos.x, FIELD_MIN_X, FIELD_MAX_X) ||
		!Eagle::Math::IsBetween(newPos.y, FIELD_MIN_Y, FIELD_MAX_Y)
		)
	{
		Eagle::GameObject::Destroy(GetGameObject());
		return;
	}

	GetTransform()->SetPosition(newPos);

	if (circleCollider->Collided(player->GetGameObject()->GetCollider()))
	{
		player->OnDamage(10.f);
		Eagle::GameObject::Destroy(GetGameObject());
		GetGameObject()->SetActive(false);
	}
}
