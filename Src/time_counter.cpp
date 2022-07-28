#include "time_counter.hpp"
#include "DxLib.h"
#include "font.hpp"

class TimeUpdater : public Eagle::Behavior {
public:
	TimeUpdater() :Behavior(ID::Update), time(0.f) {}

	float GetTime()const noexcept
	{
		return time;
	}

private:
	float time;

	void Update()override
	{
		time += Eagle::Time::DeltaTimeF();
	}
};

TimeCounter::TimeCounter()
	: Renderer()
	, font()
	, color(0xffffff)
	, timeUpdater(nullptr)
	, time(0.f)
{
}

TimeCounter::TimeCounter(const Eagle::String& _fontName, unsigned int _fontSize, Eagle::Color _color)
	: Renderer()
	, font(nullptr)
	, color(_color)
	, timeUpdater(nullptr)
	, time(0.f)
{
	font.reset(Font::Create(_fontName, _fontSize));
}

void TimeCounter::Pause()
{
	if (!timeUpdater->IsEnable())
		return;

	timeUpdater->SetEnable(false);
}

void TimeCounter::Resume()
{
	if (timeUpdater->IsEnable())
		return;

	timeUpdater->SetEnable(true);
}

float TimeCounter::GetTime() const noexcept
{
	return timeUpdater->GetTime();
}

void TimeCounter::Awake()
{
	timeUpdater = GetGameObject()->AttachComponent<TimeUpdater>();
}

void TimeCounter::Render()
{
	float time = timeUpdater->GetTime();

	char buf[32]{};
	int min = (int)(time / 60.f);
	int sec = (int)(time) - min * 60;
	int mSec = (int)((time - (float)((int)time)) * 100.f);
	std::snprintf(buf, sizeof(buf), "%02d:%02d:%02d", min, sec, mSec);

	Eagle::Vector3 pos = GetTransform()->GetWorldPosition();

	(font)
		? DrawString(pos, buf, font->GetHandle(), color)
		: DrawString(pos, buf, color);
}
