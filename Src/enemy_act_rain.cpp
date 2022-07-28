#include "enemy_act_rain.hpp"
#include "player.hpp"
#include "bullet_base.hpp"
#include "bullet_commander.hpp"
#include "drawer.hpp"

#include "effect_bomb.hpp"

#include "game_score.hpp"

#include <cassert>

namespace Internal {
	//[summary]
	//EnemyActRain‚ÅŽg—p‚³‚ê‚é’e
	class EnemyShotDrop : public BulletBase{
	public:
		EnemyShotDrop();
		EnemyShotDrop(const Eagle::Vector3& _velocity);
		~EnemyShotDrop();

		static EnemyShotDrop* Create(BulletCommander* commander, const Eagle::Vector3& _velocity, float _radius, Eagle::Color _color, float _offsetRot = 0.f);

	private:
		Eagle::GameObject* player;
		float offsetRot;
		bool firstAct;

		void Start()override;
		void Update()override;
		void FixedUpdate()override;

		void SetSecondAct();
	};

	EnemyShotDrop::EnemyShotDrop()
		: BulletBase()
		, player(nullptr)
		, offsetRot(0.f)
		, firstAct(true)
	{
		SetBehaveID(ID::Update | ID::Update);
	}

	EnemyShotDrop::EnemyShotDrop(const Eagle::Vector3& _velocity)
		: BulletBase()
		, player(nullptr)
		, offsetRot(0.f)
		, firstAct(true)
	{
		velocity = _velocity;
		SetBehaveID(ID::Update | ID::FixedUpdate);
	}

	EnemyShotDrop::~EnemyShotDrop()
	{
	}

	EnemyShotDrop* EnemyShotDrop::Create(BulletCommander* commander, const Eagle::Vector3& _velocity, float _radius, Eagle::Color _color, float _offsetRot)
	{
		auto shotObj = Eagle::GameObject::Create();
		auto shotCmt = shotObj->AttachComponent<EnemyShotDrop>(_velocity);
		shotObj->AttachComponent<CircleDrawer>(_radius, _color);
		shotObj->AttachComponent<Eagle::CircleCollider2D>(_radius);
		shotObj->SetTag("EnemyShot");
		shotCmt->offsetRot = _offsetRot;
		shotCmt->bulletCommander = commander;
		shotCmt->bulletCommander->AddBullet(shotCmt);

		if (GameConfig::GetGameLevel() == GameLevel::Easy)
		{
			shotCmt->offsetRot = 0.f;
		}

		return shotCmt;
	}

	void EnemyShotDrop::Start()
	{
		player = Eagle::GameObject::FindWithTag("Player");
	}

	void EnemyShotDrop::Update()
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

		auto collider = GetGameObject()->GetCollider();

		if (collider->Collided(player->GetCollider()))
		{
			player->GetComponent<Player>()->OnDamage();
			Eagle::GameObject::Destroy(GetGameObject());
		}

		if (!firstAct)
			return;

		if (velocity.MagnitudeSq() < 10.f)
			SetSecondAct();
	}

	void EnemyShotDrop::FixedUpdate()
	{
		if (!firstAct)
			return;

		velocity *= 0.9f;
	}

	void EnemyShotDrop::SetSecondAct()
	{
		//Player‚ªŒ©‚Â‚©‚ç‚È‚©‚Á‚½
		if (!player)
		{
			velocity *= 500.f;
			firstAct = false;
			return;
		}

		Eagle::Vector3 myPos = GetTransform()->GetWorldPosition(false);
		Eagle::Vector3 plPos = player->GetTransform()->GetWorldPosition(false);

		velocity = myPos.To(plPos).Normalize() * 500.f;

		if (offsetRot != 0.f)
		{
			auto mRot = Eagle::Matrix4x4::GetRotateZ(offsetRot);
			velocity = Eagle::Vector3::MatrixTransform(velocity, mRot);
		}

		firstAct = false;
	}

	//[summary]
	//EnemyActRain‚ªŒ‚’Ä‚³‚ê‚½‚Æ‚«‚É•úo‚³‚ê‚é’e
	class EnemyShotRain : public BulletBase {
	public:
		EnemyShotRain();
		EnemyShotRain(const Eagle::Vector3& _velocity);
		~EnemyShotRain();

		static EnemyShotRain* Create(BulletCommander* commander, const Eagle::Vector3& _velocity, float radius, Eagle::Color color);

	private:
		float fallAcceleration;

		void Start()override {}
		void Update()override;
		void FixedUpdate()override;
	};

	EnemyShotRain::EnemyShotRain()
		: BulletBase()
		, fallAcceleration(12.8f)
	{
		SetBehaveID(ID::Update | ID::FixedUpdate);
	}

	EnemyShotRain::EnemyShotRain(const Eagle::Vector3& _velocity)
		: BulletBase()
		, fallAcceleration(12.8f)
	{
		velocity = _velocity;
		SetBehaveID(ID::Update | ID::FixedUpdate);
	}

	EnemyShotRain::~EnemyShotRain()
	{
	}

	EnemyShotRain* EnemyShotRain::Create(BulletCommander* commander, const Eagle::Vector3& _velocity, float radius, Eagle::Color color)
	{
		auto shotObj = Eagle::GameObject::Create();
		auto shotCmt = shotObj->AttachComponent<EnemyShotRain>(_velocity);
		shotObj->AttachComponent<CircleDrawer>(radius, color);
		shotObj->AttachComponent<Eagle::CircleCollider2D>(radius);
		shotObj->SetTag("EnemyShot");
		shotCmt->bulletCommander = commander;
		shotCmt->bulletCommander->AddBullet(shotCmt);


		return shotCmt;
	}

	void EnemyShotRain::Update()
	{
		Eagle::Vector3 newPos = GetTransform()->GetWorldPosition(false) + (velocity * Eagle::Time::DeltaTimeF());

		if (
			!Eagle::Math::IsBetween(newPos.x, FIELD_MIN_X, FIELD_MAX_X) ||
			!Eagle::Math::IsBetween(newPos.y, FIELD_MIN_Y - 100.f, FIELD_MAX_Y)
			)
		{
			Eagle::GameObject::Destroy(GetGameObject());
			return;
		}

		GetTransform()->SetPosition(newPos);

		auto player = Eagle::GameObject::FindWithTag("Player");

		auto collider = GetGameObject()->GetCollider();

		if (collider->Collided(player->GetCollider()))
		{
			player->GetComponent<Player>()->OnDamage();
			Eagle::GameObject::Destroy(GetGameObject());
		}
	}

	void EnemyShotRain::FixedUpdate()
	{
		velocity.y += fallAcceleration;

		Eagle::Math::Ceiling(velocity.y, 500.f);
	}
}

