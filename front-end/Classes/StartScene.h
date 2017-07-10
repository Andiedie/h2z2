#pragma once

#include "cocos2d.h"
#include "utils\global.h"
#include "ui\UITextField.h"

USING_NS_CC;

class StartScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	Size visibleSize;
	Vec2 origin;
	ui::TextField *hostInput = nullptr;
	ui::TextField *portInput = nullptr;
	ui::TextField *nameInput = nullptr;

	virtual bool init();

	// a selector callback
	void connect(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(StartScene);
};
