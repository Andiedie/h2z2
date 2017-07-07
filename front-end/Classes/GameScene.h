#pragma once

#include "utils/global.h"
#include "cocos2d.h"
#include "Sprite\Sprites.h"
#include <map>

USING_NS_CC;

#define FRAME_RATE 60
#define SYNC_RATE 20
#define SYNC_LIMIT FRAME_RATE/SYNC_RATE

class GameScene : public cocos2d::Layer
{
	template<typename Type1, typename Type2>
	bool _handleContact(cocos2d::Sprite *node1, cocos2d::Sprite *node2);
public:
	static cocos2d::Scene* createScene();
	cocos2d::Layer *uiLayer;

	PhysicsWorld* mWorld;
	Size visibleSize;
	Vec2 origin;
	Vec2 gameArea;
	std::set<Sprite*> outOfRangeCheck;

	Player* selfPlayer;
	std::string selfId;

	std::map<std::string, Player*> otherPlayers;
	bool started = false;
	bool alive = true;
	cocos2d::Label *hpLabel;

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	void onMouseMove(EventMouse* event);
	void onMouseDown(EventMouse* event);
	void addListener();
	void update(float);
	bool onContactBegin(PhysicsContact &contact);
	void gameOver();
	void updateHpLabel(float hp);

	void handleContact(Player* player, Bullet* bullet);
};

void resetPhysics(Node*, PhysicsBody*);

template<typename T>
bool isType(cocos2d::Sprite* ptr) {
	return dynamic_cast<T*>(ptr) != 0;
}

template<typename T>
T* castType(cocos2d::Sprite* ptr) {
	return dynamic_cast<T*>(ptr);
}
