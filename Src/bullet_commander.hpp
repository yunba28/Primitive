#pragma once
#include "eagle.hpp"
class BulletBase;

//[summary]
//画面上の弾にPauseとResumeの指示を出すだけのクラス
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