#include "key_config_processor.hpp"

#include "eagle/scene_management.hpp"

#include "drawer.hpp"
#include "text.hpp"
#include "button.hpp"
#include "button_widget.hpp"
#include "key_config_input.hpp"

#include "title_processor.hpp"

#include "player_move.hpp"
#include "player_shot.hpp"
#include "control_slow_time.hpp"

#include <filesystem>

namespace {
	void MoveTitleScene() {
		Eagle::SceneManager::CreateScene("TitleScene");
		Eagle::SceneManager::SetChangeActiveScene("TitleScene");
		Eagle::GameObject::Create("Processor")
			->AttachComponent<TitleProcessor>();
		Eagle::SceneManager::DestroyScene("KeyConfigScene");
	}
}

KeyConfigProcessor::KeyConfigProcessor()
	: Behavior(ID::NoBehave)
	, config()
{
}

void KeyConfigProcessor::Awake()
{
	//read data

	auto filePath = std::filesystem::current_path();
	filePath += "\\Data\\keybind.cnf";

	std::ifstream ifs(filePath.c_str());
	Eagle::DynamicArray<int> data;
	if (ifs)
	{
		Eagle::String buf;
		while (!ifs.eof())
		{
			std::getline(ifs, buf);
			if (buf == "")
				continue;
			data.Add(std::stoi(buf));
		}
	}
	else
	{
		data.Base() = { 17,31,30,32,42,28 };
	}

	//config

	const float unitHeight = 40.f;
	const float offsetY = 50.f;
	const Eagle::Color stayColor = 0xcccccc;
	const Eagle::Color hoverColor = 0xffffff;
	float posX = 24.f;
	float posY = unitHeight + offsetY;
	float inputerPosX = 250.f;
	unsigned fontSize = 32;
	unsigned configIdx = 0;

	auto widget = Eagle::GameObject::Create()->AttachComponent<ButtonWidget>();

	//background-move config

	auto moveConfigBackground = Eagle::GameObject::Create("MoveConfigBackground")->AttachComponent<BoxDrawer>(380, 300, 0x000000);
	moveConfigBackground->SetTransparency(0.5f);
	moveConfigBackground->SetOffset(190.f, 150.f);
	moveConfigBackground->GetTransform()->SetPosition(12.f, 12.f, 0.f);

	//title-move config

	auto moveConfigTitle = Eagle::GameObject::Create("MoveConfigTitle")->AttachComponent<Text>("KeyConfig");
	moveConfigTitle->GetTransform()->SetPosition(212.f, 48.f, 0.f);
	moveConfigTitle->SetFont("", 32);

	//Move-Up

	config[configIdx].inputer = Eagle::GameObject::Create("UpMoveConfig")->AttachComponent<KeyConfigInput>(static_cast<Input::Key>(data[configIdx]));
	config[configIdx].inputer->SetEnabledColor(hoverColor);
	config[configIdx].inputer->SetDisabledColor(stayColor);
	config[configIdx].inputer->SetOutputFunction([](Input::Key key) {PlayerMove::SetMoveUpKey(key); });
	config[configIdx].inputer->GetTransform()->SetPosition(inputerPosX, posY, 0);
	config[configIdx].inputer->GetText()->SetDrawPosition(DrawPosition::Left);
	config[configIdx].inputer->SetWidget(widget);
	config[configIdx].button = MakeButton("UpMoveButton", "Move-Up", "", fontSize, 300, fontSize);
	config[configIdx].button->GetBoxDrawer()->SetEnable(false);
	config[configIdx].button->SetStayColor(stayColor);
	config[configIdx].button->SetHoverColor(hoverColor);
	config[configIdx].button->SetUseFadeScale(false);
	config[configIdx].button->SetOnPressed([&]() {config[0].inputer->SetEnable(true); });
	config[configIdx].button->GetTransform()->SetPosition(posX, posY, 0);
	config[configIdx].button->GetText()->SetDrawPosition(DrawPosition::Left);
	config[configIdx].inputer->GetGameObject()->SetLayer(3);
	config[configIdx].button->GetGameObject()->SetLayer(3);
	widget->Add(config[configIdx].button);
	posY += unitHeight;
	configIdx += 1;

	//Move-Down

	config[configIdx].inputer = Eagle::GameObject::Create("DownMoveConfig")->AttachComponent<KeyConfigInput>(static_cast<Input::Key>(data[configIdx]));
	config[configIdx].inputer->SetEnabledColor(hoverColor);
	config[configIdx].inputer->SetDisabledColor(stayColor);
	config[configIdx].inputer->SetOutputFunction([](Input::Key key) {PlayerMove::SetMoveDownKey(key); });
	config[configIdx].inputer->GetTransform()->SetPosition(inputerPosX, posY, 0);
	config[configIdx].inputer->GetText()->SetDrawPosition(DrawPosition::Left);
	config[configIdx].inputer->SetWidget(widget);
	config[configIdx].button = MakeButton("DownMoveButton", "Move-Down", "", fontSize, 300, fontSize);
	config[configIdx].button->GetBoxDrawer()->SetEnable(false);
	config[configIdx].button->SetStayColor(stayColor);
	config[configIdx].button->SetHoverColor(hoverColor);
	config[configIdx].button->SetUseFadeScale(false);
	config[configIdx].button->SetOnPressed([&]() {config[1].inputer->SetEnable(true); });
	config[configIdx].button->GetTransform()->SetPosition(posX, posY, 0);
	config[configIdx].button->GetText()->SetDrawPosition(DrawPosition::Left);
	config[configIdx].inputer->GetGameObject()->SetLayer(3);
	config[configIdx].button->GetGameObject()->SetLayer(3);
	widget->Add(config[configIdx].button);
	posY += unitHeight;
	configIdx += 1;

	//Move-left

	config[configIdx].inputer = Eagle::GameObject::Create("LeftMoveConfig")->AttachComponent<KeyConfigInput>(static_cast<Input::Key>(data[configIdx]));
	config[configIdx].inputer->SetEnabledColor(hoverColor);
	config[configIdx].inputer->SetDisabledColor(stayColor);
	config[configIdx].inputer->SetOutputFunction([](Input::Key key) {PlayerMove::SetMoveLeftKey(key); });
	config[configIdx].inputer->GetTransform()->SetPosition(inputerPosX, posY, 0);
	config[configIdx].inputer->GetText()->SetDrawPosition(DrawPosition::Left);
	config[configIdx].inputer->SetWidget(widget);
	config[configIdx].button = MakeButton("LeftMoveButton", "Move-Left", "", fontSize, 300, fontSize);
	config[configIdx].button->GetBoxDrawer()->SetEnable(false);
	config[configIdx].button->SetStayColor(stayColor);
	config[configIdx].button->SetHoverColor(hoverColor);
	config[configIdx].button->SetUseFadeScale(false);
	config[configIdx].button->SetOnPressed([&]() {config[2].inputer->SetEnable(true); });
	config[configIdx].button->GetTransform()->SetPosition(posX, posY, 0);
	config[configIdx].button->GetText()->SetDrawPosition(DrawPosition::Left);
	config[configIdx].inputer->GetGameObject()->SetLayer(3);
	config[configIdx].button->GetGameObject()->SetLayer(3);
	widget->Add(config[configIdx].button);
	posY += unitHeight;
	configIdx += 1;

	//Move-Right

	config[configIdx].inputer = Eagle::GameObject::Create("RightMoveConfig")->AttachComponent<KeyConfigInput>(static_cast<Input::Key>(data[configIdx]));
	config[configIdx].inputer->SetEnabledColor(hoverColor);
	config[configIdx].inputer->SetDisabledColor(stayColor);
	config[configIdx].inputer->SetOutputFunction([](Input::Key key) {PlayerMove::SetMoveRightKey(key); });
	config[configIdx].inputer->GetTransform()->SetPosition(inputerPosX, posY, 0);
	config[configIdx].inputer->GetText()->SetDrawPosition(DrawPosition::Left);
	config[configIdx].inputer->SetWidget(widget);
	config[configIdx].button = MakeButton("RightMoveButton", "Move-Right", "", fontSize, 300, fontSize);
	config[configIdx].button->GetBoxDrawer()->SetEnable(false);
	config[configIdx].button->SetStayColor(stayColor);
	config[configIdx].button->SetHoverColor(hoverColor);
	config[configIdx].button->SetUseFadeScale(false);
	config[configIdx].button->SetOnPressed([&]() {config[3].inputer->SetEnable(true); });
	config[configIdx].button->GetTransform()->SetPosition(posX, posY, 0);
	config[configIdx].button->GetText()->SetDrawPosition(DrawPosition::Left);
	config[configIdx].inputer->GetGameObject()->SetLayer(3);
	config[configIdx].button->GetGameObject()->SetLayer(3);
	widget->Add(config[configIdx].button);
	posY += unitHeight;
	configIdx += 1;

	//Move-Slow

	config[configIdx].inputer = Eagle::GameObject::Create("SlowMoveConfig")->AttachComponent<KeyConfigInput>(static_cast<Input::Key>(data[configIdx]));
	config[configIdx].inputer->SetEnabledColor(hoverColor);
	config[configIdx].inputer->SetDisabledColor(stayColor);
	config[configIdx].inputer->SetOutputFunction([](Input::Key key) {ControlSlowTime::SetSlowKey(key); });
	config[configIdx].inputer->GetTransform()->SetPosition(inputerPosX, posY, 0);
	config[configIdx].inputer->GetText()->SetDrawPosition(DrawPosition::Left);
	config[configIdx].inputer->SetWidget(widget);
	config[configIdx].button = MakeButton("SlowMoveButton", "Move-Slow", "", fontSize, 300, fontSize);
	config[configIdx].button->GetBoxDrawer()->SetEnable(false);
	config[configIdx].button->SetStayColor(stayColor);
	config[configIdx].button->SetHoverColor(hoverColor);
	config[configIdx].button->SetUseFadeScale(false);
	config[configIdx].button->SetOnPressed([&]() {config[4].inputer->SetEnable(true); });
	config[configIdx].button->GetTransform()->SetPosition(posX, posY, 0);
	config[configIdx].button->GetText()->SetDrawPosition(DrawPosition::Left);
	config[configIdx].inputer->GetGameObject()->SetLayer(3);
	config[configIdx].button->GetGameObject()->SetLayer(3);
	widget->Add(config[configIdx].button);
	posY += unitHeight;
	configIdx += 1;

	//ShotAction

	config[configIdx].inputer = Eagle::GameObject::Create("ShotConfig")->AttachComponent<KeyConfigInput>(static_cast<Input::Key>(data[configIdx]));
	config[configIdx].inputer->SetEnabledColor(hoverColor);
	config[configIdx].inputer->SetDisabledColor(stayColor);
	config[configIdx].inputer->SetOutputFunction([](Input::Key key) {PlayerShot::SetShotKey(key); });
	config[configIdx].inputer->GetTransform()->SetPosition(inputerPosX, posY, 0);
	config[configIdx].inputer->GetText()->SetDrawPosition(DrawPosition::Left);
	config[configIdx].inputer->SetWidget(widget);
	config[configIdx].button = MakeButton("ShotButton", "Shot", "", fontSize, 300, fontSize);
	config[configIdx].button->GetBoxDrawer()->SetEnable(false);
	config[configIdx].button->SetStayColor(stayColor);
	config[configIdx].button->SetHoverColor(hoverColor);
	config[configIdx].button->SetUseFadeScale(false);
	config[configIdx].button->SetOnPressed([&]() {config[5].inputer->SetEnable(true); });
	config[configIdx].button->GetTransform()->SetPosition(posX, posY, 0);
	config[configIdx].button->GetText()->SetDrawPosition(DrawPosition::Left);
	config[configIdx].inputer->GetGameObject()->SetLayer(3);
	config[configIdx].button->GetGameObject()->SetLayer(3);
	widget->Add(config[configIdx].button);
	posY += unitHeight;
	configIdx += 1;

	posY += unitHeight;

	auto writeConfigData = [&]() {
		auto filePath = std::filesystem::current_path();
		filePath += "\\Data\\keybind.cnf";

		std::ofstream ofs(filePath.c_str());

		for (const auto& node : config)
		{
			ofs << static_cast<int>(node.inputer->GetCurrentKey()) << std::endl;
		}

		MoveTitleScene();
	};

	auto exit = MakeButton("ReturnButton", "Return", "", 28, 300, fontSize);
	exit->GetBoxDrawer()->SetEnable(false);
	exit->SetStayColor(stayColor);
	exit->SetHoverColor(hoverColor);
	exit->SetUseFadeScale(false);
	exit->SetOnPressed(writeConfigData);
	exit->GetTransform()->SetPosition(posX, posY, 0);
	exit->GetText()->SetDrawPosition(DrawPosition::Left);
	exit->GetGameObject()->SetLayer(3);
	widget->Add(exit);
}
