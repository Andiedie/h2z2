#pragma once

#include "utils\global.h"
#include "cocos2d.h"
#include <algorithm>

class Player : public cocos2d::Sprite {
	const static float maxHp;
	float hp = 200.0f;

	Player();
public:
	std::string weaponId = "";
	virtual rapidjson::Document createSyncData() const;
	virtual void sync(rapidjson::GenericValue<rapidjson::UTF8<>> &data);
	void addVelocity(cocos2d::Vec2 v);
	void setVelocityX(float vx);
	void setVelocityY(float vy);
	void broadcastHit(float dmg) const;
	void broadcastDead() const;
	float getHp() const;
	void setHp(float _hp);
	void heal(float _hp);
	bool damage(float _hp); // return true if alive

	static Player* create(cocos2d::Vec2 pos = cocos2d::Vec2::ZERO);
};
