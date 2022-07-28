#pragma once
#include "eagle.hpp"

class Effect : public Eagle::Behavior {
public:
	Effect() :Behavior(ID::NoBehave) { Effect::effectList.Add(this); }
	~Effect()
	{
		auto itr = std::find(Effect::effectList.begin(), Effect::effectList.end(), this);
		if (itr != Effect::effectList.end())
		{
			Effect::effectList.Remove(itr);
		}
	}
private:
	static Eagle::List<Effect*> effectList;
};

inline Eagle::List<Effect*> Effect::effectList{};