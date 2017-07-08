#pragma once

#include "HealPack.h"

USING_NS_CC;
using namespace std;

map<string, HealPack*> HealPack::pool;

HealPack::HealPack() {}

HealPack* HealPack::create(rapidjson::GenericValue<UTF8<>> &data) {
	auto pack = new (std::nothrow) HealPack();
	if (pack && pack->initWithFile("healpack.png")) {
		// init
		pack->setPosition(data["posX"].GetDouble(), data["posY"].GetDouble());
		pack->id = data["id"].GetString();
		pack->hp = data["hp"].GetDouble();
		pack->setScale(0.4f);

		auto body = PhysicsBody::createBox(pack->getContentSize(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
		body->setCategoryBitmask(0x00000004);
		body->setCollisionBitmask(0x00000001); // only collides with player
		body->setContactTestBitmask(0x00000001);
		pack->setPhysicsBody(body);
		
		pool.insert(make_pair(pack->id, pack));
		pack->autorelease();
		return pack;
	}
	CC_SAFE_DELETE(pack);
	return nullptr;
}

void HealPack::broadcastEaten() const {
	Document dom;
	dom.SetObject();
	dom.AddMember("type", "eatPack", dom.GetAllocator());
	dom.AddMember("packId", StringRef(id.c_str()), dom.GetAllocator());

	GSocket->sendEvent("broadcast", dom);
}

int HealPack::getHp() const { return hp; }

const string& HealPack::getId() const { return id; }

void HealPack::remove(const string &id) {
	auto it = pool.find(id);
	if (it != pool.end()) {
		auto pack = it->second;
		pool.erase(pack->id);
		pack->removeFromParentAndCleanup(true);
	}
}