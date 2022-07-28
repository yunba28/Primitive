#include "player.hpp"
#include "player_move.hpp"
#include "player_shot.hpp"
#include "player_armor_gauge.hpp"
#include "control_slow_time.hpp"
#include "drawer.hpp"
#include "gauge.hpp"
#include "interval.hpp"
#include "bullet_commander.hpp"
#include "effect_hit.hpp"

#include "sound_manager.hpp"

Player::Player()
	: Behavior(ID::Update)
	, circleDrawer(nullptr)
	, circleCollider(nullptr)
	, controlSlowTime(nullptr)
	, damageFrameBox(nullptr)
	, damageInterval(new Interval)
	, armor(100.f)
	, damageCutRate(0.f)
{
	damageInterval->SetInterval(0.2f);
}

Player::~Player()
{
}

void Player::Pause()
{
	controlSlowTime->SetEnable(false);
}

void Player::Resume()
{
	controlSlowTime->SetEnable(true);
}

void Player::OnDamage(float damage)
{
	if (!damageInterval->IsReady())
		return;

	damageInterval->Ready();

	if ((Eagle::Time::GetTimeFactorF() < 1.f) && GameConfig::IsEnableGameLevel(GameLevel::Hard))
	{
		damage *= 1.5f;
	}

	damage *= (1.f - damageCutRate);

	armor -= damage;

	Eagle::Math::Floor(armor, 0.f);

	damageFrameBox
		->DoFade(1.f, 0.045f)
		.SetLoop(3, Eagle::LoopType::Reverse)
		.SetOnComplete([&]() 
			{
				damageFrameBox->SetTransparency(0.f);
			}
	);

	SoundManager::Play("se_damage5.mp3", DX_PLAYTYPE_BACK, 0.8f);

	if (armor <= 33.f)
	{
		auto obj = Eagle::GameObject::Find("PlayerInnerBox");
		if (!obj)
			return;
		Eagle::GameObject::Destroy(obj);
		EffectHit::Create(GetTransform()->GetWorldPosition(false), 17.5f, 1.f, circleDrawer->GetColor());
	}
	else if (armor <= 66.f)
	{
		auto obj = Eagle::GameObject::Find("PlayerOuterBox");
		if (!obj)
			return;
		Eagle::GameObject::Destroy(obj);
		EffectHit::Create(GetTransform()->GetWorldPosition(false), 17.5f, 1.f, circleDrawer->GetColor());
	}
}

void Player::SetDamageCutRate(float _cutRate)
{
	damageCutRate = Eagle::Math::Ceiling(_cutRate, 1.f);
}

CircleDrawer* Player::GetCircleDrawer() const noexcept
{
	return circleDrawer;
}

Eagle::CircleCollider2D* Player::GetCircleCollider() const noexcept
{
	return circleCollider;
}

float Player::GetArmor() const noexcept
{
	return armor;
}

Player* Player::Create(Eagle::String name, float speed, float radius, Eagle::Color color, float shotInterval)
{
	auto playerObj = Eagle::GameObject::Create(name);
	auto playerCmt = playerObj->AttachComponent<Player>();
	playerObj->GetTransform()->SetPosition(400, 650, 0);
	playerObj->AttachComponent<PlayerMove>(speed);
	playerObj->AttachComponent<PlayerShot>(shotInterval, speed * 2.f, color);
	playerCmt->circleDrawer = playerObj->AttachComponent<CircleDrawer>(radius, color);
	playerCmt->circleCollider = playerObj->AttachComponent<Eagle::CircleCollider2D>(radius);

	playerCmt->controlSlowTime = Eagle::GameObject::Create("ControlSlowTime")->AttachComponent<ControlSlowTime>();
	playerCmt->controlSlowTime->GetGauge()->SetSize(180.f, 10.f);
	playerCmt->controlSlowTime->GetTransform()->SetPosition(900, 65, 0);
	playerCmt->controlSlowTime->GetTransform()->DoMoveX(-200.f, 1.f).SetEase(Eagle::GetEase(Eagle::EaseType::QuadOut));

	Eagle::Color outerColor = GameConfig::IsEnableGameLevel(GameLevel::Debug) ? 0xffff00 : color.value;

	auto box1 = Eagle::GameObject::Create("PlayerOuterBox")->AttachComponent<BoxDrawer>(radius * 5.f, radius * 5.f, outerColor);
	box1->GetTransform()->SetPosition(playerObj->GetTransform()->GetWorldPosition());
	box1->GetGameObject()->SetParent(playerObj);
	box1->SetFill(false);
	box1->GetTransform()->DoLocalRotateZ(90.f, 3.f).SetLoop(-1, Eagle::LoopType::Reset);
	box1->GetGameObject()->SetLayer(3);
	auto box2 = Eagle::GameObject::Create("PlayerInnerBox")->AttachComponent<BoxDrawer>(radius * 3.5f, radius * 3.5f, color);
	box2->GetTransform()->SetPosition(playerObj->GetTransform()->GetWorldPosition());
	box2->GetGameObject()->SetParent(playerObj);
	box2->SetFill(false);
	box2->GetTransform()->DoLocalRotateZ(90.f, 1.f).SetLoop(-1, Eagle::LoopType::Reset);
	box2->GetGameObject()->SetLayer(3);

	playerObj->SetTag("Player");

	return playerCmt;
}

void Player::Start()
{
	damageFrameBox = Eagle::GameObject::Find("FrameBox")->GetComponent<BoxDrawer>();
}

void Player::Update()
{
	damageInterval->Update();

	if (controlSlowTime->IsStart())
	{
		SetSlowTimeState(0.2f, 0.5f);
	}

	if (controlSlowTime->IsEnd())
	{
		SetSlowTimeState(1.f, 2.f);
	}
}

void Player::SetSlowTimeState(float _timeSpeed, float _radiusFactor)
{
	const float radius = circleCollider->GetRadius();
	Eagle::Time::SetTimeFactor(_timeSpeed);
	circleCollider->SetRadius(radius * _radiusFactor);
	circleDrawer->SetRadius(radius * _radiusFactor);
}
