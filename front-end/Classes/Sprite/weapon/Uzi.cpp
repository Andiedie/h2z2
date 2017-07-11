#include "Uzi.h"
USING_NS_CC;
using namespace std;

string Uzi::file = "uzi";
int Uzi::magazine = 30;
float Uzi::fireInterval = 0.01f;
float Uzi::reloadTime = 3;
int Uzi::damage = 8;
float Uzi::speed = 400.0f;

Uzi::Uzi(std::string id) {
	this->initWithFile("weapon/" + Uzi::file + ".png");
	this->current = Uzi::magazine;
	this->id = id;
}

bool Uzi::fire(bool force) {
	if (!force && (reloading || inFireInterval || current <= 0)) return false;
	setFireInterVal();
	auto player = getParent();
	auto scene = player->getParent();
	auto angle = player->getRotation();
	auto pos = player->getPosition();
	angle += random(-15, 15);
	pos += 45.0f * Vec2(sinf(CC_DEGREES_TO_RADIANS(angle)), cosf(CC_DEGREES_TO_RADIANS(angle)));
	scene->addChild(new Bullet(file, pos, damage, player->getRotation(), speed));
	angle += random(-15, 15);
	pos += 45.0f * Vec2(sinf(CC_DEGREES_TO_RADIANS(angle)), cosf(CC_DEGREES_TO_RADIANS(angle)));
	scene->addChild(new Bullet(file, pos, damage, player->getRotation(), speed));
	angle += random(-15, 15);
	pos += 45.0f * Vec2(sinf(CC_DEGREES_TO_RADIANS(angle)), cosf(CC_DEGREES_TO_RADIANS(angle)));
	scene->addChild(new Bullet(file, pos, damage, player->getRotation(), speed));
	this->current = max(0, current - 1);
	return true;
}

int Uzi::getMagazine() {
	return magazine;
}

int Uzi::getDamage() {
	return damage;
}

float Uzi::getReloadTime() {
	return reloadTime;
}

float Uzi::getFireInterval() {
	return fireInterval;
}

string Uzi::getFile() {
	return file;
}
