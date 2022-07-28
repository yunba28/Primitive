#pragma once
#include "eagle.hpp"

class PlayerMove;
class PlayerShot;
class CircleDrawer;
class BoxDrawer;
class PlayerArmorGauge;
class ControlSlowTime;
class Gauge;
class Interval;

//[summary]
//�v���C���[�Ɋւ���R���|�[�l���g�̊Ǘ����s��
//���Pause�i�ꎞ��~�j��Resume�i�ĊJ�j
//�����ɂ�Player::Create�֐����g�p���Ă�������
class Player : public Eagle::Behavior, public Eagle::PauseAndResume {
private:
	using CircleCollider2D = Eagle::CircleCollider2D;
public:
	Player();
	~Player();

	void Pause()override;
	void Resume()override;

	void OnDamage(float damage = 10.f);

	//1.f�Ŕ�e��100%�J�b�g
	void SetDamageCutRate(float _cutRate);

	CircleDrawer* GetCircleDrawer()const noexcept;

	CircleCollider2D* GetCircleCollider()const noexcept;

	float GetArmor()const noexcept;

	static Player* Create(Eagle::String name, float speed, float radius, Eagle::Color color, float shotInterval = 0.1f);

private:
	CircleDrawer* circleDrawer;
	CircleCollider2D* circleCollider;
	ControlSlowTime* controlSlowTime;
	BoxDrawer* damageFrameBox;
	UNIQUE(Interval) damageInterval;
	float armor;
	float damageCutRate;

	void Start()override;
	void Update()override;

	void SetSlowTimeState(float _timeSpeed, float _radiusFactor);
};