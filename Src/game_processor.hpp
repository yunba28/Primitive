#pragma once
#include "game_utility.hpp"

class Player;
class EnemySpawner;
class EnemySpawnOrder;
class BulletCommander;
class TimeCounter;
class MenuDialog;
class GameOverDialog;

//[summary]
//Bootシーンの処理を行うコンポーネント
class GameProcessor final : public Eagle::Behavior {
public:
	GameProcessor();
	~GameProcessor();

private:
	Player* player;
	EnemySpawnOrder* enemySpawnOrder;
	BulletCommander* bulletCommander;
	TimeCounter* timeCounter;
	MenuDialog* menuDialog;

	void Awake()override final;
	void Start()override final;
	void Update()override final;
	void LateUpdate()override final;

	void DebugInit();
};