#pragma once
#include "cocos2d.h"
#include "../Bullet.h"
#include <string>
#include "SimpleAudioEngine.h"
#define AUDIO CocosDenshion::SimpleAudioEngine::getInstance()

class Weapon : public cocos2d::Sprite {
protected:
	static std::string file;
	static int magazine;
	static float fireInterval;
	static float reloadTime;
	static int damage;
	static float speed;
	bool inFireInterval = false;
	unsigned int reloading = 0;
	std::string id;
	Weapon();
	void setFireInterVal();
public:
	int current;
	const std::string& getId();
	virtual std::string getFile() = 0;
	virtual int getMagazine() = 0;
	virtual int getDamage() = 0;
	virtual float getReloadTime() = 0;
	virtual float getFireInterval() = 0;
	void reload();
	void reset();
	virtual bool fire(bool force = false) = 0;
	void broadCastToken();
	void broadCastDropped();
	void broadCastFire();
	static void init();
};
