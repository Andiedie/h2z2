#pragma once
#include "cocos2d.h"
#include "../Bullet.h"
#include <string>

class Weapon : public cocos2d::Sprite {
protected:
	static std::string file;
	static int magazine;
	static float fireInterval;
	static float reloadTime;
	static int damage;
	int current;
	bool enable;
	std::string id;
	Weapon();
public:
	int getCurrent();
	const std::string& getId();
	virtual int getMagazine() = 0;
	virtual int getDamage() = 0;
	virtual void reload() = 0;
	virtual void fire() = 0;
	void broadCastToken();
	void broadCastDropped();
};
