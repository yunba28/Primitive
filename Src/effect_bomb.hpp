#pragma once
#include "effect.hpp"

class EffectBomb : public Effect {
public:
	static EffectBomb* Create(const Eagle::Vector3& _position, float _radius, Eagle::Color _color);

private:
	void Start()override;
};