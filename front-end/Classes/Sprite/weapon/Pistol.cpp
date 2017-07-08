#include "Pistol.h"
USING_NS_CC;
using namespace std;

string Pistol::file = "pistol";
int Pistol::magazine = 9;
float Pistol::fireInterval = 0.3f;
float Pistol::reloadTime = 1.0f;
int Pistol::damage = 30;

Pistol::Pistol(std::string id) {
	this->initWithFile("weapon/" + Pistol::file + ".png");
	this->current = Pistol::magazine;
	this->id = id;
}

void Pistol::fire() {
	if (reloading || inFireInterval || current <= 0) return;
	setFireInterVal();
	auto player = getParent();
	auto scene = player->getParent();
	auto angle = player->getRotation();
	auto pos = player->getPosition();
	auto normalizedDirection = Vec2(sinf(CC_DEGREES_TO_RADIANS(angle)), cosf(CC_DEGREES_TO_RADIANS(angle)));
	pos += 45.0f * normalizedDirection;
	auto bullet = new Bullet(file, pos, player->getRotation(), 700.0f);
	scene->addChild(bullet);
	bullet->broadcast();
	this->current = max(0, current - 1);
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
