#include "screen_shaker.hpp"

ScreenShaker::ScreenShaker()
	: Behavior(ID::Update)
	, strength(0.f)
	, t(0.f)
	, timeFactor(1.f)
{
	SetEnable(false);
}

ScreenShaker::~ScreenShaker()
{
}

void ScreenShaker::OnShake(const Eagle::Vector3& _strength, float _duration)
{
	strength = _strength;
	t = 0.f;
	timeFactor = 1.f / _duration;
	SetEnable(true);
}

void ScreenShaker::Update()
{
	t += Eagle::Time::DeltaTimeF() * timeFactor;

	Eagle::Math::Clamp(t, 0.00001f, 1.f);

	if (t == 1.f)
	{
		Eagle::Transform::SetWorldOrigin(Eagle::Vector3::zero);
		SetEnable(false);
		return;
	}

	Eagle::Vector3 v = Eagle::Ease::Linear(t, strength, Eagle::Vector3::zero);

	Eagle::Transform::SetWorldOrigin(v * std::sin(Eagle::Math::PI * 2 / t));
}
