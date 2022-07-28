#include "player_shot.hpp"
#include "player_bullet.hpp"
#include "player_move.hpp"
#include "bullet_commander.hpp"
#include "interval.hpp"

#include "sound_manager.hpp"

Input::Key PlayerShot::shotKey = Input::Key::Return;

PlayerShot::PlayerShot()
	: Behavior(ID::Update)
	, bulletCommander(nullptr)
	, interval(new Interval)
	, bulletSpeed(450.f)
	, bulletColor(0x0000ff)
{
	interval->SetInterval(0.1f);
}

PlayerShot::PlayerShot(float _interval, float _bulletSpeed, Eagle::Color _color)
	: Behavior(ID::Update)
	, bulletCommander(nullptr)
	, interval(new Interval)
	, bulletSpeed(_bulletSpeed)
	, bulletColor(_color)
{
	interval->SetInterval(_interval);
}

void PlayerShot::Pause()
{
	SetEnable(false);
}

void PlayerShot::Resume()
{
	SetEnable(true);
}

void PlayerShot::SetShotKey(Input::Key _key)
{
	PlayerShot::shotKey = _key;
}

void PlayerShot::Start()
{
	bulletCommander = Eagle::GameObject::FindWithTag("BulletCommander")
		->GetComponent<BulletCommander>();
}

void PlayerShot::Update()
{
	interval->Update();

	//�ᑬ���͍U�����Ȃ�
	if (Eagle::Time::GetTimeFactor() < 1.0)
		return;

	//�V���b�g�A�N�V�������s���Ȃ���΃J�E���^�����Z�b�g����
	//�ȍ~�̏����𒆒f
	if (!PlayerShot::GetShotAction())return;

	//�C���^�[�o���������\���ǂ���
	if (!interval->IsReady())
		return;

	interval->Ready();

	int r = SoundManager::Play("se_beam06.mp3", DX_PLAYTYPE_BACK, 0.75f, true);
	
	//�e����b�Ɉړ����鑬�x
	Eagle::Vector3 shotVelocity(0, -bulletSpeed, 0);
	//�e�𐶐�
	auto shot = PlayerBullet::Create(bulletCommander, shotVelocity, 5.f, bulletColor);

	//�I�u�W�F�N�g�̍��W���J�n�n�_�ɐݒ�
	shot->GetTransform()->SetPosition(GetTransform()->GetWorldPosition());
}

bool PlayerShot::GetShotAction() noexcept
{
	int input = 0;

	if (Input::GetStay(PlayerShot::shotKey))
		input |= 0b001;

	if (Input::GetStay(Input::Mouse::Left))
		input |= 0b010;

	if (Input::GetStay(Input::Pad::A))
		input |= 0b100;

	return input & 0b111;
}
