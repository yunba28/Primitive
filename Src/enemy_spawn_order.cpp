#include "enemy_spawn_order.hpp"

EnemySpawnOrder::EnemySpawnOrder()
	: Behavior(ID::LateUpdate)
	, PauseAndResume()
	, spawnerList()
{
}

void EnemySpawnOrder::Pause()
{
	SetEnable(false);
}

void EnemySpawnOrder::Resume()
{
	SetEnable(true);
}

EnemySpawner* EnemySpawnOrder::CreateSpawner(Eagle::Name _spawnerName, float _interval, EnemyAct _actType)
{
	SpawnerNode node;
	node.spawner = Eagle::GameObject::Create(_spawnerName)->AttachComponent<EnemySpawner>(_interval, _actType);
	spawnerList.Add(node);
	return spawnerList.Back().spawner;
}

void EnemySpawnOrder::AddOrder(float _activeTime, float _interval, EnemyAct _actType, unsigned int _slot)
{
	if (spawnerList.Length() <= _slot)
		return;

	spawnerList[_slot].orderQue.push(OrderParam{ _activeTime,_interval,_actType });

	if (spawnerList[_slot].orderQue.size() == 1)
	{
		OrderParam param = spawnerList[_slot].orderQue.front();
		spawnerList[_slot].spawner->SetSpawnInterval(param.interval);
		spawnerList[_slot].spawner->SetSpawnEnemyType(param.actType);
		spawnerList[_slot].spawner->GetGameObject()->SetActive(true);
	}
}

void EnemySpawnOrder::AddOrder(OrderParam _param, unsigned int _slot)
{
	if (spawnerList.Length() <= _slot)
		return;

	spawnerList[_slot].orderQue.push(_param);

	if (spawnerList[_slot].orderQue.size() == 1)
	{
		OrderParam param = spawnerList[_slot].orderQue.front();
		spawnerList[_slot].spawner->SetSpawnInterval(param.interval);
		spawnerList[_slot].spawner->SetSpawnEnemyType(param.actType);
		spawnerList[_slot].spawner->GetGameObject()->SetActive(true);
	}
}

void EnemySpawnOrder::SetOrder(Que<OrderParam> _orderQue, unsigned int _slot)
{
	if (spawnerList.Length() <= _slot)
		return;

	spawnerList[_slot].orderQue = _orderQue;

	OrderParam param = spawnerList[_slot].orderQue.front();
	spawnerList[_slot].spawner->SetSpawnInterval(param.interval);
	spawnerList[_slot].spawner->SetSpawnEnemyType(param.actType);
	spawnerList[_slot].spawner->GetGameObject()->SetActive(true);
}

void EnemySpawnOrder::DestroySpawner(EnemySpawner* _spawner)
{
	auto itr = std::find_if(spawnerList.begin(), spawnerList.end(), [&](const SpawnerNode& sn) {return sn.spawner == _spawner;});

	if (itr == spawnerList.end())
		return;

	spawnerList.Remove(itr);
}

void EnemySpawnOrder::DestroySpawner(unsigned int _slot)
{
	if (spawnerList.Length() <= _slot)
		return;

	spawnerList.Remove(_slot);
}

EnemySpawner* EnemySpawnOrder::GetSpawner(unsigned int _slot) const noexcept
{
	if (spawnerList.Length() <= _slot)
		return nullptr;

	return spawnerList.At(_slot).spawner;
}

void EnemySpawnOrder::Awake()
{
	auto spawner = Eagle::GameObject::Create("EnemySpawner0")->AttachComponent<EnemySpawner>(1.f, EnemyAct::Rain);
	SpawnerNode node;
	node.spawner = spawner;
	spawnerList.Add(node);
}

void EnemySpawnOrder::LateUpdate()
{
	const float deltaTime = Eagle::Time::DeltaTimeF();

	for (auto& node : spawnerList)
	{
		if (node.orderQue.empty())
			continue;

		if (!node.spawner->IsEnable())
			continue;

		node.orderQue.front().activeTime -= deltaTime;

		if (node.orderQue.front().activeTime <= 0.f)
		{
			ChangeOrder(node);
		}
	}
}

void EnemySpawnOrder::OnDestroy()
{
	for (auto& node : spawnerList)
	{
		Eagle::GameObject::Destroy(node.spawner->GetGameObject());
	}
}

void EnemySpawnOrder::ChangeOrder(SpawnerNode& node)
{
	node.orderQue.pop();

	if (node.orderQue.empty())
	{
		node.spawner->GetGameObject()->SetActive(false);
		return;
	}

	auto& order = node.orderQue.front();

	node.spawner->SetSpawnInterval(order.interval);
	node.spawner->SetSpawnEnemyType(order.actType);
}
