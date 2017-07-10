#pragma once

#include "Boom.h"

USING_NS_CC;

Boom::Boom() {}

Boom* Boom::create(Vec2 pos) {
	auto boom = new (std::nothrow) Boom();
	if (boom && boom->initWithFile("explode.plist")) {
		auto body = PhysicsBody::createBox(boom->getContentSize(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
		body->setCategoryBitmask(0);
		body->setCollisionBitmask(0);
		body->setContactTestBitmask(0);
		// boom->setPhysicsBody(body);

		boom->setPosition(pos);
		boom->autorelease();
		AUDIO->playEffect("sound/boom.wav");
		return boom;
	}
	CC_SAFE_DELETE(boom);
	return nullptr;
}
