#pragma once
#include "cocos2d.h"
#include "utils\global.h"

class Bullet : public cocos2d::Sprite {
private:
	std::string file;
	static cocos2d::Vec2 gameArea;
	void autoRemove(float);
	int damage;
public:
	int getDamage();
	Bullet(std::string file, cocos2d::Vec2 pos, int damage, float angle, float speed);
	static void initAutoRemove(cocos2d::Vec2 gameArea);
	void broadcast() const;
};
