#pragma once
#include "cocos2d.h"
#include "utils\global.h"

class Bullet : public cocos2d::Sprite {
private:
	std::string file;
	float damage;
	static cocos2d::Vec2 gameArea;
	void autoRemove(float);
public:
	Bullet(std::string file, cocos2d::Vec2 pos, float angle, float speed);
	float getDamage();
	static void initAutoRemove(cocos2d::Vec2 gameArea);
	void broadcast() const;
};
