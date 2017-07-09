#include "Shotgun.h"
USING_NS_CC;
using namespace std;

string Shotgun::file = "shotgun";
int Shotgun::magazine = 20;
float Shotgun::fireInterval = 0.75f;
float Shotgun::reloadTime = 2.0f;
int Shotgun::damage = 20;

Shotgun::Shotgun(std::string id) {
	this->initWithFile("weapon/" + Shotgun::file + ".png");
	this->current = Shotgun::magazine;
	this->id = id;
}

void Shotgun::fire() {
	if (reloading || inFireInterval || current <= 0) return;
	setFireInterVal();
	auto player = getParent();
	auto scene = player->getParent();
	auto angle = player->getRotation();
	auto pos = player->getPosition();
	auto normalizedDirection = Vec2(sinf(CC_DEGREES_TO_RADIANS(angle)), cosf(CC_DEGREES_TO_RADIANS(angle)));
	pos += 45.0f * normalizedDirection;
	pos.y -= 50;
	auto bullet = new Bullet(file, pos, damage, player->getRotation(), 1000.0f);
	scene->addChild(bullet);
	bullet->broadcast();
	pos.y += 10;
	bullet = new Bullet(file, pos, damage, player->getRotation(), 1000.0f);
	scene->addChild(bullet);
	bullet->broadcast();
	pos.y += 10;
	bullet = new Bullet(file, pos, damage, player->getRotation(), 1000.0f);
	scene->addChild(bullet);
	bullet->broadcast();
	pos.y += 10;
	bullet = new Bullet(file, pos, damage, player->getRotation(), 1000.0f);
	scene->addChild(bullet);
	bullet->broadcast();
	pos.y += 10;
	bullet = new Bullet(file, pos, damage, player->getRotation(), 1000.0f);
	scene->addChild(bullet);
	bullet->broadcast();
	this->current = max(0, current - 5);
}

int Shotgun::getMagazine() {
	return magazine;
}

int Shotgun::getDamage() {
	return damage;
}

float Shotgun::getReloadTime() {
	return reloadTime;
}

float Shotgun::getFireInterval() {
	return fireInterval;
}
