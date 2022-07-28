#include "debug_viewer.hpp"
#include "player.hpp"
#include "text.hpp"

DebugViewer::DebugViewer(Player* _player)
	: Behavior(ID::Update)
	, player(_player)
	, objectView(nullptr)
	, playerView(nullptr)
{
}

void DebugViewer::Awake()
{
	auto debugText = Eagle::GameObject::Create("DebugText")->AttachComponent<Text>("Debug");
	debugText->SetFont("Consolas", 24);
	debugText->GetTransform()->SetPosition(35, 14, 0);
	debugText->GetGameObject()->SetLayer(5);

	objectView = Eagle::GameObject::Create("ObjectViewText")->AttachComponent<Text>();
	objectView->SetFont("Consolas", 24);
	objectView->GetTransform()->SetPosition(68, 44, 0);
	objectView->GetGameObject()->SetLayer(5);

	playerView = Eagle::GameObject::Create("PlayerPositionViewText")->AttachComponent<Text>();
	playerView->SetFont("Consolas", 24);
	playerView->GetTransform()->SetPosition(62, 74, 0);
	playerView->GetGameObject()->SetLayer(5);
}

void DebugViewer::Update()
{
	objectView->SetText(Eagle::Format("Object %3d", GetGameObject()->GetScene()->GetGameObjectCount()));
	auto pos = player->GetTransform()->GetWorldPosition(false);
	playerView->SetText(Eagle::Format("X %6.3f\nY %6.3f", pos.x, pos.y));
}
