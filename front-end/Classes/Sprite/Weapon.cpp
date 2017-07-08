#pragma once
#include "Weapon.h"
USING_NS_CC;

Weapon::Weapon() {}

Weapon* Weapon::create(Vec2 pos) {
	auto weapon = new (std::nothrow) Weapon();
	if (weapon && weapon->initWithFile("wp1.png")) {
		weapon->setPosition(pos);
		weapon->setScale(0.2f);
		auto body = PhysicsBody::createBox(weapon->getContentSize()*weapon->getScale(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
		body->setCategoryBitmask(0x00000003);
		body->setCollisionBitmask(0x00000001);
		body->setContactTestBitmask(0x00000001);
		weapon->setPhysicsBody(body);
		return weapon;
	}
	CC_SAFE_DELETE(weapon);
	return nullptr;
}

void Weapon::token() {
	this->removeComponent(this->getPhysicsBody());
	this->setScale(1.0f);
}

void Weapon::dropped() {
	this->setScale(0.2f);
	auto body = PhysicsBody::createBox(this->getContentSize()*this->getScale(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
	body->setCategoryBitmask(0x00000003);
	body->setCollisionBitmask(0x00000001);
	body->setContactTestBitmask(0x00000001);
	this->setPhysicsBody(body);
}

