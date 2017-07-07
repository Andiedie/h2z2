#pragma once

#include "cocos2d.h"
#include "utils\global.h"
#include <string>
#include <map>

class HealPack : public cocos2d::Sprite {
	HealPack();

	static std::map<std::string, HealPack*> pool;
	float hp;
	std::string id;
public:
	static HealPack* create(rapidjson::GenericValue<UTF8<>> &data);
	static void remove(const std::string &id);
	void broadcastEaten() const;
	float getHp() const;
	const std::string& getId() const;
};
