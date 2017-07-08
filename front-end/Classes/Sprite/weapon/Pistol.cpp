#include "Pistol.h"
USING_NS_CC;
using namespace std;

string Pistol::file = "pistol";
int Pistol::magazine = 7;
float Pistol::fireInterval = 0.75;
float Pistol::reloadTime = 1.5;
int Pistol::damage = 30;

Pistol::Pistol(std::string id) {
	this->initWithFile("weapon/" + Pistol::file + ".png");
	this->current = Pistol::magazine;
	this->id = id;
}

void Pistol::fire() {
	auto player = getParent();
	auto scene = player->getParent();
	auto angle = player->getRotation();
	auto pos = player->getPosition();
	auto normalizedDirection = Vec2(sinf(CC_DEGREES_TO_RADIANS(angle)), cosf(CC_DEGREES_TO_RADIANS(angle)));
	pos += 45.0f * normalizedDirection;
	auto bullet = new Bullet(file, pos, player->getRotation(), 500.0f);
	scene->addChild(bullet);
	bullet->broadcast();
}

void Pistol::reload() {

}

int Pistol::getMagazine() {
	return magazine;
}

int Pistol::getDamage() {
	return damage;
}
