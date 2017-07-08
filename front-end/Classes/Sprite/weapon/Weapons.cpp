#include "Weapons.h"
using namespace std;
USING_NS_CC;

map<string, Weapon*> Weapons::pool;

Weapons::Weapons() {}

Weapon* Weapons::create(int type, string id, Vec2 pos) {
	Weapon* w;
	switch (type) {
		case 0:
			w = new Pistol(id);
			break;
		default:
			w = nullptr;
			break;
	}
	w->setScale(0.2f);
	w->setPosition(pos);
	auto body = PhysicsBody::createBox(w->getContentSize()*w->getScale(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
	body->setCategoryBitmask(0x00000003);
	body->setCollisionBitmask(0x00000001);
	body->setContactTestBitmask(0x00000001);
	w->setPhysicsBody(body);
	pool.insert(make_pair(id, w));
	return w;
}

void Weapons::take(Player* player, const string& id) {
	auto it = pool.find(id);
	if (it != pool.end()) {
		auto w = it->second;
		Document dom;
		dom.SetObject();
		dom.AddMember("type", "takeWeapon", dom.GetAllocator());
		dom.AddMember("weaponId", StringRef(id.c_str()), dom.GetAllocator());
		GSocket->sendEvent("broadcast", dom);
		w->removeComponent(w->getPhysicsBody());
		w->setScale(1.0f);
		player->addChild(w);
		auto size = player->getContentSize();
		w->setRotation(0);
		w->setPosition(Vec2(size.width / 2, size.height + 39));
		player->weaponId = id;
	}
}

Weapon* Weapons::drop(Player* player) {
	auto id = player->weaponId;
	auto it = pool.find(id);
	if (it != pool.end()) {
		auto w = it->second;
		Document dom;
		dom.SetObject();
		dom.AddMember("type", "dropWeapon", dom.GetAllocator());
		dom.AddMember("weaponId", StringRef(id.c_str()), dom.GetAllocator());
		GSocket->sendEvent("broadcast", dom);
		w->setScale(0.2f);
		auto body = PhysicsBody::createBox(w->getContentSize()*w->getScale(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
		body->setCategoryBitmask(0x00000003);
		body->setCollisionBitmask(0x00000001);
		body->setContactTestBitmask(0x00000001);
		w->setPhysicsBody(body);
		auto pos = player->getPosition();
		auto angle = player->getRotation();
		w->setRotation(angle);
		if (angle > 270) angle = angle - 360.0;
		angle = angle / 180.0 * M_PI;
		float x = 100.0 * sin(angle);
		float y = 100.0 * cos(angle);
		w->setPosition(Vec2(pos.x + x, pos.y + y));
		player->removeChild(w, false);
		player->weaponId = "";
		return w;
	}
	return nullptr;
}
