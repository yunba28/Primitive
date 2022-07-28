#include "game_score.hpp"
#include "game_utility.hpp"

#include <filesystem>
#include <fstream>

int GameScore::score = 0;
int GameScore::highScore = 0;

void GameScore::SetScore(const int _score)
{
	GameScore::score = _score;
}

void GameScore::AddScore(const int _score)
{
	float factor = [] {
		if (GameConfig::IsEnableGameLevel(GameLevel::Easy))
			return 0.5f;
		if (GameConfig::IsEnableGameLevel(GameLevel::Normal))
			return 1.0f;
		if (GameConfig::IsEnableGameLevel(GameLevel::Hard))
			return 2.5f;
		return 1.0f;
	}();
	GameScore::score += static_cast<int>(static_cast<float>(_score) * factor);
}

int GameScore::GetScore() noexcept
{
	return GameScore::score;
}

void GameScore::LoadHighScore()
{
	auto filePath = std::filesystem::current_path();
	filePath += "\\Data\\score.dat";
	std::ifstream ifs(filePath.c_str());

	if (!ifs)
		return;

	Eagle::String buf;
	std::getline(ifs, buf);

	if (buf == "")
		return;

	GameScore::highScore = std::stoi(buf);
}

int GameScore::GetHighScore() noexcept
{
	return GameScore::highScore;
}

void GameScore::HighScoreUpdate()
{
	if (GameConfig::IsEnableGameLevel(GameLevel::Debug))
		return;

	if (GameScore::score < GameScore::highScore)
		return;

	auto filePath = std::filesystem::current_path();
	filePath += "\\Data\\score.dat";
	std::ofstream ofs(filePath.c_str());

	if (!ofs)
		return;

	ofs << GameScore::score;
	GameScore::highScore = GameScore::score;
}
