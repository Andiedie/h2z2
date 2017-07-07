#pragma once

#include "Player.h"
#include <chrono>

USING_NS_CC;
using namespace std::chrono;

Player::Player() {}

Player* Player::create(Vec2 pos) {
	auto player = new (std::nothrow) Player();
	if (player && player->initWithFile("player.png")) {
		// init
		player->setScale(0.2f);
		player->setPosition(pos);
		auto playerBody = PhysicsBody::createBox(player->getContentSize(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
		playerBody->setCategoryBitmask(0x00000001);
		playerBody->setCollisionBitmask(0xFFFFFFFE); // disable collision between players
		playerBody->setContactTestBitmask(0xFFFFFFFE);
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
