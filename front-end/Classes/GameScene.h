#pragma once

#include "utils/global.h"
#include "cocos2d.h"
#include <map>

USING_NS_CC;

#define FRAME_RATE 60
#define SYNC_RATE 20
#define SYNC_LIMIT FRAME_RATE/SYNC_RATE

class GameScene : public cocos2d::Layer
{
public:
	PhysicsWorld* mWorld;
    static cocos2d::Scene* createScene();
	Size visibleSize;
	Vec2 origin;
	Vec2 gameArea;
	std::set<Sprite*> outOfRangeCheck;

	Sprite* selfPlayer;
	std::string selfId;

	std::map<std::string, Sprite*> otherPlayers;
	bool started = false;

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	void onMouseMove(EventMouse* event);
	void onMouseDown(EventMouse* event);
	void checkOutOfRange(float);
	void addListener();
	void update(float);
	Sprite* createPlayer(const std::string& id = "");
	Sprite* createBullet(Vec2 pos, float angle);
	Sprite* createBoom(Vec2 pos);
	bool onContactBegin(PhysicsContact &contact);
};

void addSpeed(Node*, Vec2);
void setSpeedX(Node*, float);
void setSpeedY(Node*, float);
void resetPhysics(Node*, PhysicsBody*);
Document createSyncData(Node*);
void syncSprite(Node*, GenericValue<rapidjson::UTF8<>>&);