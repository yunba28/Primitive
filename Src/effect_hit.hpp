#pragma once
#include "effect.hpp"

class CapsuleDrawer;

class EffectHit : public Effect {
public:
	static EffectHit* Create(const Eagle::Vector3& _position, float _length, float _radius, Eagle::Color _color);

private:
	Eagle::FixedArray<CapsuleDrawer*, 8> particle{};

	void Start()override;
};