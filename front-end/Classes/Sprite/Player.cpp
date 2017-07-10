#pragma once

#include "Player.h"
#include <chrono>

USING_NS_CC;
using namespace std::chrono;

const int Player::maxHp = 200;

Player::Player() {}

Player* Player::create(Color3B color, Vec2 pos) {
	auto player = new (std::nothrow) Player();
	if (player && player->initWithFile("player.png")) {
		// init
		player->setScale(0.3f);
		player->setPosition(pos);
		player->setColor(color);
		auto playerBody = PhysicsBody::createBox(player->getContentSize(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
		playerBody->setCategoryBitmask(0x00000001);
		playerBody->setCollisionBitmask(0xFFFFFFFE); // disable collision between players
		playerBody->setContactTestBitmask(0xFFFFFFFE);
		playerBody->setRotationEnable(false);
		player->setPhysicsBody(playerBody);

		player->autorelease();
		return player;
	}
	CC_SAFE_DELETE(player);
	return nullptr;
}

Document Player::createSyncData() const {
	Document dom;
	auto body = this->getPhysicsBody();
	dom.SetObject();

	// position & speed & angle
	rapidjson::Value speedX, speedY, posX, posY;
	auto speed = body->getVelocity();

	// note: must use position of 'player' instead of 'body', to have a correct position
	//	 while working with the Follow Action
	auto pos = this->getPosition();

	dom.AddMember("speedX", speed.x, dom.GetAllocator());
	dom.AddMember("speedY", speed.y, dom.GetAllocator());
	dom.AddMember("posX", pos.x, dom.GetAllocator());
	dom.AddMember("posY", pos.y, dom.GetAllocator());
	dom.AddMember("angle", this->getRotation(), dom.GetAllocator());

	milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	dom.AddMember("timestamp", ms.count(), dom.GetAllocator());	// used to check ping
	return dom;
}

void Player::sync(GenericValue<UTF8<>> &data) {
	auto body = this->getPhysicsBody();
	body->setVelocity(Vec2(data["speedX"].GetDouble(), data["speedY"].GetDouble()));
	this->setPosition(data["posX"].GetDouble(), data["posY"].GetDouble());
	this->setRotation(data["angle"].GetDouble());
}

void Player::addVelocity(Vec2 v) {
	auto body = this->getPhysicsBody();
	if (body) {
		body->setVelocity(body->getVelocity() + v);
	}
}

void Player::setVelocityX(float vx) {
	auto body = this->getPhysicsBody();
	if (body) {
		body->setVelocity(Vec2(vx, body->getVelocity().y));
	}
}

void Player::setVelocityY(float vy) {
	auto body = this->getPhysicsBody();
	if (body) {
		body->setVelocity(Vec2(body->getVelocity().x, vy));
	}
}

void Player::broadcastHit(float dmg) const {
	Document dom;
	dom.SetObject();
	dom.AddMember("type", "hit", dom.GetAllocator());
	dom.AddMember("damage", 20.0f, dom.GetAllocator());	// TODO: damage determined by bullet type
	GSocket->sendEvent("broadcast", dom);
}

int Player::getHp() const { return hp; }

void Player::setHp(int _hp) {
	hp = std::min(_hp, maxHp);
}

void Player::heal(int _hp) {
	hp = std::min(hp + _hp, maxHp);
}

bool Player::damage(int _hp) {
	hp = std::max(0, hp - _hp);
	return hp > 0;
}

void Player::broadcastDead() const {
	Document dom;
	dom.SetObject();
	dom.AddMember("type", "dead", dom.GetAllocator());
	GSocket->sendEvent("broadcast", dom);
}

void Player::takeWeapon(Weapon* w) {
	if (weapon != nullptr) return;
	w->reset();
	w->setScale(1.0f);
	w->setRotation(0);
	w->removeComponent(w->getPhysicsBody());
	auto size = getContentSize();
	w->setPosition(Vec2(size.width / 2, size.height + 39));
	w->removeFromParent();
	addChild(w);
	weapon = w;
}

Weapon* Player::dropWeapon() {
	if (weapon == nullptr) return nullptr;
	auto w = weapon;
	w->reset();
	w->setScale(0.3f);
	auto body = PhysicsBody::createBox(w->getContentSize() * w->getScale(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
	body->setCategoryBitmask(0x00000003);
	body->setCollisionBitmask(0x00000001);
	body->setContactTestBitmask(0x00000001);
	w->setPhysicsBody(body);
	auto pos = getPosition();
	auto angle = getRotation();
	w->setRotation(angle);
	auto normalizedDirection = Vec2(sinf(CC_DEGREES_TO_RADIANS(angle)), cosf(CC_DEGREES_TO_RADIANS(angle)));
	pos += 100.0f * normalizedDirection;
	w->setPosition(pos);
	w->removeFromParent();
	weapon = nullptr;
	return w;
}
