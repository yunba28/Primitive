#pragma once
#include "eagle.hpp"

class CircleDrawer;

class RoseColliderView : public Eagle::Behavior {
public:
	RoseColliderView(CircleDrawer* _body, CircleDrawer* _mesh);

private:
	CircleDrawer* body;
	CircleDrawer* mesh;

	void Update()override;
};