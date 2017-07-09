#pragma once
#include "Bullet.h"
USING_NS_CC;
using namespace std;

cocos2d::Vec2 Bullet::gameArea;

void Bullet::initAutoRemove(Vec2 gameArea) {
	Bullet::gameArea = gameArea;
}

void Bullet::autoRemove(float) {
	const static float limit = 100.0f;
	auto pos = this->getPosition();
	if (pos.x >= -limit && pos.y <= gameArea.x + limit) return;
	if (pos.y >= -limit && pos.y <= gameArea.y + limit) return;
	this->removeFromParentAndCleanup(true);
}

int Bullet::getDamage() {
	return this->damage;
}

Bullet::Bullet(std::string file, cocos2d::Vec2 pos, int damage, float angle, float speed) {
	this->damage = damage;
	this->file = file;
	this->initWithFile("bullet/" + file + ".png");
	this->setScale(0.2f);
	this->setPosition(pos);
	this->setRotation(angle);
	auto normalizedDirection = Vec2(sinf(CC_DEGREES_TO_RADIANS(angle)), cosf(CC_DEGREES_TO_RADIANS(angle)));
	auto body = PhysicsBody::createBox(this->getContentSize() * this->getScale(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
	body->setCategoryBitmask(0x00000002);
	body->setCollisionBitmask(0x00000001); // only collides with player
	body->setContactTestBitmask(0x00000001);
	body->setVelocity(speed * normalizedDirection);
	this->setPhysicsBody(body);
	this->autorelease();
	this->schedule(schedule_selector(Bullet::autoRemove), 1.0f, kRepeatForever, 3.0f);
}

void Bullet::broadcast() const {
	Document dom;
	dom.SetObject();
	dom.AddMember("type", "bullet", dom.GetAllocator());
	dom.AddMember("file", StringRef(file.c_str()), dom.GetAllocator());
	dom.AddMember("posX", this->getPosition().x, dom.GetAllocator());
	dom.AddMember("posY", this->getPosition().y, dom.GetAllocator());
	dom.AddMember("angle", this->getRotation(), dom.GetAllocator());
	dom.AddMember("damage", this->damage, dom.GetAllocator());
	dom.AddMember("speed", this->getPhysicsBody()->getVelocity().getLength(), dom.GetAllocator());
	GSocket->sendEvent("broadcast", dom);
}
