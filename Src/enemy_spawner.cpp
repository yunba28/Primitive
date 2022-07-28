#include "enemy_spawner.hpp"
#include "interval.hpp"
#include "enemy_act_army.hpp"
#include "enemy_act_rain.hpp"
#include "enemy_act_curve.hpp"
#include "enemy_act_artillery.hpp"
#include "game_utility.hpp"

EnemySpawner::EnemySpawner()
	: Behavior(ID::Update)
	, enemyList()
	, spawnInterval(new Interval)
	, makeActor(EnemyAct::Rain)
	, isActive(true)
{
}

EnemySpawner::EnemySpawner(float _spawnInterval, EnemyAct _makeActor)
	: Behavior()
	, enemyList()
	, spawnInterval(new Interval)
	, makeActor(_makeActor)
	, isActive(true)
{
	spawnInterval->SetInterval(_spawnInterval);
}

EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::Pause()
{
	if (!isActive)
		return;

	isActive = !isActive;

	SetEnable(isActive);
}

void EnemySpawner::Resume()
{
	if (isActive)
		return;

	isActive = !isActive;

	SetEnable(isActive);
}

void EnemySpawner::OnDeath(Enemy* enemy)
{
	auto itr = std::find(enemyList.begin(), enemyList.end(), enemy);

	if (itr == enemyList.end())
		return;

	Eagle::GameObject::Destroy(enemy->GetGameObject());

	enemyList.Remove(itr);
}

void EnemySpawner::SetSpawnInterval(float interval)
{
	spawnInterval->SetInterval(interval);
}

void EnemySpawner::SetSpawnEnemyType(EnemyAct actorType)
{
	makeActor = actorType;
}

void EnemySpawner::Update()
{

	spawnInterval->Update();

	//インターバルが準備可能かどうか
	if (!spawnInterval->IsReady())
		return;

	spawnInterval->Ready();

	CreateEnemy(makeActor);
}

void MakePoint4FromTop(Eagle::Vector3 point[4], float offsetPos);
void MakePoint4FromLeft(Eagle::Vector3 point[4], float offsetPos);
void MakePoint4FromRight(Eagle::Vector3 point[4], float offsetPos);

Enemy* EnemySpawner::CreateEnemy(EnemyAct actor)
{
	Enemy* enemy = nullptr;
	void(*func[3])(Eagle::Vector3*, float) = { MakePoint4FromTop,MakePoint4FromLeft,MakePoint4FromRight };
	const int funcIdx = Eagle::Random::Range(0, 2);
	Eagle::Vector3 point[4];
	float t = 0.f;

	switch (actor)
	{
	case EnemyAct::Army1:
		func[funcIdx](point,100.f);
		for (int i = 0;i < 7;++i)
		{
			enemy = Enemy::Create(this, 10.f, 0xff0000);
			enemy->GetGameObject()->SetLayer(2);
			enemy->SetActor(enemy->GetGameObject()->AttachComponent<EnemyActArmy>(point, t, 5.f, ArmyType::Diffusion));
			t -= 0.05f;
			enemyList.Add(std::forward<Enemy*&&>(enemy));
		}
		break;
	case EnemyAct::Army2:
		func[funcIdx](point,100.f);
		for (int i = 0; i < 5; ++i)
		{
			enemy = Enemy::Create(this, 10.f, 0xff0000);
			enemy->GetGameObject()->SetLayer(2);
			enemy->SetActor(enemy->GetGameObject()->AttachComponent<EnemyActArmy>(point, t, 5.f, ArmyType::Trail));
			t -= 0.05f;
			enemyList.Add(std::forward<Enemy*&&>(enemy));
		}
		break;
	case EnemyAct::Rain:
		enemy = Enemy::Create(this, 10.f, 0xff0000);
		enemy->GetGameObject()->SetLayer(2);
		enemy->SetActor(enemy->GetGameObject()->AttachComponent<EnemyActRain>());
		enemyList.Add(std::forward<Enemy*&&>(enemy));
		break;
	case EnemyAct::Curve:
		enemy = Enemy::Create(this, 10.f, 0xff0000);
		enemy->GetGameObject()->SetLayer(2);
		enemy->SetActor(enemy->GetGameObject()->AttachComponent<EnemyActCurve>());
		enemy->SetArmor(3);
		enemyList.Add(std::forward<Enemy*&&>(enemy));
		break;
	case EnemyAct::Artillery:
		enemy = Enemy::Create(this, 10.f, 0xff0000);
		enemy->GetGameObject()->SetLayer(2);
		enemy->SetActor(enemy->GetGameObject()->AttachComponent<EnemyActArtillery>());
		enemy->SetArmor(750);
		enemyList.Add(std::forward<Enemy*&&>(enemy));
		break;
	default:
		enemy = Enemy::Create(this, 10.f, 0xff0000);
		enemy->GetGameObject()->SetLayer(2);
		enemy->SetActor(enemy->GetGameObject()->AttachComponent<EnemyActRain>());
		enemyList.Add(std::forward<Enemy*&&>(enemy));
		break;
	}

	return enemy;
}

