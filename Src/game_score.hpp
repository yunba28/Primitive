#pragma once
#include "eagle/include/string.hpp"

class GameScore{
private:
	GameScore() = default;
	~GameScore() = default;

public:
	static void SetScore(const int _score);
	static void AddScore(const int _score);
	static int GetScore()noexcept;

	static void LoadHighScore();
	static int GetHighScore()noexcept;
	static void HighScoreUpdate();

private:
	static int score;
	static int highScore;
};