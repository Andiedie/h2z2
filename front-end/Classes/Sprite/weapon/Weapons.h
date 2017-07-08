#pragma once
#include "../Player.h"
#include "Pistol.h"
#include "utils\global.h"
#include <string>
#include <map>

class Weapons {
private:
	Weapons();
	static std::map<std::string, Weapon*> pool;
public:
	static Weapon* create(int type, std::string id, cocos2d::Vec2 pos);
	static void take(Player* player, const std::string& id);
	static Weapon* drop(Player* player);
};
