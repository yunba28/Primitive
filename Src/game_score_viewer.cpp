#include "game_score_viewer.hpp"
#include "text.hpp"
#include "game_score.hpp"
#include "currency_format.hpp"

#include <fstream>
#include <filesystem>

GameScoreViewer::GameScoreViewer()
	: Behavior(ID::LateUpdate)
	, text()
	, prevScore(0)
	, nextScore(0)
	, t(0.f)
{
}

GameScoreViewer::~GameScoreViewer()
{
}

Text* GameScoreViewer::GetScoreTitle() const noexcept
{
	return text[0];
}

Text* GameScoreViewer::GetScoreValue() const noexcept
{
	return text[1];
}

Text* GameScoreViewer::GetHighScoreTitle() const noexcept
{
	return text[2];
}

Text* GameScoreViewer::GetHighScoreValue() const noexcept
{
	return text[3];
}

void GameScoreViewer::Awake()
{
	text[0] = Eagle::GameObject::Create()->AttachComponent<Text>("SCORE");
	text[0]->GetGameObject()->SetLayer(5);
	text[1] = Eagle::GameObject::Create()->AttachComponent<Text>();
	text[1]->GetGameObject()->SetLayer(5);

	text[2] = Eagle::GameObject::Create()->AttachComponent<Text>("HIGH SCORE");
	text[2]->GetGameObject()->SetLayer(5);
	text[3] = Eagle::GameObject::Create()->AttachComponent<Text>();
	text[3]->GetGameObject()->SetLayer(5);
}

void GameScoreViewer::Start()
{
	text[0]->SetFont("Consolas", 24);
	text[0]->SetDrawPosition(DrawPosition::Left);
	text[1]->SetFont("Consolas", 24);
	text[1]->SetDrawPosition(DrawPosition::Right);

	text[2]->SetFont("Consolas", 24);
	text[2]->SetDrawPosition(DrawPosition::Left);
	text[3]->SetFont("Consolas", 24);
	text[3]->SetDrawPosition(DrawPosition::Right);

	int highScore = GameScore::GetHighScore();
	if (highScore == 0)
	{
		text[3]->SetText("\nNo Data");
	}
	else
	{
		text[3]->SetText(CurrencyFormat("\n%d", highScore));
	}
}

void GameScoreViewer::LateUpdate()
{
	int score = static_cast<int>(Eagle::Ease::Linear(static_cast<double>(t), static_cast<double>(prevScore), static_cast<double>(nextScore)));

	Eagle::String scoreText = CurrencyFormat("\n%d", score);

	text[1]->SetText(scoreText);

	if (t < 1.f)
	{
		t += Eagle::Time::DeltaTimeF();
		Eagle::Math::Ceiling(t, 1.f);
	}
	UpdateNextScore();
}

void GameScoreViewer::UpdateNextScore()
{
	unsigned score = GameScore::GetScore();

	if (score > nextScore)
	{
		prevScore = static_cast<int>(Eagle::Easing(t, static_cast<float>(prevScore), static_cast<float>(nextScore)));
		t = 0.f;
		nextScore = score;
	}
}
