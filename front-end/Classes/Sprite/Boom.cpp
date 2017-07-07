#pragma once

#include "Boom.h"

USING_NS_CC;

Boom::Boom() {}

Boom* Boom::create(Vec2 pos) {
	auto boom = new (std::nothrow) Boom();
	if (boom && boom->initWithFile("bang.png")) {
		// init
		boom->setPosition(pos);
		boom->setScale(0.3f);
		boom->runAction(
			Sequence::create(
				ScaleBy::create(0.2f, 1.5f),
				ScaleBy::create(0.15f, 0.5f),
				FadeOut::create(0.1f),
				CCCallFunc::create([=]() {
					CCLOG("boom removed.");
					boom->removeFromParentAndCleanup(true);
				}),
				NULL
			)
		);

		boom->autorelease();
		return boom;
	}
	CC_SAFE_DELETE(boom);
	return nullptr;
}
