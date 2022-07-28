#include "rose_collider_view.hpp"
#include "drawer.hpp"

RoseColliderView::RoseColliderView(CircleDrawer* _body, CircleDrawer* _mesh)
	: Behavior(ID::Update)
	, body(_body)
	, mesh(_mesh)
{
}

void RoseColliderView::Update()
{
	bool flag = (Eagle::Time::GetTimeFactorF() < 1.f);
	body->SetEnable(flag);
	if (flag)
	{
		mesh->SetTransparency(0.2f);
	}
	else
	{
		mesh->SetTransparency(1.0f);
	}
}
