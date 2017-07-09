#include "Wall.h"

USING_NS_CC;

Wall::Wall() {}

Wall* Wall::create(GenericValue<UTF8<>> &data) {
	auto wall = new Wall();
	if (wall && wall->initWithFile("wall.png")) {
		wall->setPosition(data["posX"].GetDouble(), data["posY"].GetDouble());
		wall->setScaleX(data["scaleX"].GetDouble());
		wall->setScaleY(data["scaleY"].GetDouble());

		auto body = PhysicsBody::createBox(wall->getContentSize(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
		body->setDynamic(false);
		body->setCategoryBitmask(0xFFFFFFFF);
		body->setCollisionBitmask(0xFFFFFFFF);
		body->setContactTestBitmask(0xFFFFFFFF);
		wall->setPhysicsBody(body);

		wall->setRotation(data["angle"].GetDouble());

		wall->autorelease();
		return wall;
	}
	CC_SAFE_DELETE(wall);
	return nullptr;
}