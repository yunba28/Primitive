#pragma once
#include "enemy.hpp"

enum class EnemyAct {
	Army1,
	Army2,
	Rain,
	Curve,
	Artillery
};

class Interval;

class EnemySpawner : public Eagle::Behavior, public Eagle::PauseAndResume {
public:
	EnemySpawner();
	EnemySpawner(float _spawnInterval, EnemyAct _makeActor);
	~EnemySpawner();

	void Pause()override;

	void Resume()override;

	void OnDeath(Enemy* enemy);

	void SetSpawnInterval(float interval);

	void SetSpawnEnemyType(EnemyAct actorType);

private:
	Eagle::List<Enemy*> enemyList;
	UNIQUE(Interval) spawnInterval;
	EnemyAct makeActor;
	bool isActive;

	void Update()override;
	Enemy* CreateEnemy(EnemyAct actor);
};