#pragma once

#include "cocos2d.h"
#include "utils\global.h"
#include <set>

class Bullet : public cocos2d::Sprite {
	Bullet();
	static std::set<Bullet*> pool;
	static cocos2d::Vec2 gameArea;
public:
	static void initAutoRemove(cocos2d::Vec2 gameArea);
	void autoRemove(float);
	static Bullet* create(cocos2d::Vec2 pos, float angle, float velocity = 500.0f, float offset = 35.0f);

	void broadcast() const;
};
