#include "effect_bomb.hpp"
#include "drawer.hpp"
#include "sound_manager.hpp"

EffectBomb* EffectBomb::Create(const Eagle::Vector3& _position, float _radius, Eagle::Color _color)
{
	auto obj = Eagle::GameObject::Create();
	obj->GetTransform()->SetPosition(_position);
	obj->AttachComponent<CircleDrawer>(_radius,_color);
	obj->SetLayer(2);

	return obj->AttachComponent<EffectBomb>();
}

void EffectBomb::Start()
{
	GetTransform()->DoScale(Eagle::Vector3(2.5f), 1.f).SetEase(Eagle::Ease::Cubic::Out);
	auto circle = GetGameObject()->GetComponent<CircleDrawer>();
	circle->DoFade(0.f, 1.f)
		.SetEase(Eagle::Ease::Quint::InOut)
		.SetOnComplete([&]() {Eagle::GameObject::Destroy(GetGameObject()); });
	Eagle::Color origin = circle->GetColor();
	circle->SetColor(0xffffff);
	circle->DoColor(origin, 0.25f)
		.SetEase(Eagle::Ease::Quad::Out);

	SoundManager::Play("se_baaan1.mp3", DX_PLAYTYPE_BACK, 1.f);
}
