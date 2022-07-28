#include "player_bullet.hpp"
#include "bullet_commander.hpp"
#include "drawer.hpp"

#include "enemy.hpp"

class PlayerBulletDraw : public Eagle::Material {
public:
	PlayerBulletDraw(float _radius, Eagle::Color _color);
private:
	float radius;
	
	void Render()override;
};

PlayerBulletDraw::PlayerBulletDraw(float _radius, Eagle::Color _color)
	: Material()
	, radius(_radius)
{
	SetColor(_color);
}

void PlayerBulletDraw::Render()
{
	Eagle::Vector2 pos = Eagle::ToVec2(GetTransform()->GetWorldPosition());
	float hR = radius * 1.5f;
	float wR = radius;

	Eagle::Vector2 p[] = {
		{pos.x,pos.y - hR},
		{pos.x + wR,pos.y},
		{pos.x,pos.y + hR},
		{pos.x - wR,pos.y}
	};

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(GetTransparency() * 255.f));

	DrawQuadrangleAA(p[0].x, p[0].y, p[1].x, p[1].y, p[2].x, p[2].y, p[3].x, p[3].y, GetColor(), false, 2.f);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

PlayerBullet::PlayerBullet()
	: BulletBase()
{
	SetBehaveID(ID::Update);
}

PlayerBullet::PlayerBullet(const Eagle::Vector3& _velocity)
	: BulletBase()
{
	velocity = _velocity;
}

PlayerBullet::~PlayerBullet()
{
}

PlayerBullet* PlayerBullet::Create(BulletCommander* commander, const Eagle::Vector3& _velocity, float _radius, Eagle::Color _color)
{
	auto shotObj = Eagle::GameObject::Create();
	auto shotCmt = shotObj->AttachComponent<PlayerBullet>(_velocity);
	shotObj->AttachComponent<PlayerBulletDraw>(_radius, _color);
	shotObj->AttachComponent<Eagle::CircleCollider2D>(_radius);
	shotObj->SetTag("PlayerShot");
	shotCmt->bulletCommander = commander;
	shotCmt->bulletCommander->AddBullet(shotCmt);

	return shotCmt;
}


void PlayerBullet::Update()
{
	Eagle::Vector3 newPos = GetTransform()->GetWorldPosition(false);

	newPos += velocity * Eagle::Time::DeltaTime();

	if (newPos.y < 0.f)
	{
		Eagle::GameObject::Destroy(GetGameObject());
		return;
	}

	GetTransform()->SetPosition(newPos);

	auto enemies = GetGameObject()->GetScene()->FindGameObjectsWithTag("Enemy");

	if (enemies.Empty())
		return;

	auto collider = GetGameObject()->GetCollider();

	for (auto enemy : enemies)
	{
		if (!enemy->IsActive())
			continue;

		if (!collider->Collided(enemy->GetCollider()))
			continue;

		Eagle::GameObject::Destroy(GetGameObject());

		return;
	}
}