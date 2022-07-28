#pragma once

class Selectable {
public:
	virtual void OnFocus() = 0;
	virtual void OffFocus() = 0;

	bool IsFocus()const noexcept { return isFocus; }

private:
	bool isFocus = false;
};