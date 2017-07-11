#pragma once

#include "cocos2d.h"

USING_NS_CC;

class WaitingHall : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
	Size visibleSize;
	Vec2 origin;
	Label* error;

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(WaitingHall);
};
