#include "player_armor_gauge.hpp"
#include "player.hpp"
#include "gauge.hpp"
#include "drawer.hpp"

#include "eagle/ease.hpp"

PlayerArmorGauge::PlayerArmorGauge()
	: Behavior(ID::LateUpdate)
	, player(nullptr)
	, gauge(nullptr)
{
}

PlayerArmorGauge::PlayerArmorGauge(Player* _player)
	: Behavior()
	, player(_player)
	, gauge(nullptr)
{
}

Gauge* PlayerArmorGauge::GetGauge() const noexcept
{
	return gauge;
}

void PlayerArmorGauge::Awake()
{
	gauge = GetGameObject()->AttachComponent<Gauge>();
	gauge->SetSize(200.f, 10.f);
	gauge->GetGaugeBox()->SetColor(0x00ff00);
	gauge->SetGaugeAligned(GaugeAligned::Left);
}

void PlayerArmorGauge::Start()
{
	if (!player)
		player = Eagle::GameObject::FindWithTag("Player")->GetComponent<Player>();
}

void PlayerArmorGauge::LateUpdate()
{
	float newValue = player->GetArmor() / 100.f;
	Eagle::Color newColor = Eagle::Color::FromHsv(Eagle::Ease::Linear(newValue, 0.f, 120.f), 1.f, 1.f);
	gauge->SetValue(newValue);
	gauge->GetGaugeBox()->SetColor(newColor);
}