#include "button_widget.hpp"
#include "input.hpp"
#include "button.hpp"

ButtonWidget::ButtonWidget()
	: Behavior()
	, button()
	, current()
	, oldMousePos()
	, direction(WidgetDirection::Vertical)
	, currentIdx(-1)
{
}

ButtonWidget::~ButtonWidget()
{
}

void ButtonWidget::Add(Button* _button)
{
	button.Add(_button);
}

void ButtonWidget::Remove(int idx)
{
	if (idx >= button.Length())
		return;

	auto itr = button.begin();
	for (int i = 0;i < idx;++i) { ++itr; }

	button.Remove(itr);
}

void ButtonWidget::SetListDirection(WidgetDirection _direction)
{
	direction = _direction;
}

int Ring(int& value, int low, int high)
{
	if (value < low)
		return value = high;
	if (value > high)
		return value = low;
	return value;
}

void ButtonWidget::Update()
{
	Eagle::Vector2 mPos = Input::GetMousePosition();
	if (oldMousePos != mPos)
	{
		currentIdx = -1;
		current = nullptr;
	}
	oldMousePos = mPos;
	if (!((direction == WidgetDirection::Vertical) ? VerticalUpdate() : HorizontalUpdate()))
		return;
	Ring(currentIdx, 0, static_cast<int>(button.Length() - 1U));
	auto itr = button.begin();
	for (int i = 0;i < currentIdx;++i) { ++itr; }
	(*itr)->OnFocus();
	if (current)
		current->OffFocus();
	current = (*itr);
}

bool ButtonWidget::VerticalUpdate()
{
	if (Input::GetDown(Input::Key::W) || Input::GetDown(Input::Key::Up))
	{
		currentIdx -= 1;
	}
	else if (Input::GetDown(Input::Key::S) || Input::GetDown(Input::Key::Down))
	{
		currentIdx += 1;
	}
	else
	{
		return false;
	}

	return true;
}

bool ButtonWidget::HorizontalUpdate()
{
	if (Input::GetDown(Input::Key::A) || Input::GetDown(Input::Key::Left))
	{
		currentIdx -= 1;
	}
	else if (Input::GetDown(Input::Key::D) || Input::GetDown(Input::Key::Right))
	{
		currentIdx += 1;
	}
	else
	{
		return false;
	}

	return true;
}
