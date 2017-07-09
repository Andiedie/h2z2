#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#define AUDIO CocosDenshion::SimpleAudioEngine::getInstance()

class Boom : public cocos2d::ParticleSystemQuad {
	Boom();
public:
	static Boom* create(cocos2d::Vec2 pos);
};
