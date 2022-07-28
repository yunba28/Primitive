#pragma once
#include "eagle.hpp"

class SceneToSceneConnect : public Eagle::Behavior {
public:
	SceneToSceneConnect(void (*_func)());

private:
	void (*sequenceFunc)();

	void Start()override;
};