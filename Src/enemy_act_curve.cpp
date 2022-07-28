#include "enemy_act_curve.hpp"
#include "game_utility.hpp"
#include "eagle/ease.hpp"
#include "player.hpp"
#include "bullet_base.hpp"
#include "bullet_commander.hpp"
#include "interval.hpp"
#include "drawer.hpp"

#include "effect_bomb.hpp"

#include "enemy_bullet_laser.hpp"

#include "game_score.hpp"

#include <cassert>

EnemyActCurve::EnemyActCurve()
	: EnemyActBase()
	, point()
	, shotInterval(new Interval)
	, deltaSum(0.f)
	, timeFuctor(0.2f)
{
	SetBehaveID(ID::Update);
	if (GameConfig::GetGameLevel() == GameLevel::Easy)
	{
		shotInterval->SetInterval(0.2f);
	}
	else
	{
		shotInterval->SetInterval(0.1f);
	}
}

EnemyActCurve::~EnemyActCurve()
{
	
}

void EnemyActCurve::OnShootDown()
{
	OnMoveEnd();

	Eagle::Vector3 bombPos = GetTransform()->GetWorldPosition(false);
	bombPos += Eagle::ToVec3(
		Eagle::Random::Range(-15.f, 15.f),
		Eagle::Random::Range(-15.f, 15.f),
		0.f
	);

	EffectBomb::Create(bombPos, Eagle::Random::Range(16.f, 20.f), Eagle::Color::Pallet::Orange);

	GameScore::AddScore(5000000);
}

void EnemyActCurve::Start()
{
	void (EnemyActCurve:: *func[3])() = 
	{ 
		&EnemyActCurve::MakePointFromTop,
		&EnemyActCurve::MakePointFromLeft,
		&EnemyActCurve::MakePointFromRight
	};

	(this->*func[Eagle::Random::Range(0, 2)])();

	bulletCommander = Eagle::GameObject::FindWithTag("BulletCommander")
		->GetComponent<BulletCommander>();

	GetGameObject()
		->GetComponent<CircleDrawer>()
		->SetColor(Eagle::Color::Pallet::Purple);

	assert(bulletCommander != nullptr);
}

void EnemyActCurve::Update()
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
		Eagle::Vector3 shotVelocity = oldPos.To(newPos);
		shotVelocity *= (1.f / shotVelocity.Magnitude());
		shotVelocity *= 300.f;

		EnemyBulletLaser::Create(bulletCommander, shotVelocity, Eagle::Color::Pallet::Purple, 100.f, 2.f)
			->GetTransform()->SetPosition(newPos + (shotVelocity * 0.2f));

		shotInterval->Ready();
	}

	deltaSum += Eagle::Time::DeltaTimeF() * timeFuctor;
}

void EnemyActCurve::MakePointFromTop()
{
	point[0] = {
		Eagle::Random::Range(FIELD_MIN_X,FIELD_MAX_X),
		0.f,
		0.f
	};

	point[1] = {
		0.f,
		Eagle::Random::Range(FIELD_MIN_Y + 100.f,FIELD_MAX_Y - 100.f),
		0.f
	};

	point[2] = {
		0.f,
		Eagle::Random::Range(FIELD_MIN_Y + 100.f,FIELD_MAX_Y - 100.f),
		0.f
	};

	point[3] = {
		Eagle::Random::Range(FIELD_MIN_X,FIELD_MAX_X),
		0.f,
		0.f
	};

	const float LX = Eagle::Random::Range(FIELD_MIN_X, FIELD_MIN_X + 100.f);
	const float RX = Eagle::Random::Range(FIELD_MAX_X - 100.f, FIELD_MAX_X);

	if (Eagle::Random::Range01() > 0.5)
	{
		point[1].x = LX;
		point[2].x = RX;
	}
	else
	{
		point[1].x = RX;
		point[2].x = LX;
	}

	point[3].y = (Eagle::Random::Range01() > 0.5) ? 0.f : 600.f;
}

void EnemyActCurve::MakePointFromLeft()
{
	point[0] = {
		FIELD_MIN_X,
		Eagle::Random::Range(FIELD_MIN_Y,FIELD_MAX_Y),
		0.f
	};

	point[1] = {
		Eagle::Random::Range(FIELD_MIN_X + 100.f,FIELD_MAX_X - 100.f),
		0.f,
		0.f
	};

	point[2] = {
		Eagle::Random::Range(FIELD_MIN_X + 100.f,FIELD_MAX_X - 100.f),
		0.f,
		0.f
	};

	point[3] = {
		0.f,
		Eagle::Random::Range(FIELD_MIN_Y,FIELD_MAX_Y),
		0.f
	};

	const float TY = Eagle::Random::Range(FIELD_MIN_Y, FIELD_MIN_Y + 100.f);
	const float BY = Eagle::Random::Range(FIELD_MAX_Y - 100.f, FIELD_MAX_Y);

	if (Eagle::Random::Range01() > 0.5)
	{
		point[1].y = TY;
		point[2].y = BY;
	}
	else
	{
		point[1].y = TY;
		point[2].y = BY;
	}

	point[3].x = (Eagle::Random::Range01() > 0.5) ? 0.f : 600.f;
}

void EnemyActCurve::MakePointFromRight()
{
	point[0] = {
		FIELD_MAX_X,
		Eagle::Random::Range(FIELD_MIN_Y,FIELD_MAX_Y),
		0.f
	};

	point[1] = {
		Eagle::Random::Range(FIELD_MIN_X + 100.f,FIELD_MAX_X - 100.f),
		0.f,
		0.f
	};

	point[2] = {
		Eagle::Random::Range(FIELD_MIN_X + 100.f,FIELD_MAX_X - 100.f),
		0.f,
		0.f
	};

	point[3] = {
		0.f,
		Eagle::Random::Range(FIELD_MIN_Y,FIELD_MAX_Y),
		0.f
	};

	const float TY = Eagle::Random::Range(FIELD_MIN_Y, FIELD_MIN_Y + 100.f);
	const float BY = Eagle::Random::Range(FIELD_MAX_Y - 100.f, FIELD_MAX_Y);

	if (Eagle::Random::Range01() > 0.5)
	{
		point[1].y = TY;
		point[2].y = BY;
	}
	else
	{
		point[1].y = TY;
		point[2].y = BY;
	}

	point[3].x = (Eagle::Random::Range01() > 0.5) ? 0.f : 600.f;
}
