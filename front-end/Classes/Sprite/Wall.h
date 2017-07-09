#pragma once

#include "cocos2d.h"
#include "utils\global.h"

class Wall : public cocos2d::Sprite {
	Wall();
public:
	static Wall* create(GenericValue<UTF8<>> &data);
};