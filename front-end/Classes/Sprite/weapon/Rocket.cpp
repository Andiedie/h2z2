#include "Rocket.h"
USING_NS_CC;
using namespace std;

string Rocket::file = "rocket";
int Rocket::magazine = 8;
float Rocket::fireInterval = 0.5f;
float Rocket::reloadTime = 1.5f;
int Rocket::damage = 150;

Rocket::Rocket(std::string id) {
	this->initWithFile("weapon/" + Rocket::file + ".png");
	this->current = Rocket::magazine;
	this->id = id;
}

void Rocket::fire() {
	if (reloading || inFireInterval || current <= 0) return;
	setFireInterVal();
	auto player = getParent();
	auto scene = player->getParent();
	auto angle = player->getRotation();
	auto pos = player->getPosition();
	auto normalizedDirection = Vec2(sinf(CC_DEGREES_TO_RADIANS(angle)), cosf(CC_DEGREES_TO_RADIANS(angle)));
	pos += 45.0f * normalizedDirection;
	auto bullet = new Bullet(file, pos, damage, player->getRotation(), 600.0f);
	scene->addChild(bullet);
	bullet->broadcast();
	this->current = max(0, current - 1);
}

int Rocket::getMagazine() {
	return magazine;
}

int Rocket::getDamage() {
	return damage;
}

float Rocket::getReloadTime() {
	return reloadTime;
}

float Rocket::getFireInterval() {
	return fireInterval;
}
