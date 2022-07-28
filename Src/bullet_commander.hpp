#pragma once
#include "eagle.hpp"
class BulletBase;

//[summary]
//��ʏ�̒e��Pause��Resume�̎w�����o�������̃N���X
class BulletCommander : public Eagle::Behavior, public Eagle::PauseAndResume{
public:
	BulletCommander();
	~BulletCommander();

	void Pause()override;

	void Resume()override;

	void OnRemove(BulletBase* bullet);

	void AddBullet(BulletBase* bullet);

	void RemoveAll();

private:
	Eagle::List<BulletBase*> bulletList;
	bool isActive;

	void Awake()override;
};