void MakePoint4FromTop(Eagle::Vector3 point[4], float offsetPos)
{
	point[0] = {
		Eagle::Random::Range(FIELD_MIN_X,FIELD_MAX_X),
		-offsetPos,
		0.f
	};

	point[1] = {
		0.f,
		Eagle::Random::Range(FIELD_MIN_Y + 100.f,FIELD_MAX_Y - 100.f),
		0.f
	};

	point[2] = {
		0.f,
		Eagle::Random::Range(FIELD_MIN_Y + 100.f,FIELD_MAX_Y - 100.f),
		0.f
	};

	point[3] = {
		Eagle::Random::Range(FIELD_MIN_X,FIELD_MAX_X),
		0.f,
		0.f
	};

	const float LX = Eagle::Random::Range(FIELD_MIN_X, FIELD_MIN_X + 100.f);
	const float RX = Eagle::Random::Range(FIELD_MAX_X - 100.f, FIELD_MAX_X);

	if (Eagle::Random::Range01() > 0.5)
	{
		point[1].x = LX;
		point[2].x = RX;
	}
	else
	{
		point[1].x = RX;
		point[2].x = LX;
	}

	point[3].y = 600.f;
}

void MakePoint4FromLeft(Eagle::Vector3 point[4], float offsetPos)
{
	point[0] = {
		FIELD_MIN_X - offsetPos,
		Eagle::Random::Range(FIELD_MIN_Y,FIELD_MAX_Y),
		0.f
	};

	point[1] = {
		Eagle::Random::Range(FIELD_MIN_X + 100.f,FIELD_MAX_X - 100.f),
		0.f,
		0.f
	};

	point[2] = {
		Eagle::Random::Range(FIELD_MIN_X + 100.f,FIELD_MAX_X - 100.f),
		0.f,
		0.f
	};

	point[3] = {
		0.f,
		Eagle::Random::Range(FIELD_MIN_Y,FIELD_MAX_Y),
		0.f
	};

	const float TY = Eagle::Random::Range(FIELD_MIN_Y, FIELD_MIN_Y + 100.f);
	const float BY = Eagle::Random::Range(FIELD_MAX_Y - 100.f, FIELD_MAX_Y);

	if (Eagle::Random::Range01() > 0.5)
	{
		point[1].y = TY;
		point[2].y = BY;
	}
	else
	{
		point[1].y = TY;
		point[2].y = BY;
	}

	point[3].x = 600.f;
}

void MakePoint4FromRight(Eagle::Vector3 point[4], float offsetPos)
{
	point[0] = {
		FIELD_MAX_X + offsetPos,
		Eagle::Random::Range(FIELD_MIN_Y,FIELD_MAX_Y),
		0.f
	};

	point[1] = {
		Eagle::Random::Range(FIELD_MIN_X + 100.f,FIELD_MAX_X - 100.f),
		0.f,
		0.f
	};

	point[2] = {
		Eagle::Random::Range(FIELD_MIN_X + 100.f,FIELD_MAX_X - 100.f),
		0.f,
		0.f
	};

	point[3] = {
		0.f,
		Eagle::Random::Range(FIELD_MIN_Y,FIELD_MAX_Y),
		0.f
	};

	const float TY = Eagle::Random::Range(FIELD_MIN_Y, FIELD_MIN_Y + 100.f);
	const float BY = Eagle::Random::Range(FIELD_MAX_Y - 100.f, FIELD_MAX_Y);

	if (Eagle::Random::Range01() > 0.5)
	{
		point[1].y = TY;
		point[2].y = BY;
	}
	else
	{
		point[1].y = TY;
		point[2].y = BY;
	}

	point[3].x = 0.f;
}