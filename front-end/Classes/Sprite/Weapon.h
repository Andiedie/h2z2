#pragma once
#include "cocos2d.h"

class Weapon : public cocos2d::Sprite {
	Weapon();
public:
	static Weapon* create(cocos2d::Vec2 pos);
	void token();
	void dropped();
};
