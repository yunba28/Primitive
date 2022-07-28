#include "level_processor.hpp"
#include "eagle/scene_management.hpp"

#include "button.hpp"
#include "button_widget.hpp"
#include "game_processor.hpp"

#include "drawer.hpp"

#include "text.hpp"

namespace {
	void MoveGameSceneEasy()
	{
		auto box = Eagle::GameObject::Create("FadeObject")->AttachComponent<BoxDrawer>(800.f, 600.f, 0x001e43);
		box->SetTransparency(0.f);
		box->SetOffset(400.f, 300.f);
		box->GetGameObject()->SetLayer(10);

		auto moveScene = []() {
			Eagle::SceneManager::CreateScene("GameScene");
			Eagle::SceneManager::SetChangeActiveScene("GameScene");

			GameConfig::SetGameLevel(GameLevel::Easy | GameConfig::GetGameLevel());

			Eagle::GameObject::Create("Processor")
				->AttachComponent<GameProcessor>();

			Eagle::SceneManager::DestroyScene("LevelScene");
		};

		box->DoFade(1.f, 0.5f).SetOnComplete(moveScene).SetOnKill([]() {Eagle::GameObject::Destroy("FadeObject"); });
	}

	void MoveGameSceneNormal()
	{
		auto box = Eagle::GameObject::Create("FadeObject")->AttachComponent<BoxDrawer>(800.f, 600.f, 0x001e43);
		box->SetTransparency(0.f);
		box->SetOffset(400.f, 300.f);
		box->GetGameObject()->SetLayer(10);

		auto moveScene = []() {
			Eagle::SceneManager::CreateScene("GameScene");
			Eagle::SceneManager::SetChangeActiveScene("GameScene");

			GameConfig::SetGameLevel(GameLevel::Normal | GameConfig::GetGameLevel());

			Eagle::GameObject::Create("Processor")
				->AttachComponent<GameProcessor>();

			Eagle::SceneManager::DestroyScene("LevelScene");
		};

		box->DoFade(1.f, 0.5f).SetOnComplete(moveScene).SetOnKill([]() {Eagle::GameObject::Destroy("FadeObject"); });
	}

	void MoveGameSceneHard()
	{
		auto box = Eagle::GameObject::Create("FadeObject")->AttachComponent<BoxDrawer>(800.f, 600.f, 0x001e43);
		box->SetTransparency(0.f);
		box->SetOffset(400.f, 300.f);
		box->GetGameObject()->SetLayer(10);

		auto moveScene = []() {
			Eagle::SceneManager::CreateScene("GameScene");
			Eagle::SceneManager::SetChangeActiveScene("GameScene");

			GameConfig::SetGameLevel(GameLevel::Hard | GameConfig::GetGameLevel());

			Eagle::GameObject::Create("Processor")
				->AttachComponent<GameProcessor>();

			Eagle::SceneManager::DestroyScene("LevelScene");
		};

		box->DoFade(1.f, 0.5f).SetOnComplete(moveScene).SetOnKill([]() {Eagle::GameObject::Destroy("FadeObject"); });
	}
}

void LevelProcessor::Awake()
{
	auto widget = Eagle::GameObject::Create("ButtonWidget")->AttachComponent<ButtonWidget>();

	//Easyモード

	auto center = GameConfig::GetWindowSizeF() * Eagle::Float2(0.5f, 0.5f);

	levelButton[0] = MakeButton("EasyButton", "Easy", "Consolas", 48, 300, 50);
	levelButton[0]->SetOnPressed(MoveGameSceneEasy);
	levelButton[0]->GetTransform()->SetPosition(center.x, center.y - 100.f, 0.f);
	levelButton[0]->GetBoxDrawer()->SetThickness(3.f);

	widget->Add(levelButton[0]);

	//Normalモード

	levelButton[1] = MakeButton("NormalButton", "Normal", "Consolas", 48, 300, 50);
	levelButton[1]->SetOnPressed(MoveGameSceneNormal);
	levelButton[1]->GetTransform()->SetPosition(center.x, center.y, 0.f);
	levelButton[1]->GetBoxDrawer()->SetThickness(3.f);

	widget->Add(levelButton[1]);

	//Hardモード

	levelButton[2] = MakeButton("HardButton", "Hard", "Consolas", 48, 300, 50);
	levelButton[2]->SetOnPressed(MoveGameSceneHard);
	levelButton[2]->GetTransform()->SetPosition(center.x, center.y + 100.f, 0.f);
	levelButton[2]->GetBoxDrawer()->SetThickness(3.f);

	widget->Add(levelButton[2]);

	for (auto& button : levelButton)
	{
		button->SetEnable(false);
	}

	debugModeText = Eagle::GameObject::Create()->AttachComponent<Text>("Debug Mode");
	debugModeText->GetTransform()->SetPosition(790, 22, 0);
	debugModeText->SetDrawPosition(DrawPosition::Right);
	debugModeText->SetFont("Consolas", 24);
	debugModeText->SetEnable(false);

	auto box = Eagle::GameObject::Create("FadeObject")->AttachComponent<BoxDrawer>(800.f, 600.f, 0x001e43);
	box->SetOffset(400.f, 300.f);
	box->GetGameObject()->SetLayer(10);
	box->DoFade(0.f, 0.25f).SetOnComplete([&]() 
		{
			for (auto& button : levelButton)
			{
				button->SetEnable(true);
			}
		})
		.SetOnKill([]() {Eagle::GameObject::Destroy("FadeObject"); });
}

void LevelProcessor::Start()
{
	SetBackgroundColor(0x00, 0x1e, 0x43);
}

void LevelProcessor::Update()
{
	if (Input::GetDown(Input::Key::Delete))
	{
		isDebugMode = !isDebugMode;
		GameConfig::SetGameLevel(isDebugMode ? GameLevel::Debug : static_cast<GameLevel>(0));
		debugModeText->SetEnable(isDebugMode);
	}
}
