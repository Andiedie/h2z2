#include "Uzi.h"
USING_NS_CC;
using namespace std;

string Uzi::file = "uzi";
int Uzi::magazine = 50;
float Uzi::fireInterval = 0.5;
float Uzi::reloadTime = 3;
int Uzi::damage = 8;

Uzi::Uzi(std::string id) {
	this->initWithFile("weapon/" + Uzi::file + ".png");
	this->current = Uzi::magazine;
	this->id = id;
}

void Uzi::fire() {
	auto player = getParent();
	auto scene = player->getParent();
	auto angle = player->getRotation();
	auto pos = player->getPosition();
	auto normalizedDirection = Vec2(sinf(CC_DEGREES_TO_RADIANS(angle)), cosf(CC_DEGREES_TO_RADIANS(angle)));
	pos += 45.0f * normalizedDirection;
	auto bullet = new Bullet(file, pos, player->getRotation(), 500.0f);
	scene->addChild(bullet);
	bullet->broadcast();
	pos += 45.0f * normalizedDirection;
	bullet = new Bullet(file, pos, player->getRotation(), 500.0f);
	scene->addChild(bullet);
	bullet->broadcast();
	pos += 45.0f * normalizedDirection;
	bullet = new Bullet(file, pos, player->getRotation(), 500.0f);
	scene->addChild(bullet);
	bullet->broadcast();
}

void Uzi::reload() {

}

int Uzi::getMagazine() {
	return magazine;
}

int Uzi::getDamage() {
	return damage;
}
