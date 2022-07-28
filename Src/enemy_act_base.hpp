#pragma once
#include "eagle.hpp"

class BulletCommander;

//[summary]
//�G�l�~�[�̍s�������肷��R���|�[�l���g�̊��
//�ړ��ƍU���A���̑��̃��[�V�������Ǘ�����
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