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
	bool reloading = false, inFireInterval = false;
	std::string id;
	Weapon();
	void setFireInterVal();
public:
	int getCurrent();
	const std::string& getId();
	virtual int getMagazine() = 0;
	virtual int getDamage() = 0;
	virtual float getReloadTime() = 0;
	virtual float getFireInterval() = 0;
	void reload();
	virtual void fire() = 0;
	void broadCastToken();
	void broadCastDropped();
};
