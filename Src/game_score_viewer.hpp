#pragma once
#include "eagle.hpp"

class Text;

class GameScoreViewer : public Eagle::Behavior {
public:
	GameScoreViewer();
	~GameScoreViewer();

	Text* GetScoreTitle()const noexcept;
	Text* GetScoreValue()const noexcept;
	Text* GetHighScoreTitle()const noexcept;
	Text* GetHighScoreValue()const noexcept;

private:
	Eagle::FixedArray<Text*,4> text;
	int prevScore;
	int nextScore;
	float t;

	void Awake()override;
	void Start()override;
	void LateUpdate()override;

	void UpdateNextScore();
};