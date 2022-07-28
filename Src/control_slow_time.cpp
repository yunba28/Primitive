#include "control_slow_time.hpp"
#include "drawer.hpp"
#include "gauge.hpp"

#include "eagle/ease.hpp"

Input::Key ControlSlowTime::actionKey = Input::Key::LeftShift;

ControlSlowTime::ControlSlowTime()
	: Behavior(ID::Update)
	, gauge(nullptr)
	, stateFlag(0)
	, canSlowTime(true)
{
}

void ControlSlowTime::SetSlowKey(Input::Key key)
{
	ControlSlowTime::actionKey = key;
}

Gauge* ControlSlowTime::GetGauge() const noexcept
{
	return gauge;
}

bool ControlSlowTime::IsStart() const noexcept
{
	return stateFlag == static_cast<int>(State::Down);
}

bool ControlSlowTime::IsEnd() const noexcept
{
	return stateFlag == static_cast<int>(State::Up);
}

void ControlSlowTime::Awake()
{
	gauge = GetGameObject()->AttachComponent<Gauge>();
	gauge->SetSize(200.f, 10.f);
	gauge->GetGaugeBox()->SetColor(0x0000ff);
	gauge->SetGaugeAligned(GaugeAligned::Left);
}

bool ControlSlowTime::GetSlowTimePressedAction()
{
	return Input::GetDown(Input::Mouse::Right) || Input::GetDown(ControlSlowTime::actionKey);
}

bool ControlSlowTime::GetSlowTimeLeftAction()
{
	return Input::GetUp(Input::Mouse::Right) || Input::GetUp(ControlSlowTime::actionKey);
}

void ControlSlowTime::Update()
{
	//�ᑬ���[�h���p������ꍇ�X�e�[�g��Stay�ɕύX����
	if (!canSlowTime && stateFlag == static_cast<int>(State::Down))
	{
		stateFlag = static_cast<int>(State::Stay);
	}

	//�ᑬ���[�h
	if (GetSlowTimePressedAction() && canSlowTime)
	{
		stateFlag = static_cast<int>(State::Down);
		canSlowTime = false;
	}

	//�ᑬ���[�h���̏���
	if (stateFlag & static_cast<int>(State::Pressed))
	{
		float newValue = gauge->GetValue() - Eagle::Time::DeltaTimeF();
		gauge->SetValue(newValue);

		if (newValue <= 0.f)
		{
			stateFlag = static_cast<int>(State::Up);
			canSlowTime = false;
		}
	}
	//��ᑬ���[�h���̏���
	else
	{
		stateFlag = static_cast<int>(State::Neutral);

		float newValue = gauge->GetValue() + Eagle::Time::DeltaTimeF();
		gauge->SetValue(newValue);

		if (newValue >= 1.f)
		{
			canSlowTime = true;
		}
	}

	//�ʏ푬�x�ɖ߂�
	if (GetSlowTimeLeftAction() && (stateFlag & static_cast<int>(State::Pressed)))
	{
		stateFlag = static_cast<int>(State::Up);
	}

	Eagle::Color newColor = Eagle::Color::FromHsv(Eagle::Ease::Linear(gauge->GetValue(), 360.f, 240.f), 1.f, 1.f);
	gauge->GetGaugeBox()->SetColor(newColor);
}