EnemyActRain::EnemyActRain()
	: EnemyActBase()
	, speed(300.f)
{
	SetBehaveID(ID::Update);
}

EnemyActRain::EnemyActRain(float _speed)
	: EnemyActBase()
	, speed(_speed)
{
	SetBehaveID(ID::Update);
}

EnemyActRain::~EnemyActRain()
{
}

void EnemyActRain::OnShootDown()
{
	OnMoveEnd();

	MakeRainBullet(GetTransform()->GetWorldPosition(false), 3);

	Eagle::Vector3 bombPos = GetTransform()->GetWorldPosition(false);
	bombPos += Eagle::ToVec3(
		Eagle::Random::Range(-15.f, 15.f),
		Eagle::Random::Range(-15.f, 15.f),
		0.f
	);

	EffectBomb::Create(bombPos, Eagle::Random::Range(16.f, 20.f), Eagle::Color::Pallet::Orange);

	GameScore::AddScore(500000);
}

void EnemyActRain::Start()
{
	bulletCommander = Eagle::GameObject::FindWithTag("BulletCommander")
		->GetComponent<BulletCommander>();

	GetGameObject()
		->GetComponent<CircleDrawer>()
		->SetColor(Eagle::Color::Pallet::Magenta);

	GetTransform()->SetPosition(
		Eagle::Random::Range(210.f, 590.f),
		-100,
		0
	);

	assert(bulletCommander != nullptr);
}

void EnemyActRain::Update()
{
	auto pos = GetTransform()->GetWorldPosition(false);

	Eagle::Vector3 velocity(0, 1, 0);

	velocity *= speed * Eagle::Time::DeltaTimeF();

	pos += velocity;

	if (pos.y > FIELD_MAX_Y)
	{
		OnMoveEnd();
		SetEnable(false);

		pos.y -= 5.f;

		MakeDropBullet(pos, 3, -120.f, -50.f);

		return;
	}

	GetTransform()->SetPosition(pos);
}

namespace Internal {
	static constexpr float offsetRot[] = { 0.f,-15.f,15.f,-30.f,30.f,-45.f,45.f,-60.f,60.f };
}

void EnemyActRain::MakeDropBullet(const Eagle::Vector3& pos, unsigned int num, float angleMin, float angleMax)
{
	Eagle::Vector3 shotVelocity{};

	for (unsigned int i = 0;i < num;++i)
	{
		shotVelocity = Eagle::ToVec3(Eagle::Vector2::FromAngle(Eagle::Random::Range(angleMin, angleMax)));

		shotVelocity *= speed * 2.f;

		Internal::EnemyShotDrop::Create(
			bulletCommander,
			shotVelocity,
			5.f,
			Eagle::Color::Pallet::Magenta,
			GameConfig::IsEnableGameLevel(GameLevel::Easy) ? 0.f : Internal::offsetRot[i]
		)->GetTransform()->SetPosition(pos);
	}
}

void EnemyActRain::MakeRainBullet(const Eagle::Vector3& pos, unsigned int num)
{
	Eagle::Vector3 shotVelocity{};

	for (unsigned int i = 0;i < num;++i)
	{
		shotVelocity.Set(0, -1, 0);

		shotVelocity = Eagle::Vector3::MatrixTransform(shotVelocity, Eagle::Matrix4x4::GetRotateZ(Internal::offsetRot[i]));

		shotVelocity *= speed;

		Internal::EnemyShotRain::Create(bulletCommander, shotVelocity, 5.f, Eagle::Color::Pallet::Magenta)
			->GetTransform()->SetPosition(pos);
	}
}
