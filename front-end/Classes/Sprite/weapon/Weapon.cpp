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
float Weapon::speed;

const string& Weapon::getId() {
	return this->id;
}

void Weapon::broadCastToken() {
	AUDIO->playEffect("sound/take.wav");
	Document dom;
	dom.SetObject();
	dom.AddMember("type", "takeWeapon", dom.GetAllocator());
	dom.AddMember("weaponId", StringRef(id.c_str()), dom.GetAllocator());
	GSocket->sendEvent("broadcast", dom);
}

void Weapon::broadCastDropped() {
	AUDIO->playEffect("sound/drop.wav");
	Document dom;
	dom.SetObject();
	dom.AddMember("type", "dropWeapon", dom.GetAllocator());
	dom.AddMember("current", current, dom.GetAllocator());
	GSocket->sendEvent("broadcast", dom);
}

void Weapon::setFireInterVal() {
	if (inFireInterval) return;
	inFireInterval = true;
	runAction(Sequence::create(DelayTime::create(getFireInterval()), CallFunc::create([this]() {
		this->inFireInterval = false;
	}), nullptr));
	AUDIO->playEffect(("sound/" + getFile() + ".wav").data());
}

void Weapon::reload() {
	if (reloading || current == getMagazine()) return;
	reloading = AUDIO->playEffect("sound/reload.wav", true);
	runAction(Sequence::create(DelayTime::create(getReloadTime()), CallFunc::create([this]() {
		AUDIO->stopEffect(this->reloading);
		this->current = this->getMagazine();
		this->inFireInterval = false;
		this->reloading = 0;
	}), nullptr));
}

void Weapon::reset() {
	if (reloading) {
		AUDIO->stopEffect(this->reloading);
		this->reloading = 0;
	}
	this->inFireInterval = false;
}

void Weapon::broadCastFire() {
	Document dom;
	dom.SetObject();
	dom.AddMember("type", "fire", dom.GetAllocator());
	dom.AddMember("weaponId", StringRef(id.c_str()), dom.GetAllocator());
	GSocket->sendEvent("broadcast", dom);
}

void Weapon::init() {
	AUDIO->preloadEffect("sound/laser.wav");
	AUDIO->preloadEffect("sound/pistol.wav");
	AUDIO->preloadEffect("sound/rocket.wav");
	AUDIO->preloadEffect("sound/shotgun.wav");
	AUDIO->preloadEffect("sound/uzi.wav");
	AUDIO->preloadEffect("sound/reload.wav");
	AUDIO->preloadEffect("sound/boom.wav");
	AUDIO->preloadEffect("sound/take.wav");
	AUDIO->preloadEffect("sound/drop.wav");
	AUDIO->preloadEffect("sound/start.wav");
}
