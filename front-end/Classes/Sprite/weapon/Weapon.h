#pragma once
#include "cocos2d.h"
#include <string>

class Weapon : public cocos2d::Sprite {
protected:
	Weapon();
	static std::string file;
	static int magazine;
	static float fireInterval;
	static float reloadTime;
	int current;
	bool enable;
	std::string id;
public:
	const std::string& getId();
	void reload();
	void broadCastToken();
	void broadCastDropped();
	virtual void fire() = 0;
};
