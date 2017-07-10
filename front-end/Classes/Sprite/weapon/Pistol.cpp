#include "Pistol.h"
USING_NS_CC;
using namespace std;

string Pistol::file = "pistol";
int Pistol::magazine = 15;
float Pistol::fireInterval = 0.2f;
float Pistol::reloadTime = 1.0f;
int Pistol::damage = 58;
float Pistol::speed = 700.0f;

Pistol::Pistol(std::string id) {
	this->initWithFile("weapon/" + Pistol::file + ".png");
	this->current = Pistol::magazine;
	this->id = id;
}

bool Pistol::fire(bool force) {
	if (!force && (reloading || inFireInterval || current <= 0)) return false;
	setFireInterVal();
	auto player = getParent();
	auto scene = player->getParent();
	auto angle = player->getRotation();
	auto pos = player->getPosition();
	pos += 45.0f * Vec2(sinf(CC_DEGREES_TO_RADIANS(angle)), cosf(CC_DEGREES_TO_RADIANS(angle)));
	scene->addChild(new Bullet(file, pos, damage, player->getRotation(), speed));
	this->current = max(0, current - 1);
	return true;
}

int Pistol::getMagazine() {
	return magazine;
}

int Pistol::getDamage() {
	return damage;
}

float Pistol::getReloadTime() {
	return reloadTime;
}

float Pistol::getFireInterval() {
	return fireInterval;
}

string Pistol::getFile() {
	return file;
}

