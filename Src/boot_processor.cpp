#include "boot_processor.hpp"
#include "eagle/scene_management.hpp"

#include "text.hpp"
#include "drawer.hpp"
#include "title_processor.hpp"
#include "player_move.hpp"
#include "player_shot.hpp"
#include "control_slow_time.hpp"

#include "sound_manager.hpp"

#include <fstream>
#include <filesystem>

namespace {
	void MoveTitleScene()
	{
		Eagle::SceneManager::CreateScene("TitleScene");
		Eagle::SceneManager::SetChangeActiveScene("TitleScene");
		Eagle::GameObject::Create("Processor")
			->AttachComponent<TitleProcessor>();
	}
}

void BootProcessor::Awake()
{
	titleText = Eagle::GameObject::Create("BootText")->AttachComponent<Text>(GameConfig::GetTitleName());

	titleText->SetFont("Century", 86);

	auto center = GameConfig::GetWindowSizeF() * Eagle::Float2(0.5f, 0.5f);

	titleText->GetTransform()->SetPosition(center.x, center.y, 0.f);
	titleText->SetTransparency(1.f);
	titleText->DoFade(0.f, 1.5f).SetDelay(2.5f).SetEase(Eagle::Ease::Cubic::In);

	float w = center.x * 2.f / static_cast<float>(fadeBox.Length());
	float h = center.y * 2.f;
	float offset = w * 0.5f;

	for (size_t i = 0; i + 1 < fadeBox.Length(); i += 2)
	{
		fadeBox[i] = Eagle::GameObject::Create()->AttachComponent<BoxDrawer>(w, h, 0x000000);
		fadeBox[i]->GetTransform()->SetPosition(center.x, center.y, 0.f);
		fadeBox[i]->SetOffset(offset, 0.f);
		fadeBox[i + 1] = Eagle::GameObject::Create()->AttachComponent<BoxDrawer>(w, h, 0x000000);
		fadeBox[i + 1]->GetTransform()->SetPosition(center.x, center.y, 0.f);
		fadeBox[i + 1]->SetOffset(-offset, 0.f);

		float delay = (1.f / fadeBox.Length()) * static_cast<float>(i);

		fadeBox[i]->DoFade(0.f, 1.f).SetDelay(delay);
		fadeBox[i + 1]->DoFade(0.f, 1.f).SetDelay(delay);

		offset += w;
	}

	auto filePath = std::filesystem::current_path();
	filePath += "\\Data\\keybind.cnf";

	std::ifstream ifs(filePath.c_str());

	if (!ifs)
		return;

	void (*read[])(int) = {
		[](int val) {PlayerMove::SetMoveUpKey(static_cast<Input::Key>(val)); },
		[](int val) {PlayerMove::SetMoveDownKey(static_cast<Input::Key>(val));},
		[](int val) {PlayerMove::SetMoveLeftKey(static_cast<Input::Key>(val));},
		[](int val) {PlayerMove::SetMoveRightKey(static_cast<Input::Key>(val));},
		[](int val) {ControlSlowTime::SetSlowKey(static_cast<Input::Key>(val));},
		[](int val) {PlayerShot::SetShotKey(static_cast<Input::Key>(val));}
	};

	unsigned idx = 0;
	Eagle::String buf;
	while (!ifs.eof() && idx < 6)
	{
		std::getline(ifs, buf);
		read[idx](std::stoi(buf));
		idx += 1;
	}
}

void BootProcessor::Start()
{
	Eagle::Invoke(4.f, MoveTitleScene);

	SoundManager::Load("se_laser01.mp3");
	SoundManager::Load("se_shot1.mp3");
	SoundManager::Load("se_beam2.mp3");
	SoundManager::Load("se_beam06.mp3", 8);
	SoundManager::Load("se_baaan1.mp3", 8);
	SoundManager::Load("se_damage5.mp3");
	SoundManager::Load("se_select10.mp3", 1);
	SoundManager::Load("se_pikon5.mp3");
	SoundManager::Load("bgm_way01.wav");
}
