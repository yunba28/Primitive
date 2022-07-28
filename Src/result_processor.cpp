#include "result_processor.hpp"
#include "eagle/scene_management.hpp"
#include "scene_to_scene_connect.hpp"

#include "button.hpp"
#include "button_widget.hpp"
#include "game_processor.hpp"
#include "level_processor.hpp"
#include "title_processor.hpp"

#include "game_score.hpp"
#include "currency_format.hpp"

#include "drawer.hpp"

#include "text.hpp"

namespace {
	void MoveTitleScene() {
		auto box = Eagle::GameObject::Create("FadeObject")->AttachComponent<BoxDrawer>(800.f, 600.f, 0x001e43);
		box->SetTransparency(0.f);
		box->SetOffset(400.f, 300.f);
		box->GetGameObject()->SetLayer(10);

		auto moveScene = []() {
			Eagle::SceneManager::CreateScene("TitleScene");
			Eagle::SceneManager::SetChangeActiveScene("TitleScene");
			Eagle::GameObject::Create("Processor")
				->AttachComponent<TitleProcessor>();

			Eagle::SceneManager::DestroySceneImmediately(Eagle::SceneManager::GetSceneAt("GameScene"));
			Eagle::SceneManager::DestroyScene("ResultScene");

			SetBackgroundColor(0x00, 0x1e, 0x43);
		};

		box->DoFade(1.f, 0.5f).SetOnComplete(moveScene).SetOnKill([]() {Eagle::GameObject::Destroy("FadeObject"); });
	}

	void MoveLevelScene()
	{
		Eagle::SceneManager::CreateScene("LevelScene");
		Eagle::SceneManager::SetChangeActiveScene("LevelScene");

		Eagle::GameObject::Create("Processor")
			->AttachComponent<LevelProcessor>();
	}

	void GameContinue() {
		auto box = Eagle::GameObject::Create("FadeObject")->AttachComponent<BoxDrawer>(800.f, 600.f, 0x001e43);
		box->SetTransparency(0.f);
		box->SetOffset(400.f, 300.f);
		box->GetGameObject()->SetLayer(10);

		auto moveScene = []() {
			Eagle::SceneManager::CreateScene("TemporaryScene");
			Eagle::SceneManager::SetChangeActiveScene("TemporaryScene");

			Eagle::GameObject::Create()->AttachComponent<SceneToSceneConnect>(MoveLevelScene);

			Eagle::SceneManager::DestroySceneImmediately(Eagle::SceneManager::GetSceneAt("GameScene"));
			Eagle::SceneManager::DestroyScene("ResultScene");
		};

		box->DoFade(1.f, 0.5f).SetOnComplete(moveScene).SetOnKill([]() {Eagle::GameObject::Destroy("FadeObject"); });
	}
}

void ResultProcessor::Awake()
{
	auto center = GameConfig::GetWindowSizeF() * Eagle::Float2(0.5f, 0.5f);

	auto titleText = Eagle::GameObject::Create(GetGameObject()->GetName() + "_DialogText")->AttachComponent<Text>();
	titleText->SetText("GAME CLEAR");
	titleText->SetColor(0xffff00);
	titleText->SetFont("Consolas", 86);
	titleText->GetTransform()->SetPosition(center.x, center.y - 200.f, 0.f);
	titleText->GetGameObject()->SetParent(GetGameObject());
	titleText->GetGameObject()->SetLayer(7);

	auto scoreText = Eagle::GameObject::Create(GetGameObject()->GetName() + "_ScoreText1")->AttachComponent<Text>();
	scoreText->SetText("SCORE");
	scoreText->SetColor(0xffffff);
	scoreText->SetFont("Consolas", 72);
	scoreText->GetTransform()->SetPosition(center.x, center.y - 98, 0.f);
	scoreText->GetGameObject()->SetParent(GetGameObject());
	scoreText->GetGameObject()->SetLayer(7);

	scoreText = Eagle::GameObject::Create(GetGameObject()->GetName() + "_ScoreText2")->AttachComponent<Text>();
	scoreText->SetText(CurrencyFormat("\n%d", GameScore::GetScore()));
	scoreText->SetColor(0xffffff);
	scoreText->SetFont("Consolas", 68);
	scoreText->GetTransform()->SetPosition(center.x, center.y - 100, 0.f);
	scoreText->GetGameObject()->SetParent(GetGameObject());
	scoreText->GetGameObject()->SetLayer(7);

	auto widget = Eagle::GameObject::Create("ButtonWidget")->AttachComponent<ButtonWidget>();

	//スタートボタンの設定

	auto button = MakeButton("RetryButton", "Retry", "Consolas", 48, 300, 50);
	button->SetOnPressed(GameContinue);
	button->GetTransform()->SetPosition(center.x, center.y + 100.f, 0.f);
	button->GetBoxDrawer()->SetThickness(3.f);

	widget->Add(button);

	//クウィット（終了）ボタンの設定

	button = MakeButton("TitleButton", "Title", "Consolas", 48, 300, 50);
	button->SetOnPressed(MoveTitleScene);
	button->GetTransform()->SetPosition(center.x, center.y + 200.f, 0.f);
	button->GetBoxDrawer()->SetThickness(3.f);

	widget->Add(button);
}

void ResultProcessor::Start()
{
	GameScore::HighScoreUpdate();
}