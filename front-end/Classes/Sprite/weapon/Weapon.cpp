#pragma once
#include "Weapon.h"
#include "utils\global.h"
USING_NS_CC;
using namespace std;

Weapon::Weapon() {}

std::string Weapon::file;
int Weapon::magazine;
float Weapon::fireInterval;
float Weapon::reloadTime;
int Weapon::damage;

const string& Weapon::getId() {
	return this->id;
}

void Weapon::broadCastToken() {
	Document dom;
	dom.SetObject();
	dom.AddMember("type", "takeWeapon", dom.GetAllocator());
	dom.AddMember("weaponId", StringRef(id.c_str()), dom.GetAllocator());
	GSocket->sendEvent("broadcast", dom);
}

void Weapon::broadCastDropped() {
	Document dom;
	dom.SetObject();
	dom.AddMember("type", "dropWeapon", dom.GetAllocator());
	GSocket->sendEvent("broadcast", dom);
}

void Weapon::setFireInterVal() {
	if (inFireInterval) return;
	inFireInterval = true;
	runAction(Sequence::create(DelayTime::create(getFireInterval()), CallFunc::create([this]() {
		this->inFireInterval = false;
	}), nullptr));
}

void Weapon::reload() {
	if (reloading) return;
	reloading = true;
	this->current = -1; // indicating reload state
	runAction(Sequence::create(DelayTime::create(getReloadTime()), CallFunc::create([this]() {
		this->current = this->getMagazine();
		this->reloading = false;
	}), nullptr));
}

