#pragma once

#include "Bullet.h"

USING_NS_CC;
using namespace std;

set<Bullet*> Bullet::pool;
Vec2 Bullet::gameArea;

void Bullet::initAutoRemove(Vec2 gameArea) {
	Bullet::gameArea = gameArea;
}

void Bullet::autoRemove(float) {
	const static float limit = 100.0f;
	auto pos = this->getPosition();
	if (pos.x >= -limit && pos.y <= gameArea.x + limit) return;
	if (pos.y >= -limit && pos.y <= gameArea.y + limit) return;
	this->removeFromParentAndCleanup(true);
	CCLOG("bullet removed");
}

Bullet::Bullet() {}

Bullet* Bullet::create(Vec2 pos, float angle, float velocity, float offset) {
	auto bullet = new (std::nothrow) Bullet();
	if (bullet && bullet->initWithFile("bullet.png")) {
		// init
		auto normalizedDirection = Vec2(sinf(CC_DEGREES_TO_RADIANS(angle)), cosf(CC_DEGREES_TO_RADIANS(angle)));
		bullet->setRotation(angle);
		pos += offset * normalizedDirection;
		bullet->setPosition(pos);
		bullet->setScale(0.2f);

		// init physicsBody
		auto body = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
		body->setCategoryBitmask(0x00000002);
		body->setCollisionBitmask(0x00000001); // only collides with player
		body->setContactTestBitmask(0x00000001);
		body->setVelocity(velocity * normalizedDirection);
		bullet->setPhysicsBody(body);

		bullet->autorelease();
		bullet->schedule(schedule_selector(Bullet::autoRemove), 1.0f, kRepeatForever, 1.0f);
		return bullet;
	}
	CC_SAFE_DELETE(bullet);
	return nullptr;
}

void Bullet::broadcast() const {
	Document dom;
	dom.SetObject();
	dom.AddMember("type", "shoot", dom.GetAllocator()); // indicate broadcasting type
	dom.AddMember("posX", this->getPosition().x, dom.GetAllocator());
	dom.AddMember("posY", this->getPosition().y, dom.GetAllocator());
	dom.AddMember("angle", this->getRotation(), dom.GetAllocator());
	dom.AddMember("velocity", this->getPhysicsBody()->getVelocity().getLength(), dom.GetAllocator());

	GSocket->sendEvent("broadcast", dom);
}
