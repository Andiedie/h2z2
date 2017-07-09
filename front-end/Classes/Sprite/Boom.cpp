#pragma once

#include "Boom.h"

USING_NS_CC;

Boom::Boom() {}

Boom* Boom::create(Vec2 pos) {
	auto boom = new (std::nothrow) Boom();
	if (boom && boom->initWithFile("explode.plist")) {
		boom->setPosition(pos);
		boom->setScale(.6f);
		boom->autorelease();
		AUDIO->playEffect("sound/boom.wav");
		return boom;
	}
	CC_SAFE_DELETE(boom);
	return nullptr;
}
