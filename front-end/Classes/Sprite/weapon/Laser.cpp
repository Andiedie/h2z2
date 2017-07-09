#include "Laser.h"
USING_NS_CC;
using namespace std;

string Laser::file = "laser";
int Laser::magazine = 40;
float Laser::fireInterval = 0.2f;
float Laser::reloadTime = 1.0f;
int Laser::damage = 30;

Laser::Laser(std::string id) {
	this->initWithFile("weapon/" + Laser::file + ".png");
	this->current = Laser::magazine;
	this->id = id;
}

void Laser::fire() {
	if (reloading || inFireInterval || current <= 0) return;
	setFireInterVal();
	auto player = getParent();
	auto scene = player->getParent();
	auto angle = player->getRotation();
	auto pos = player->getPosition();
	int number = 8;
	auto normalizedDirection = Vec2(sinf(CC_DEGREES_TO_RADIANS(angle)), cosf(CC_DEGREES_TO_RADIANS(angle)));
	pos += 45.0f * normalizedDirection;
	for (int i = 0; i < number; i++) {
		auto bullet = new Bullet(file, pos, damage, player->getRotation(), 500.0f);
		pos += 20.0f * normalizedDirection;
		scene->addChild(bullet);
		bullet->broadcast();
	}
	this->current = max(0, current - number);
}

int Laser::getMagazine() {
	return magazine;
}

int Laser::getDamage() {
	return damage;
}

float Laser::getReloadTime() {
	return reloadTime;
}

float Laser::getFireInterval() {
	return fireInterval;
}
