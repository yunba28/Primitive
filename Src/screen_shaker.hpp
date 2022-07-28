#pragma once
#include "eagle.hpp"

class ScreenShaker : public Eagle::Behavior {
public:
	ScreenShaker();
	~ScreenShaker();

	void OnShake(const Eagle::Vector3& _strength, float _duration);

private:
	Eagle::Vector3 strength;
	float t;
	float timeFactor;

	void Update()override;
};