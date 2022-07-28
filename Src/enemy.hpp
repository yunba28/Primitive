#pragma once
#include "eagle.hpp"

class EnemyActBase;
class CircleDrawer;

class EnemySpawner;

//[summary]
//エネミーに関するコンポーネントの管理を行う
//主にPause（一時停止）とResume（再開）
//生成にはEnemy::Create関数を使用してください
class Enemy : public Eagle::Behavior, public Eagle::PauseAndResume {
public:
	Enemy();
	~Enemy();

	void Pause()override;

	void Resume()override;

	void SetArmor(int _armor)noexcept;

	void SetActor(EnemyActBase* actor)noexcept;

	int GetArmor()const noexcept;

	static Enemy* Create(EnemySpawner* enemySpawner, float radius, Eagle::Color color, float shotInterval = 0.1f);

private:
	EnemySpawner* enemySpawner;
	EnemyActBase* enemyMove;
	int armor;

	void Update()override;
	void OnCollisionEnter(const Eagle::Collision& collision)override;
};