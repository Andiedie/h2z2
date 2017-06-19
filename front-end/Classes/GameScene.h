#pragma once

#include "utils/global.h"
#include "cocos2d.h"
#include <map>

USING_NS_CC;

#define FRAME_RATE 60
#define SYNC_RATE 20
// #define SYNC_RATE 5
#define SYNC_LIMIT FRAME_RATE/SYNC_RATE

class GameScene : public cocos2d::Layer
{
public:
	PhysicsWorld* mWorld;
    static cocos2d::Scene* createScene();
	Size visibleSize;
	Vec2 origin;

	Sprite* selfPlayer;
	std::string selfId;

	std::map<std::string, Sprite*> otherPlayers;
	bool started = false;

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	void addListener();
	void update(float);
};

void addSpeed(Node*, Vec2);
void setSpeedX(Node*, float);
void setSpeedY(Node*, float);
void resetPhysics(Node*, PhysicsBody*);
Sprite* createPlayer(const std::string& id = "");
Document createSyncData(Sprite*);