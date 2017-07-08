#pragma once
#include "../Player.h"
#include "Pistol.h"

class Weapons {
private:
	Weapons();
	static std::map<std::string, Weapon*> pool;
	static cocos2d::Node* node;
public:
	static Weapon* create(int type, std::string id, cocos2d::Vec2 pos);
	static void take(Player* player, const std::string& id, bool broadcast = false);
	static void drop(Player* player, bool broadcast = false);
};
