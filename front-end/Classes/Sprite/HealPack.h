#pragma once

#include "cocos2d.h"
#include "utils\global.h"
#include <string>
#include <map>

class HealPack : public cocos2d::Sprite {
	HealPack();

	static std::map<std::string, HealPack*> pool;
	int hp;
	std::string id;
public:
	static HealPack* create(rapidjson::GenericValue<UTF8<>> &data);
	static void remove(const std::string &id);
	void broadcastEaten() const;
	int getHp() const;
	const std::string& getId() const;
};
