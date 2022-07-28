#pragma once
#include "eagle.hpp"

class Font;
class TimeUpdater;

class TimeCounter : public Eagle::Renderer, public Eagle::PauseAndResume {
public:
	TimeCounter();
	TimeCounter(const Eagle::String& _fontName, unsigned int _fontSize, Eagle::Color _color);

	void Pause()override;

	void Resume()override;

	float GetTime()const noexcept;

private:
	UNIQUE(Font) font;
	Eagle::Color color;
	TimeUpdater* timeUpdater;
	float time;

	void Awake()override;

	void Render()override;
};