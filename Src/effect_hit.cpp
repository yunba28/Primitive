#include "effect_hit.hpp"
#include "drawer.hpp"

EffectHit* EffectHit::Create(const Eagle::Vector3& _position, float _length, float _radius, Eagle::Color _color)
{
	auto obj = Eagle::GameObject::Create();
	auto effect = obj->AttachComponent<EffectHit>();
	obj->GetTransform()->SetPosition(_position);

	static const Eagle::Vector2 axis[] = {
		Eagle::Vector2::up,
		Eagle::Vector2::down,
		Eagle::Vector2::right,
		Eagle::Vector2::left,
		Eagle::ToVec2(1,-1),
		Eagle::ToVec2(1,1),
		Eagle::ToVec2(-1,-1),
		Eagle::ToVec2(-1,1)
	};

	int idx = 0;

	for (auto& par : effect->particle)
	{
		auto parObj = Eagle::GameObject::Create();
		parObj->SetParent(obj);
		par = parObj->AttachComponent<CapsuleDrawer>();
		par->SetAxis(axis[idx]);
		par->SetRadius(_radius);
		par->SetHeight(_length);
		par->SetColor(_color);
		parObj->GetTransform()->SetPosition(Eagle::ToVec3(axis[idx] * (_length * 0.5f + _radius)));

		++idx;
	}

	return effect;
}

void EffectHit::Start()
{
	Eagle::TweenerCore<Eagle::MoveAttribute>* tween = nullptr;

	for (auto& par : particle)
	{
		auto axis = par->GetExtents().Normalize();
		float height = par->GetHeight();
		tween = &par->GetTransform()->DoLocalMove(Eagle::ToVec3(axis * height * 1.2f), 0.25f).SetEase(Eagle::GetEase(Eagle::EaseType::CubicOut));
	}

	tween->SetOnComplete([&]() {
		Eagle::GameObject::Destroy(GetGameObject());
		for (auto& par : particle)
		{
			Eagle::GameObject::Destroy(par->GetGameObject());
		}
	});
}
