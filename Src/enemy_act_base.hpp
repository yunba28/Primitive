#pragma once
#include "eagle.hpp"

class BulletCommander;

//[summary]
//エネミーの行動を決定するコンポーネントの基盤
//移動と攻撃、その他のモーションを管理する
class EnemyActBase : public Eagle::Behavior {
public:
	EnemyActBase();
	virtual ~EnemyActBase() {}

	virtual void OnShootDown(){}

	bool IsMoveEnd()const noexcept;

protected:
	BulletCommander* bulletCommander;

private:
	bool isMoveEnd;

	virtual void Update()override = 0;

protected:
	void OnMoveEnd()noexcept;

};