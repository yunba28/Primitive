#include "enemy_bullet_laser.hpp"
#include "player.hpp"
#include "drawer.hpp"
#include "bullet_commander.hpp"

EnemyBulletLaser::EnemyBulletLaser()
	: BulletBase()
	, player(nullptr)
	, capsuleCollider(nullptr)
{
}

EnemyBulletLaser::EnemyBulletLaser(const Eagle::Vector3& _velocity)
	: BulletBase()
	, player(nullptr)
	, capsuleCollider(nullptr)
{
	velocity = _velocity;
}

EnemyBulletLaser::~EnemyBulletLaser()
{
}

EnemyBulletLaser* EnemyBulletLaser::Create(
	BulletCommander* commander,
	const Eagle::Vector3& velocity,
	Eagle::Color color,
	float laserLength,
	float laserThickness
)
{
	auto shotObj = Eagle::GameObject::Create();
	auto shotCmt = shotObj->AttachComponent<EnemyBulletLaser>(velocity);
	auto capsule = shotObj->AttachComponent<CapsuleDrawer>();
	shotCmt->capsuleCollider = shotObj->AttachComponent<Eagle::CapsuleCollider2D>();
	shotObj->SetTag("EnemyShot");
	shotCmt->bulletCommander = commander;
	shotCmt->bulletCommander->AddBullet(shotCmt);
	shotCmt->player = Eagle::GameObject::FindWithTag("Player")->GetComponent<Player>();

	const Eagle::Vector2 colliderAxis = Eagle::ToVec2(velocity.Normalize());

	capsule->SetAxis(colliderAxis);
	capsule->SetHeight(laserLength);
	capsule->SetRadius(laserThickness);
	capsule->SetColor(color);
	shotCmt->capsuleCollider->SetAxis(colliderAxis);
	shotCmt->capsuleCollider->SetHeight(laserLength);
	shotCmt->capsuleCollider->SetRadius(laserThickness);

	return shotCmt;
}

void EnemyBulletLaser::Start()
{
}

void EnemyBulletLaser::Update()
{
	Eagle::Vector3 newPos = GetTransform()->GetWorldPosition(false) + (velocity * Eagle::Time::DeltaTimeF());

	Eagle::Vector2 extents = (capsuleCollider->GetScaledExtents() + Eagle::Vector2(capsuleCollider->GetScaledRadius()));

	Eagle::Vector2 tailPos = Eagle::ToVec2(newPos) - extents;

	if (
		!Eagle::Math::IsBetween(tailPos.x, FIELD_MIN_X, FIELD_MAX_X) ||
		!Eagle::Math::IsBetween(tailPos.y, FIELD_MIN_Y, FIELD_MAX_Y)
		)
	{
		Eagle::GameObject::Destroy(GetGameObject());
		return;
	}

	GetTransform()->SetPosition(newPos);

	if (capsuleCollider->Collided(player->GetGameObject()->GetCollider()))
	{
		player->OnDamage(10.f);
	}
}
