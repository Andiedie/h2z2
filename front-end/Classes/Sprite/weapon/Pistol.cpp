#include "Pistol.h"
USING_NS_CC;
using namespace std;

string Pistol::file = "pistol";
int Pistol::magazine = 7;
float Pistol::fireInterval = 0.75;
float Pistol::reloadTime = 1.5;

Pistol::Pistol(string id) {
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
	scene->addChild(new Bullet(file, pos, player->getRotation(), 500.0f));
}