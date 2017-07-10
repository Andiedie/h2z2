#include "Shotgun.h"
USING_NS_CC;
using namespace std;

string Shotgun::file = "shotgun";
int Shotgun::magazine = 6;
float Shotgun::fireInterval = 1.0f;
float Shotgun::reloadTime = 2.5f;
int Shotgun::damage = 32;
float Shotgun::speed = 600.0f;

Shotgun::Shotgun(std::string id) {
	this->initWithFile("weapon/" + Shotgun::file + ".png");
	this->current = Shotgun::magazine;
	this->id = id;
}

bool Shotgun::fire(bool force) {
	if (!force && (reloading || inFireInterval || current <= 0)) return false;
	setFireInterVal();
	auto player = getParent();
	auto scene = player->getParent();
	auto angle = player->getRotation();
	auto pos = player->getPosition();
	int kuosan = 17;
	auto b1 = Vec2(sinf(CC_DEGREES_TO_RADIANS(angle - kuosan*2)), cosf(CC_DEGREES_TO_RADIANS(angle - kuosan*2)));
	auto b2 = Vec2(sinf(CC_DEGREES_TO_RADIANS(angle - kuosan)), cosf(CC_DEGREES_TO_RADIANS(angle - kuosan)));
	auto b3 = Vec2(sinf(CC_DEGREES_TO_RADIANS(angle)), cosf(CC_DEGREES_TO_RADIANS(angle)));
	auto b4 = Vec2(sinf(CC_DEGREES_TO_RADIANS(angle + kuosan)), cosf(CC_DEGREES_TO_RADIANS(angle + kuosan)));
	auto b5 = Vec2(sinf(CC_DEGREES_TO_RADIANS(angle + kuosan*2)), cosf(CC_DEGREES_TO_RADIANS(angle + kuosan*2)));
	scene->addChild(new Bullet(file, pos + 60.0f * b1, damage, player->getRotation(), speed));
	scene->addChild(new Bullet(file, pos + 60.0f * b2, damage, player->getRotation(), speed));
	scene->addChild(new Bullet(file, pos + 60.0f * b3, damage, player->getRotation(), speed));
	scene->addChild(new Bullet(file, pos + 60.0f * b4, damage, player->getRotation(), speed));
	scene->addChild(new Bullet(file, pos + 60.0f * b5, damage, player->getRotation(), speed));
	this->current = max(0, current - 1);
	return true;
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

string Shotgun::getFile() {
	return file;
}

