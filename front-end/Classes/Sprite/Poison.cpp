#include "Poison.h"
USING_NS_CC;
using namespace std;

Vec2 Poison::leftTop;
Vec2 Poison::leftBottom;
Vec2 Poison::rightTop;
Vec2 Poison::rightBottom;
Color4F Poison::color = Color4F(0, 150, 0, 90);
DrawNode* Poison::dn;
float Poison::radius = 0;
int Poison::extendStep = 5;
int Poison::damageStep = 25;

Poison::Poison() {}

DrawNode* Poison::init(Vec2 area) {
	Poison::dn = DrawNode::create();
	Poison::leftTop = Vec2(0, area.y);
	Poison::leftBottom = Vec2(0, 0);
	Poison::rightTop = Vec2(area.x, area.y);
	Poison::rightBottom = Vec2(area.x, 0);
	Poison::radius = 0;
	return dn;
}

void Poison::drawPoison() {
	static int count = 0;
	if (count++ < Poison::extendStep) return;
	count = 0;
	dn->clear();
	dn->drawDot(leftTop, Poison::radius, color);
	dn->drawDot(leftBottom, Poison::radius, color);
	dn->drawDot(rightTop, Poison::radius, color);
	dn->drawDot(rightBottom, Poison::radius, color);
	Poison::radius += 1.5;
}

float Poison::getRadius() {
	return Poison::radius;
}

int Poison::getDamage() {
	return pow(radius, 0.35)/2;
}

bool Poison::inside(Vec2 pos) {
	static int count = 0;
	if (count++ < Poison::damageStep) return false;
	count = 0;
	return pos.distance(leftTop) < radius ||
		pos.distance(leftBottom) < radius ||
		pos.distance(rightTop) < radius ||
		pos.distance(rightBottom) < radius;
}
