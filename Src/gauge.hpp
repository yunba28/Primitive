#pragma once
#include "eagle.hpp"

class BoxDrawer;

enum class GaugeAligned {
	Center,
	Left,
	Right
};

class Gauge : public Eagle::Behavior {
private:
	using BarSize = Eagle::Float2;
	using AlignFunc = void (Gauge::*)();
public:
	Gauge();
	~Gauge();

	void SetSize(float _width, float _height);

	void SetGaugeAligned(GaugeAligned _aligned);

	void SetValue(float _value);

	BoxDrawer* GetGaugeBox()const noexcept;

	BoxDrawer* GetBackgroundBox()const noexcept;

	float GetValue()const noexcept;

private:
	struct{
		BoxDrawer* gauge{ nullptr };
		BoxDrawer* background{ nullptr };
	}box;
	BarSize barSize;
	GaugeAligned aligned;
	float value;

	static AlignFunc alignFunc[3];

	void Awake()override;
	void OnDestroy()override;

	void AlignCenter();
	void AlignLeft();
	void AlignRight();
};
