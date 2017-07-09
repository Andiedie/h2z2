#pragma once

#include "utils\global.h"
#include "cocos2d.h"
#include "weapon\Weapons.h"
#include <algorithm>

class Player : public cocos2d::Sprite {
	const static int maxHp;
	int hp = 200;
	Player();
public:
	Weapon* weapon = nullptr;
	virtual rapidjson::Document createSyncData() const;
	virtual void sync(rapidjson::GenericValue<rapidjson::UTF8<>> &data);
	void addVelocity(cocos2d::Vec2 v);
	void setVelocityX(float vx);
	void setVelocityY(float vy);
	void broadcastHit(float dmg) const;
	void broadcastDead() const;
	int getHp() const;
	void setHp(int _hp);
	void heal(int _hp);
	bool damage(int _hp); // return true if alive
	void takeWeapon(Weapon* w);
	Weapon* dropWeapon();

	static Player* create(cocos2d::Vec2 pos = cocos2d::Vec2::ZERO);
};
