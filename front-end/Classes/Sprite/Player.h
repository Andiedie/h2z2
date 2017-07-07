#pragma once

#include "utils\global.h"
#include "cocos2d.h"

class Player : public cocos2d::Sprite {
	Player();
public:
	virtual rapidjson::Document createSyncData() const;
	virtual void sync(rapidjson::GenericValue<rapidjson::UTF8<>> &data);
	void addVelocity(cocos2d::Vec2 v);
	void setVelocityX(float vx);
	void setVelocityY(float vy);

	static Player* create(cocos2d::Vec2 pos = cocos2d::Vec2::ZERO);
};
