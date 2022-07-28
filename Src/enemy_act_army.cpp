#include "enemy_act_army.hpp"
#include "interval.hpp"
#include "drawer.hpp"
#include "effect_bomb.hpp"
#include "bullet_commander.hpp"
#include "enemy_bullet_circle.hpp"
#include "game_score.hpp"

EnemyActArmy::EnemyActArmy(Eagle::Vector3* _point, float _t, float _duration, ArmyType _type)
	: EnemyActBase()
	, point()
	, shotInterval(new Interval)
	, type(_type)
	, injectionAngle(0.f)
	, deltaSum(_t)
	, timeFuctor(1.f / _duration)
{
	for (int i = 0;i < 4;++i)
	{
		point[i] = _point[i];
	}

	shotInterval->SetInterval((GameConfig::IsEnableGameLevel(GameLevel::Easy)) ? 0.6f : 0.425f);
}

EnemyActArmy::~EnemyActArmy()
{
}

void EnemyActArmy::OnShootDown()
{
	OnMoveEnd();

	Eagle::Vector3 bombPos = GetTransform()->GetWorldPosition(false);
	bombPos += Eagle::ToVec3(
		Eagle::Random::Range(-15.f, 15.f),
		Eagle::Random::Range(-15.f, 15.f),
		0.f
	);

	EffectBomb::Create(bombPos, Eagle::Random::Range(16.f, 20.f), Eagle::Color::Pallet::Orange);

	GameScore::AddScore(1000000);
}

void EnemyActArmy::Start()
{
	bulletCommander = Eagle::GameObject::FindWithTag("BulletCommander")
		->GetComponent<BulletCommander>();

	GetGameObject()
		->GetComponent<CircleDrawer>()
		->SetColor(0xE2421F);
}

void EnemyActArmy::Update()
{
	shotInterval->Update();

	Eagle::Vector3 oldPos = GetTransform()->GetWorldPosition();
	Eagle::Vector3 newPos = Eagle::Bezier::Point4(deltaSum, point[0], point[1], point[2], point[3]);

	GetTransform()->SetPosition(newPos);

	if (deltaSum >= 1.f)
	{
		OnMoveEnd();
		SetEnable(false);
		return;
	}

	if (shotInterval->IsReady())
	{
		if (type == ArmyType::Diffusion)
		{
			DiffusionShot();
		}
		else if (type == ArmyType::Trail)
		{
			TrailShot(oldPos.To(newPos).Normalize());
		}

		shotInterval->Ready();
	}

	deltaSum += Eagle::Time::DeltaTimeF() * timeFuctor;
}

void EnemyActArmy::DiffusionShot()
{
	Eagle::Vector3 shotVelocity = Eagle::ToVec3(Eagle::Vector2::FromAngle(injectionAngle, 100.f));

	EnemyBulletCircle::Create(bulletCommander, shotVelocity, 0xE2421F, 5.f)
		->GetTransform()->SetPosition(GetTransform()->GetWorldPosition(false));

	injectionAngle = deltaSum * 360.f;
}

void EnemyActArmy::TrailShot(Eagle::Vector3 forwardAxis)
{
	Eagle::Vector3 shotVelocity[] = {
		forwardAxis * 50.f,
		Eagle::Vector3::MatrixTransform(forwardAxis,Eagle::Matrix4x4::GetRotateZ(30.f)) * 50.f,
		Eagle::Vector3::MatrixTransform(forwardAxis,Eagle::Matrix4x4::GetRotateZ(-30.f)) * 50.f
	};

	for (const auto& velocity : shotVelocity)
	{
		EnemyBulletCircle::Create(bulletCommander, velocity, 0xE2421F, 5.f)
			->GetTransform()->SetPosition(GetTransform()->GetWorldPosition(false));
	}
}
