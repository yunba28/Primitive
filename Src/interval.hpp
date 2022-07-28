#pragma once
#include "eagle.hpp"

class Interval {
public:
	Interval() :deltaSum(0.f), interval(1.f) {}

	void SetInterval(float _interval)
	{
		interval = _interval;
	}

	void Update()
	{
		deltaSum += Eagle::Time::DeltaTimeF();
	}

	bool IsReady()const noexcept
	{
		return deltaSum >= interval;
	}

	void Ready()noexcept
	{
		deltaSum = 0.f;
	}

private:
	float deltaSum;
	float interval;
};