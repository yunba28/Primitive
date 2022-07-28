#include "menu_dialog.hpp"
#include "button.hpp"
#include "button_widget.hpp"
#include "text.hpp"
#include "drawer.hpp"

#include "title_processor.hpp"

#include "eagle/scene_management.hpp"

namespace {
	void MoveTitleScene() {
		Eagle::SceneManager::CreateScene("TitleScene");
		Eagle::SceneManager::SetChangeActiveScene("TitleScene");
		Eagle::GameObject::Create("Processor")
			->AttachComponent<TitleProcessor>();

		Eagle::SceneManager::DestroyScene("GameScene");

		SetBackgroundColor(0x00, 0x1e, 0x43);
		Eagle::Time::SetTimeFactor(1.f);
	}
}

MenuDialog::MenuDialog()
	: Behavior(ID::NoBehave)
	, dialogBox(nullptr)
	, buttonWidget(nullptr)
	, button()
{
}

void MenuDialog::Awake()
{
	dialogBox = GetGameObject()->AttachComponent<BoxDrawer>();
	dialogBox->SetSize(300, 300);
	dialogBox->SetOffset(150, 150); //他のボタンの位置を参照するために矩形の座標を左上に調整
	dialogBox->GetTransform()->SetPosition(250,150,0); //真ん中に出るようにする
	dialogBox->SetColor(0x555555);
	dialogBox->SetTransparency(0.8f);

	buttonWidget = Eagle::GameObject::Create("ButtonWidget")->AttachComponent<ButtonWidget>();

	button.close = MakeButton(GetGameObject()->GetName() + "_CloseButton", "X", "Consolas", 24, 32, 32);
	button.close->GetGameObject()->SetParent(GetGameObject());
	button.close->SetUseFadeColor(false);
	button.close->GetTransform()->SetPosition(284, 16, 0); //ダイアログボックスの右上
	button.close->GetBoxDrawer()->SetFill(true);
	button.close->GetText()->SetColor(0);
	button.close->SetOnPressed(
		[&]() 
		{
			Eagle::PauseAndResume::ResumeAll();
			this->SetEnable(false);
		}
	);
	button.close->GetGameObject()->SetLayer(6);

	button.toTitle = MakeButton(GetGameObject()->GetName() + "_ToTitleButton", "Title", "Consolas", 48, 200, 50);
	button.toTitle->GetGameObject()->SetParent(GetGameObject());
	button.toTitle->SetUseFadeColor(false);
	button.toTitle->GetTransform()->SetPosition(150, 150, 0);
	button.toTitle->GetBoxDrawer()->SetFill(true);
	button.toTitle->GetText()->SetColor(0);
	button.toTitle->GetGameObject()->SetLayer(6);
	button.toTitle->SetOnPressed(MoveTitleScene);

	buttonWidget->Add(button.close);
	buttonWidget->Add(button.toTitle);
}

void MenuDialog::OnEnable()
{
	Eagle::Time::SetTimeFactor(1.f);
	dialogBox->SetEnable(true);
	buttonWidget->SetEnable(true);
	button.SetEnable(true);
}

void MenuDialog::OnDisable()
{
	dialogBox->SetEnable(false);
	buttonWidget->SetEnable(false);
	button.SetEnable(false);
}

void MenuDialog::ButtonList::SetEnable(bool flag)
{
	close->GetGameObject()->SetActive(flag);
	toTitle->GetGameObject()->SetActive(flag);
}
