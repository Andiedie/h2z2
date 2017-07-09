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
		case 1:
			w = new Uzi(id);
			break;
		case 2:
			w = new Shotgun(id);
			break;
		case 3:
			w = new Laser(id);
			break;
		case 4:
			w = new Rocket(id);
			break;
		default:
			w = nullptr;
			break;
	}
	if (w != nullptr) {
		w->setScale(0.3f);
		w->setPosition(pos);
		auto body = PhysicsBody::createBox(w->getContentSize()*w->getScale(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
		body->setCategoryBitmask(0x00000003);
		body->setCollisionBitmask(0x00000001);
		body->setContactTestBitmask(0x00000001);
		w->setPhysicsBody(body);
		pool.insert(make_pair(id, w));
	}
	return w;
}

Weapon* Weapons::getById(std::string id) {
	auto it = pool.find(id);
	if (it != pool.end()) return it->second;
	return nullptr;
}
