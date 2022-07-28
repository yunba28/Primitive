#include "game_processor.hpp"

#include "player.hpp"
#include "player_move.hpp"
#include "player_shot.hpp"
#include "player_armor_gauge.hpp"
#include "enemy_spawn_order.hpp"
#include "bullet_commander.hpp"
#include "effect_hit.hpp"
#include "text.hpp"
#include "gauge.hpp"
#include "time_counter.hpp"
#include "fps_viewer.hpp"
#include "game_score.hpp"
#include "game_score_viewer.hpp"
#include "drawer.hpp"
#include "screen_shaker.hpp"
#include "debug_viewer.hpp"

#include "menu_dialog.hpp"
#include "gameover_dialog.hpp"

#include "sound_manager.hpp"

#include <winsock.h>

GameProcessor::GameProcessor()
	: Behavior(ID::Update | ID::LateUpdate)
	, player(nullptr)
	, enemySpawnOrder(nullptr)
	, bulletCommander(nullptr)
	, timeCounter(nullptr)
	, menuDialog(nullptr)
{
}

GameProcessor::~GameProcessor()
{
	SoundManager::Stop("bgm_way01.wav");
}

void GameProcessor::Awake()
{
	player = Player::Create("Player01", 400.f, 7.5f, 0xfffafa, 0.05f); //Player関連のコンポーネントを一気に生成
	player->GetGameObject()->SetLayer(3);
	player->GetTransform()->DoLocalMoveY(-200.f, 2.0f).SetEase(Eagle::GetEase(Eagle::EaseType::QuartOut));

	auto armorGauge = Eagle::GameObject::Create("PlayerArmorGauge")->AttachComponent<PlayerArmorGauge>(player);
	armorGauge->GetGauge()->SetSize(180.f, 10.f);
	armorGauge->GetTransform()->SetPosition(900, 30, 0);
	armorGauge->GetTransform()->DoMoveX(-200.f, 1.f).SetEase(Eagle::GetEase(Eagle::EaseType::QuadOut));

	auto armorText = Eagle::GameObject::Create("PlayerArmorGaugeText")->AttachComponent<Text>("Armor");
	armorText->SetFont("Consolas", 24);
	armorText->SetDrawPosition(DrawPosition::Left);
	armorText->GetTransform()->SetPosition(810,14,0);
	armorText->GetTransform()->DoMoveX(-200.f, 1.f).SetEase(Eagle::GetEase(Eagle::EaseType::QuadOut));
	armorText->GetGameObject()->SetLayer(5);

	auto powerText = Eagle::GameObject::Create("PowerGaugeText")->AttachComponent<Text>("Power");
	powerText->SetFont("Consolas", 24);
	powerText->SetDrawPosition(DrawPosition::Left);
	powerText->GetTransform()->SetPosition(810, 48, 0);
	powerText->GetTransform()->DoMoveX(-200.f, 1.f).SetEase(Eagle::GetEase(Eagle::EaseType::QuadOut));
	powerText->GetGameObject()->SetLayer(5);
	
	float activeExt = 1.f;
	if (GameConfig::IsEnableGameLevel(GameLevel::Easy))
	{
		player->SetDamageCutRate(0.8f);
	}
	else if (GameConfig::IsEnableGameLevel(GameLevel::Easy))
	{
		player->SetDamageCutRate(0.5f);
		activeExt = 1.2f;
	}
	else if (GameConfig::IsEnableGameLevel(GameLevel::Easy))
	{
		player->SetDamageCutRate(0.0f);
		activeExt = 1.35f;
	}

	if (GameConfig::IsEnableGameLevel(GameLevel::Debug))
	{
		player->SetDamageCutRate(1.0f);

		DebugInit();
	}
	
	enemySpawnOrder = Eagle::GameObject::Create("EnemySpawnOrder01")->AttachComponent<EnemySpawnOrder>();
	enemySpawnOrder->CreateSpawner("EnemySpawnOrder02", 2.f, EnemyAct::Army1);

	////slot0
	enemySpawnOrder->AddOrder(2.5f, 5.5f, EnemyAct::Rain);
	enemySpawnOrder->AddOrder(10.f * activeExt, 0.5f, EnemyAct::Rain);
	enemySpawnOrder->AddOrder(20.f * activeExt, 2.f, EnemyAct::Army1);
	enemySpawnOrder->AddOrder(10.f * activeExt, 2.f, EnemyAct::Curve);
	enemySpawnOrder->AddOrder(20.f * activeExt, 1.f, EnemyAct::Rain);
	enemySpawnOrder->AddOrder(20.f * activeExt, 1.f, EnemyAct::Curve);
	enemySpawnOrder->AddOrder(6.f, 5.0f, EnemyAct::Artillery);

	////slot1
	enemySpawnOrder->AddOrder(10.f * activeExt + 2.5f, 100.f, EnemyAct::Army1, 1);
	enemySpawnOrder->AddOrder(20.f * activeExt, 5.f, EnemyAct::Curve, 1);
	enemySpawnOrder->AddOrder(10.f * activeExt, 100.f, EnemyAct::Army2, 1);
	enemySpawnOrder->AddOrder(20.f * activeExt, 4.f, EnemyAct::Army2, 1);
								  
	bulletCommander = Eagle::GameObject::Create("BulletCommander")->AttachComponent<BulletCommander>();

	timeCounter = Eagle::GameObject::Create("GameTimer")->AttachComponent<TimeCounter>("Consolas", 24, 0xffffff);
	timeCounter->GetTransform()->SetPosition(400, 12-200, 0);
	timeCounter->GetTransform()->DoMoveY(200.f, 1.f).SetEase(Eagle::GetEase(Eagle::EaseType::QuadOut));
	timeCounter->GetGameObject()->SetLayer(4);

	auto fpsViewer = Eagle::GameObject::Create("FpsViewer")->AttachComponent<FpsViewer>("Consolas", 24, 0xffffff);
	fpsViewer->GetTransform()->SetPosition(990, 580, 0);
	fpsViewer->GetTransform()->DoMoveX(-200.f, 1.f).SetEase(Eagle::GetEase(Eagle::EaseType::QuadOut));
	fpsViewer->GetGameObject()->SetLayer(5);

	auto scoreViewer = Eagle::GameObject::Create("ScoreViewer")->AttachComponent<GameScoreViewer>();
	scoreViewer->GetHighScoreTitle()->GetTransform()->SetPosition(810, 90, 0);
	scoreViewer->GetHighScoreTitle()->GetTransform()->DoMoveX(-200.f, 1.f).SetEase(Eagle::GetEase(Eagle::EaseType::QuadOut));
	scoreViewer->GetHighScoreValue()->GetTransform()->SetPosition(990, 90, 0);
	scoreViewer->GetHighScoreValue()->GetTransform()->DoMoveX(-200.f, 1.f).SetEase(Eagle::GetEase(Eagle::EaseType::QuadOut));
	scoreViewer->GetScoreTitle()->GetTransform()->SetPosition(810, 140, 0);
	scoreViewer->GetScoreTitle()->GetTransform()->DoMoveX(-200.f, 1.f).SetEase(Eagle::GetEase(Eagle::EaseType::QuadOut));
	scoreViewer->GetScoreValue()->GetTransform()->SetPosition(990, 140, 0);
	scoreViewer->GetScoreValue()->GetTransform()->DoMoveX(-200.f, 1.f).SetEase(Eagle::GetEase(Eagle::EaseType::QuadOut));

	menuDialog = Eagle::GameObject::Create("MenuDialog")->AttachComponent<MenuDialog>();
	menuDialog->SetEnable(false);
	menuDialog->GetGameObject()->SetLayer(5);

	Eagle::PauseAndResume::PauseAll();

	//ゲーム画面の左扉
	auto leftBox = Eagle::GameObject::Create("LeftBox");
	leftBox->AttachComponent<BoxDrawer>(400.f, 600.f, Eagle::Color::FromHsv(0.f, 0.f, 0.6f));
	leftBox->GetTransform()->SetPosition(200, 300, 0);
	leftBox->SetLayer(4);
	leftBox->GetTransform()->DoMoveX(-200.f,1.f)
		.SetDelay(0.5f)
		.SetEase(Eagle::Ease::Bounce::Out)
		.SetOnComplete(
			[]() {
				Eagle::PauseAndResume::ResumeAll();
			}
	);

	//ゲーム画面の右扉
	auto rightBox = Eagle::GameObject::Create("RightBox");
	rightBox->AttachComponent<BoxDrawer>(400.f, 600.f, Eagle::Color::FromHsv(0.f, 0.f, 0.6f));
	rightBox->GetTransform()->SetPosition(600, 300, 0);
	rightBox->SetLayer(4);

	rightBox->GetTransform()->DoMoveX(200.f, 1.f)
		.SetDelay(0.5f)
		.SetEase(Eagle::Ease::Bounce::Out);

	auto fadeBox = Eagle::GameObject::Create("FadeObject")->AttachComponent<BoxDrawer>(800.f, 600.f, 0x001e43);
	fadeBox->SetOffset(400.f, 300.f);
	fadeBox->GetGameObject()->SetLayer(10);
	fadeBox->DoFade(0.f, 0.5f).SetOnKill([]() {Eagle::GameObject::Destroy("FadeObject"); });

	auto frameBox = Eagle::GameObject::Create("FrameBox")->AttachComponent<BoxDrawer>(397.f, 597.f, 0xff0000);
	frameBox->SetOffset(400.f, 300.f);
	frameBox->GetGameObject()->SetLayer(5);
	frameBox->SetFill(false);
	frameBox->SetThickness(6.f);
	frameBox->SetTransparency(0.f);
}

void GameProcessor::Start()
{
	SetBackgroundColor(0x00, 0x1e, 0x43);
	GameScore::SetScore(0);
	SoundManager::Play("bgm_way01.wav", DX_PLAYTYPE_LOOP, 1.f);
}

void GameProcessor::Update()
{

	if (Input::GetDown(Input::Key::Escape))
	{
		Eagle::PauseAndResume::PauseAll();
		menuDialog->SetEnable(true);
	}
}

void GameProcessor::LateUpdate()
{
	auto createDialog = []() {
		Eagle::PauseAndResume::PauseAll();
		Eagle::GameObject::Create("GameOverDialog")->AttachComponent<GameOverDialog>();
		Eagle::Time::SetTimeFactor(1.0);
	};

	if (player->GetArmor() <= 0.f)
	{
		EffectHit::Create(player->GetTransform()->GetWorldPosition(false), 24.f, 1.f, 0xfffafa);
		player->GetGameObject()->SetActive(false);
		Eagle::Time::SetTimeFactor(0.05);
		Eagle::Invoke(0.255f, createDialog);
		SetEnable(false);
	}
}

void GameProcessor::DebugInit()
{
	Eagle::GameObject::Create("DebugViewer")->AttachComponent<DebugViewer>(player);
}
