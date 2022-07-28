#include "title_processor.hpp"
#include "eagle/scene_management.hpp"

#include "button.hpp"
#include "button_widget.hpp"
#include "level_processor.hpp"
#include "key_config_processor.hpp"

#include "drawer.hpp"

#include "text.hpp"

#include "text_ex.hpp"

namespace {
	void MoveLevelScene()
	{
		Eagle::SceneManager::CreateScene("LevelScene");
		Eagle::SceneManager::SetChangeActiveScene("LevelScene");

		Eagle::SceneManager::DestroyScene("TitleScene");

		Eagle::GameObject::Create("Processor")
			->AttachComponent<LevelProcessor>();
	}

	void MoveKeyConfigScene()
	{
		Eagle::SceneManager::CreateScene("KeyConfigScene");
		Eagle::SceneManager::SetChangeActiveScene("KeyConfigScene");

		Eagle::SceneManager::DestroyScene("TitleScene");

		Eagle::GameObject::Create("Processor")
			->AttachComponent<KeyConfigProcessor>();
	}

	void FadeScene()
	{
		auto box = Eagle::GameObject::Create("FadeObject")->AttachComponent<BoxDrawer>(800.f, 600.f, 0x001e43);
		box->SetTransparency(0.f);
		box->SetOffset(400.f, 300.f);
		box->GetGameObject()->SetLayer(10);
		box->DoFade(1.f, 0.25f).SetOnComplete(MoveLevelScene).SetOnKill([]() {Eagle::GameObject::Destroy("FadeObject"); });
	}
}

void TitleProcessor::Awake()
{
	auto text = Eagle::GameObject::Create()->AttachComponent<TextEx>();
	text->GetTransform()->SetPosition(180, -100, 0);
	text->SetText("Primitive");
	text->SetFont("Century", 96);
	float delay = 0.f;
	for (auto& data : text->GetCharaDataList())
	{
		data.text->GetTransform()->DoMoveY(200.f, 1.f).SetDelay(delay).SetEase(Eagle::GetEase(Eagle::EaseType::BounceOut));
		delay += 1.f / 10.f;
	}

	auto widget = Eagle::GameObject::Create("ButtonWidget")->AttachComponent<ButtonWidget>();

	auto center = GameConfig::GetWindowSizeF() * Eagle::Float2(0.5f, 0.5f);

	//スタートボタンの設定

	auto button = MakeButton("StartButton", "Start", "Consolas", 48, 300, 50);
	button->SetOnPressed(FadeScene);
	button->GetTransform()->SetPosition(center.x, center.y - 50.f, 0.f);
	button->GetBoxDrawer()->SetThickness(3.f);

	widget->Add(button);

	//オプションボタンの設定

	button = MakeButton("OptionButton", "KeyConfig", "Consolas", 48, 300, 50);
	button->SetOnPressed(MoveKeyConfigScene);
	button->GetTransform()->SetPosition(center.x, center.y + 50.f, 0.f);
	button->GetBoxDrawer()->SetThickness(3.f);

	widget->Add(button);

	//クウィット（終了）ボタンの設定

	button = MakeButton("QuitButton", "Quit", "Consolas", 48, 300, 50);
	button->SetOnPressed(
		[]() {
			Eagle::System::PlayQuit();
		}
	);
	button->GetTransform()->SetPosition(center.x, center.y + 150.f, 0.f);
	button->GetBoxDrawer()->SetThickness(3.f);

	widget->Add(button);
}

void TitleProcessor::Start()
{
	SetBackgroundColor(0x00, 0x1e, 0x43);
}
