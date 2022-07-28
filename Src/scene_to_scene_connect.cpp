#include "scene_to_scene_connect.hpp"
#include "eagle/scene_management.hpp"

SceneToSceneConnect::SceneToSceneConnect(void(*_func)())
	: Behavior(ID::NoBehave)
	, sequenceFunc(_func)
{
}

void SceneToSceneConnect::Start()
{
	sequenceFunc();

	Eagle::SceneManager::DestroyScene(GetGameObject()->GetScene()->GetName());
}
