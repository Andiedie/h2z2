#pragma once

#include "Boom.h"

USING_NS_CC;

Boom::Boom() {}

Boom* Boom::create(Vec2 pos) {
	auto boom = new (std::nothrow) Boom();
	if (boom && boom->initWithFile("explode.plist")) {
		// init
		boom->setDuration(0.25f);
		boom->_emissionRate = 4.0f;
		boom->setPosition(pos);
		boom->setScale(5.0f);
		boom->autorelease();
		return boom;
	}
	CC_SAFE_DELETE(boom);
	return nullptr;
}
