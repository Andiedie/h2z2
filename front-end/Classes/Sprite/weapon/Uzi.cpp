#include "Uzi.h"
USING_NS_CC;
using namespace std;

string Uzi::file = "uzi";
int Uzi::magazine = 40;
float Uzi::fireInterval = 0.05;
float Uzi::reloadTime = 3;
int Uzi::damage = 13;

Uzi::Uzi(std::string id) {
	this->initWithFile("weapon/" + Uzi::file + ".png");
	this->current = Uzi::magazine;
	this->id = id;
}

void Uzi::fire() {
	if (reloading || inFireInterval || current <= 0) return;
	setFireInterVal();
	auto player = getParent();
	auto scene = player->getParent();
	auto angle = player->getRotation();
	auto pos = player->getPosition();
	auto normalizedDirection = Vec2(sinf(CC_DEGREES_TO_RADIANS(angle)), cosf(CC_DEGREES_TO_RADIANS(angle)));
	pos += 45.0f * normalizedDirection;
	auto bullet = new Bullet(file, pos, damage, player->getRotation(), 500.0f);
	scene->addChild(bullet);
	bullet->broadcast();
	this->current = max(0, current - 1);
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
