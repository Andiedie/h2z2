#pragma once
#include "cocos2d.h"

class Poison {
private:
	Poison();
	static cocos2d::Vec2 leftTop;
	static cocos2d::Vec2 leftBottom;
	static cocos2d::Vec2 rightTop;
	static cocos2d::Vec2 rightBottom;
	static cocos2d::Color4F color;
	static cocos2d::DrawNode* dn;
	static float radius;
	static int extendStep;
	static int damageStep;
public:
	static cocos2d::DrawNode* init(cocos2d::Vec2 area);
	static void drawPoison();
	static float getRadius();
	static int getDamage();
	static bool inside(cocos2d::Vec2 pos);
};