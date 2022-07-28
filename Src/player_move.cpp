#include "player_move.hpp"
#include "player_bullet.hpp"

//=================================  ���[�J���Ȋ֐��̐錾  ======================================//

//�ړ��̓��͂����Ŏ󂯎��
//�L�[
Eagle::Vector3 GetMoveAxis(Input::Key up, Input::Key down, Input::Key left, Input::Key right);

//�ړ��̓��͂����Ŏ󂯎��
//�p�b�h
Eagle::Vector3 GetMoveAxis();

//=============================================================================================//

Input::Key PlayerMove::moveKey[] = { Input::Key::W,Input::Key::S ,Input::Key::A ,Input::Key::D };

PlayerMove::PlayerMove()
	: Behavior(ID::Update)
	, speed(300.f)
{

}

PlayerMove::PlayerMove(const float _speed)
	: Behavior(ID::Update)
	, speed(_speed)
{
}

PlayerMove::~PlayerMove()
{
}

void PlayerMove::Pause()
{
	SetEnable(false);
}

void PlayerMove::Resume()
{
	SetEnable(true);
}

void PlayerMove::SetSpeed(float _speed)
{
	speed = _speed;
}

float PlayerMove::GetSpeed() const
{
	return speed;
}

void PlayerMove::SetMoveUpKey(Input::Key _key)
{
	PlayerMove::moveKey[0] = _key;
}

void PlayerMove::SetMoveDownKey(Input::Key _key)
{
	PlayerMove::moveKey[1] = _key;
}

void PlayerMove::SetMoveLeftKey(Input::Key _key)
{
	PlayerMove::moveKey[2] = _key;
}

void PlayerMove::SetMoveRightKey(Input::Key _key)
{
	PlayerMove::moveKey[3] = _key;
}

void PlayerMove::SetMoveKey(Input::Key _up, Input::Key _down, Input::Key _left, Input::Key _right)
{
	PlayerMove::moveKey[0] = _up;
	PlayerMove::moveKey[1] = _down;
	PlayerMove::moveKey[2] = _left;
	PlayerMove::moveKey[3] = _right;
}

void PlayerMove::Update()
{
	//�ړ������̎擾
	Eagle::Vector3 moveAxis = GetMoveAxis(moveKey[0], moveKey[1], moveKey[2], moveKey[3]);

	moveAxis = [&] {
		Eagle::Vector3 axis = GetMoveAxis();
		if (!axis)
			return moveAxis;
		return axis;
	}();

	//�ړ��̓��͂��Ȃ�
	if (!moveAxis)
		return;

	//�ړ���̍��W
	Eagle::Vector3 newPos = GetTransform()->GetWorldPosition(false);

	//�ړ������Ɉړ��ʂ��|����
	newPos += moveAxis * (speed * Eagle::Time::DeltaTimeF());

	Eagle::Math::Clamp(newPos.x, FIELD_MIN_X, FIELD_MAX_X); //�t�B�[���h�O�ړ��̐���
	Eagle::Math::Clamp(newPos.y, FIELD_MIN_Y, FIELD_MAX_Y); //�t�B�[���h�O�ړ��̐���

	//�ړ�
	GetTransform()->SetPosition(newPos);
}

Eagle::Vector3 GetMoveAxis(Input::Key up, Input::Key down, Input::Key left, Input::Key right)
{
	Eagle::Vector3 axis = Eagle::Vector3::zero;

	if (Input::GetStay(up))
		axis.y -= 1.f;
	if (Input::GetStay(down))
		axis.y += 1.f;
	if (Input::GetStay(left))
		axis.x -= 1.f;
	if (Input::GetStay(right))
		axis.x += 1.f;

	if (!axis)
		return axis;

	axis.Normalize();

	return axis;
}

Eagle::Vector3 GetMoveAxis()
{
	Eagle::Vector3 axis = Eagle::Vector3::zero;

	auto padAxis = Input::GetPadLeftThumb();

	if (!padAxis)
		return axis;

	axis = Eagle::Vector3(padAxis.x, padAxis.y, 0.f).Normalize();

	return axis;
}