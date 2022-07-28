#include "gauge.hpp"

#include "drawer.hpp"

Gauge::Gauge()
	: Behavior(ID::NoBehave)
	, box()
	, barSize(200.f,10.f)
	, aligned(GaugeAligned::Center)
	, value(1.f)
{

}

Gauge::AlignFunc Gauge::alignFunc[3] = {
	&Gauge::AlignCenter,
	&Gauge::AlignLeft,
	&Gauge::AlignRight
};

Gauge::~Gauge()
{
}

void Gauge::SetSize(float _width, float _height)
{
	barSize.Set(_width, _height);
	box.gauge->SetSize(_width, _height);
	box.background->SetSize(_width, _height);
}

void Gauge::SetGaugeAligned(GaugeAligned _aligned)
{
	aligned = _aligned;

	(this->*alignFunc[static_cast<int>(aligned)])();
}

void Gauge::SetValue(float _value)
{
	value = Eagle::Math::Clamp01(_value);

	BarSize drawSize = barSize;
	drawSize.x *= value;

	box.gauge->SetSize(drawSize.x, drawSize.y);

	(this->*alignFunc[static_cast<int>(aligned)])();
}

BoxDrawer* Gauge::GetGaugeBox() const noexcept
{
	return box.gauge;
}

BoxDrawer* Gauge::GetBackgroundBox() const noexcept
{
	return box.gauge;
}

float Gauge::GetValue() const noexcept
{
	return value;
}

void Gauge::Awake()
{
	box.background = Eagle::GameObject::Create()->AttachComponent<BoxDrawer>(200.f, 10.f, 0x000000);
	box.gauge = Eagle::GameObject::Create()->AttachComponent<BoxDrawer>(200.f, 10.f, 0xffffff);

	box.background->SetTransparency(0.5f);

	box.background->GetGameObject()->SetParent(GetGameObject());
	box.gauge->GetGameObject()->SetParent(GetGameObject());

	box.background->GetGameObject()->SetLayer(5);
	box.gauge->GetGameObject()->SetLayer(5);
}

void Gauge::OnDestroy()
{
	Eagle::GameObject::Destroy(box.gauge->GetGameObject());
	Eagle::GameObject::Destroy(box.background->GetGameObject());
}

void Gauge::AlignCenter()
{
	box.gauge->GetTransform()->SetPosition(0, 0, 0);
}

void Gauge::AlignLeft()
{
	BarSize gSize = static_cast<Eagle::Float2>(box.gauge->GetSize());

	float offset = (barSize.x - gSize.x) * 0.5f;

	box.gauge->GetTransform()->SetPosition(-offset, 0.f, 0.f);
}

void Gauge::AlignRight()
{
	BarSize gSize = static_cast<Eagle::Float2>(box.gauge->GetSize());

	float offset = (barSize.x - gSize.x) * 0.5f;

	box.gauge->GetTransform()->SetPosition(offset, 0.f, 0.f);
}
