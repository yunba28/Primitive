#pragma once
#include "eagle.hpp"

class Player;
class Gauge;

class PlayerArmorGauge : public Eagle::Behavior {
public:
	PlayerArmorGauge();
	PlayerArmorGauge(Player* _player);

	Gauge* GetGauge()const noexcept;

private:
	Player* player;
	Gauge* gauge;

	void Awake()override;
	void Start()override;
	void LateUpdate()override;
};