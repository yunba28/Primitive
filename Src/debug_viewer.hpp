#pragma once
#include "eagle.hpp"

class Player;
class Text;

class DebugViewer : public Eagle::Behavior {
public:
	DebugViewer(Player* _player);
private:
	Player* player;
	Text* objectView;
	Text* playerView;

	void Awake()override;
	void Update()override;
};