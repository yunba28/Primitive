#include "gameover_dialog.hpp"
#include "button.hpp"
#include "button_widget.hpp"
#include "text.hpp"
#include "drawer.hpp"
#include "currency_format.hpp"

#include "game_utility.hpp"
#include "game_processor.hpp"
#include "title_processor.hpp"
#include "game_score.hpp"
#include "scene_to_scene_connect.hpp"

#include "eagle/scene_management.hpp"

namespace {
	void MoveTitleScene() {
		Eagle::SceneManager::CreateScene("TitleScene");
		Eagle::SceneManager::SetChangeActiveScene("TitleScene");
		Eagle::GameObject::Create("Processor")
			->AttachComponent<TitleProcessor>();

		Eagle::SceneManager::DestroyScene("GameScene");

		SetBackgroundColor(0x00, 0x1e, 0x43);
	}

	void MoveGameScene()
	{
		Eagle::SceneManager::CreateScene("GameScene");
		Eagle::SceneManager::SetChangeActiveScene("GameScene");

		Eagle::GameObject::Create("Processor")
			->AttachComponent<GameProcessor>();
	}

	void GameContinue() {
		Eagle::SceneManager::CreateScene("TemporaryScene");
		Eagle::SceneManager::SetChangeActiveScene("TemporaryScene");

		Eagle::GameObject::Create()->AttachComponent<SceneToSceneConnect>(MoveGameScene);

		Eagle::SceneManager::DestroyScene("GameScene");
	}
}

GameOverDialog::GameOverDialog()
	: Behavior(ID::NoBehave)
	, dialogBox(nullptr)
	, titleText(nullptr)
	, scoreText()
	, button()
{
}

void GameOverDialog::Awake()
{
	auto window = GameConfig::GetWindowSizeF();
	auto hWindow = window * Eagle::Float2(0.5f, 0.5f);

	dialogBox = GetGameObject()->AttachComponent<BoxDrawer>();
	dialogBox->SetSize(window.x, window.y);
	dialogBox->SetOffset(hWindow.x, hWindow.y); //他のボタンの位置を参照するために矩形の座標を左上に調整
	dialogBox->GetTransform()->SetPosition(0, 0, 0); //真ん中に出るようにする
	dialogBox->SetColor(0x000000);
	dialogBox->SetTransparency(0.5f);
	GetGameObject()->SetLayer(8);

	titleText = Eagle::GameObject::Create(GetGameObject()->GetName() + "_DialogText")->AttachComponent<Text>();
	titleText->SetText("GAME OVER");
	titleText->SetColor(0xff0000);
	titleText->SetFont("Consolas", 86);
	titleText->GetTransform()->SetPosition(hWindow.x, hWindow.y - 200.f, 0.f);
	titleText->GetGameObject()->SetParent(GetGameObject());
	titleText->GetGameObject()->SetLayer(9);

	scoreText[0] = Eagle::GameObject::Create(GetGameObject()->GetName() + "_ScoreText1")->AttachComponent<Text>();
	scoreText[0]->SetText("SCORE");
	scoreText[0]->SetColor(0xffffff);
	scoreText[0]->SetFont("Consolas", 72);
	scoreText[0]->GetTransform()->SetPosition(hWindow.x, hWindow.y - 98, 0.f);
	scoreText[0]->GetGameObject()->SetParent(GetGameObject());
	scoreText[0]->GetGameObject()->SetLayer(9);

	scoreText[1] = Eagle::GameObject::Create(GetGameObject()->GetName() + "_ScoreText2")->AttachComponent<Text>();
	scoreText[1]->SetText(CurrencyFormat("\n%d", GameScore::GetScore()));
	scoreText[1]->SetColor(0xffffff);
	scoreText[1]->SetFont("Consolas", 68);
	scoreText[1]->GetTransform()->SetPosition(hWindow.x, hWindow.y - 100, 0.f);
	scoreText[1]->GetGameObject()->SetParent(GetGameObject());
	scoreText[1]->GetGameObject()->SetLayer(9);

	auto widget = Eagle::GameObject::Create("ButtonWidget")->AttachComponent<ButtonWidget>();

	button.retry = MakeButton(GetGameObject()->GetName() + "_RetryButton", "Retry", "Consolas", 48, 200, 50);
	button.retry->GetGameObject()->SetParent(GetGameObject());
	button.retry->GetTransform()->SetPosition(hWindow.x, hWindow.y + 100.f, 0); //ダイアログボックスの右上
	button.retry->GetText()->SetColor(0xffffff);
	button.retry->SetOnPressed(GameContinue);
	button.retry->GetGameObject()->SetLayer(9);

	button.quit = MakeButton(GetGameObject()->GetName() + "_QuitButton", "Quit", "", 48, 200, 50);
	button.quit->GetGameObject()->SetParent(GetGameObject());
	button.quit->GetTransform()->SetPosition(hWindow.x, hWindow.y + 200.f, 0);
	button.quit->GetText()->SetColor(0xffffff);
	button.quit->GetGameObject()->SetLayer(9);
	button.quit->SetOnPressed(MoveTitleScene);

	widget->Add(button.retry);
	widget->Add(button.quit);
}

void GameOverDialog::Start()
{
	GameScore::HighScoreUpdate();
}

void GameOverDialog::OnEnable()
{
	dialogBox->SetEnable(true);
	button.SetEnable(true);
}

void GameOverDialog::OnDisable()
{
	dialogBox->SetEnable(false);
	button.SetEnable(false);
}

void GameOverDialog::ButtonList::SetEnable(bool flag)
{
	retry->GetGameObject()->SetActive(flag);
	quit->GetGameObject()->SetActive(flag);
}