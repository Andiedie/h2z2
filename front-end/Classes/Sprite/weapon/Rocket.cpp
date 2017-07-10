#include "Rocket.h"
USING_NS_CC;
using namespace std;

string Rocket::file = "rocket";
int Rocket::magazine = 5;
float Rocket::fireInterval = 1.5f;
float Rocket::reloadTime = 2.5f;
int Rocket::damage = 120;
float Rocket::speed = 900.0f;

Rocket::Rocket(std::string id) {
	this->initWithFile("weapon/" + Rocket::file + ".png");
	this->current = Rocket::magazine;
	this->id = id;
}

bool Rocket::fire(bool force) {
	if (!force && (reloading || inFireInterval || current <= 0)) return false;
	setFireInterVal();
	auto player = getParent();
	auto scene = player->getParent();
	auto angle = player->getRotation();
	auto pos = player->getPosition();
	pos += 45.0f * Vec2(sinf(CC_DEGREES_TO_RADIANS(angle)), cosf(CC_DEGREES_TO_RADIANS(angle)));
	auto bullet = new Bullet(file, pos, damage, player->getRotation(), speed);
	bullet->setScale(0.7f);
	scene->addChild(bullet);
	this->current = max(0, current - 1);
	return true;
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

string Rocket::getFile() {
	return file;
}

