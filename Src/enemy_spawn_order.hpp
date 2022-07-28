#pragma once
#include "enemy_spawner.hpp"

struct OrderParam {
	float activeTime = 1.f; //オーダーの有効時間
	float interval = 1.f;
	EnemyAct actType = EnemyAct::Rain;
};

class EnemySpawnOrder : public Eagle::Behavior, public Eagle::PauseAndResume {
private:
	template<typename Ty>
	using Que = std::queue<Ty>;

	struct SpawnerNode {
		EnemySpawner* spawner = nullptr;
		Que<OrderParam> orderQue{};
	};

public:
	EnemySpawnOrder();

	void Pause()override;

	void Resume()override;

	EnemySpawner* CreateSpawner(Eagle::Name _spawnerName, float _interval, EnemyAct _actType);

	void AddOrder(float _activeTime, float _interval, EnemyAct _actType, unsigned int _slot = 0);
	void AddOrder(OrderParam _param, unsigned int _slot = 0);

	void SetOrder(Que<OrderParam> _orderQue, unsigned int _slot = 0);

	void DestroySpawner(EnemySpawner* _spawner);
	void DestroySpawner(unsigned int _slot);

	EnemySpawner* GetSpawner(unsigned int _slot = 0)const noexcept;

private:
	Eagle::DynamicArray<SpawnerNode> spawnerList;

	void Awake()override;

	void LateUpdate()override;

	void OnDestroy()override;

	void ChangeOrder(SpawnerNode& node);
};