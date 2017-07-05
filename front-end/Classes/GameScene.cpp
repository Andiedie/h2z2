#include "GameScene.h"
#include "utils\global.h"
#include <chrono>

USING_NS_CC;
using namespace std::chrono;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();

	scene->getPhysicsWorld()->setAutoStep(false);
	scene->getPhysicsWorld()->setGravity(Vec2::ZERO);
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
	layer->mWorld = scene->getPhysicsWorld();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	this->addListener();
    
    this->visibleSize = Director::getInstance()->getVisibleSize();
    this->origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create("background.jpg");
	background->setPosition(visibleSize / 2);
	this->addChild(background, -1);

	// received as the game starts
	GSocket->on("initData", [=](GameSocket* client, Document& dom) {
		this->selfId = dom["data"]["selfId"].GetString();
		auto& arr = dom["data"]["players"];
		for (SizeType i = 0; i < arr.Size(); i++) {
			const std::string& id = arr[i].GetString();
			if (id == selfId) {
				this->selfPlayer = createPlayer();
				this->selfPlayer->setPosition(visibleSize / 2);
				this->addChild(selfPlayer, 1);

				// make the camera follow the player
				this->runAction(Follow::create(selfPlayer));
			}
			else {
				auto player = createPlayer(id);
				player->setPosition(visibleSize / 2);
				this->addChild(player, 1);
				this->otherPlayers.insert(std::make_pair(id, player));
			}
		}

		started = true;
	});

	// received periodically, like every x frames
	GSocket->on("sync", [=](GameSocket* client, Document& dom) {
		auto& arr = dom["data"];
		for (SizeType i = 0; i < arr.Size(); i++) {
			auto& data = arr[i]["data"];

			// check ping
			// milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
			// CCLOG("ping: %lld", ms.count() - data["timestamp"].GetInt64());

			const std::string& id = arr[i]["id"].GetString();
			auto it = this->otherPlayers.find(id);
			if (it == this->otherPlayers.end()) continue; // data of selfPlayer, just ignore it

			auto player = it->second;
			syncSprite(player, data);
		}
	});

	// manually update the physics world
	schedule(schedule_selector(GameScene::update), 1.0f / FRAME_RATE, kRepeatForever, 0.1f);

    return true;
}

void GameScene::update(float dt) {
	if (!started) return;
	static int frameCounter = 0;
	CCLOG("%lf %lf", selfPlayer->getPhysicsBody()->getPosition().x, selfPlayer->getPhysicsBody()->getPosition().y);
	this->getScene()->getPhysicsWorld()->step(1.0f / FRAME_RATE);
	frameCounter++;
	if (frameCounter == SYNC_LIMIT) {
		frameCounter = 0;
		// CCLOG("sync");
		// TODO: sync with server
		GSocket->sendEvent("sync", createSyncData(this->selfPlayer));
	}
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		setSpeedY(selfPlayer, 200.0f);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		setSpeedY(selfPlayer, -200.0f);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		setSpeedX(selfPlayer, -200.0f);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		setSpeedX(selfPlayer, 200.0f);
		break;
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	auto body = selfPlayer->getPhysicsBody();
	switch (code) {
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		if (body->getVelocity().y < 0.0f) break;
		setSpeedY(selfPlayer, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		if (body->getVelocity().y > 0.0f) break;
		setSpeedY(selfPlayer, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		if (body->getVelocity().x > 0.0f) break;
		setSpeedX(selfPlayer, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		if (body->getVelocity().x < 0.0f) break;
		setSpeedX(selfPlayer, 0);
		break;
	}
}

void GameScene::onMouseMove(EventMouse* event) {
	auto pos = Vec2(event->getCursorX(), event->getCursorY());
	// position of physicsBody as relative to the camera
	selfPlayer->setRotation(-CC_RADIANS_TO_DEGREES((pos - selfPlayer->getPhysicsBody()->getPosition()).getAngle()) + 90.0f);
}

void GameScene::addListener() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);

	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(GameScene::onMouseMove, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	// auto contactListener = EventListenerPhysicsContact::create();
	// contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onConcactBegin, this);
	// _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void addSpeed(Node* node, Vec2 speed) {
	auto body = node->getPhysicsBody();
	if (body) {
		body->setVelocity(body->getVelocity() + speed);
	}
}

void setSpeedX(Node* node, float spx) {
	auto body = node->getPhysicsBody();
	if (body) {
		body->setVelocity(Vec2(spx, body->getVelocity().y));
	}
}

void setSpeedY(Node* node, float spy) {
	auto body = node->getPhysicsBody();
	if (body) {
		body->setVelocity(Vec2(body->getVelocity().x, spy));
	}
}

void resetPhysics(Node* node, PhysicsBody* body) {
	node->removeComponent(node->getPhysicsBody());
	node->setPhysicsBody(body);
}

Sprite* createPlayer(const std::string& id) {
	auto player = Sprite::create("player.png");
	player->setScale(0.5f);
	auto playerBody = PhysicsBody::createBox(player->getContentSize(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
	playerBody->setCategoryBitmask(0x00000001);
	playerBody->setCollisionBitmask(0xFFFFFFFE); // disable collision between players
	player->setPhysicsBody(playerBody);
	return player;
}

Document createSyncData(Node* player) {
	Document dom;
	auto body = player->getPhysicsBody();
	dom.SetObject();

	// position & speed & angle
	rapidjson::Value speedX, speedY, posX, posY;
	auto speed = body->getVelocity();

	// note: must use position of 'player' instead of 'body', to have a correct position
	//	 while working with the Follow Action
	auto pos = player->getPosition();	

	dom.AddMember("speedX", speed.x, dom.GetAllocator());
	dom.AddMember("speedY", speed.y, dom.GetAllocator());
	dom.AddMember("posX", pos.x, dom.GetAllocator());
	dom.AddMember("posY", pos.y, dom.GetAllocator());
	dom.AddMember("angle", player->getRotation(), dom.GetAllocator());

	milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	dom.AddMember("timestamp", ms.count(), dom.GetAllocator());	// used to check ping
	return dom;
}

void syncSprite(Node* sprite, GenericValue<rapidjson::UTF8<>>& data) {
	auto body = sprite->getPhysicsBody();
	body->setVelocity(Vec2(data["speedX"].GetDouble(), data["speedY"].GetDouble()));
	sprite->setPosition(data["posX"].GetDouble(), data["posY"].GetDouble());
	sprite->setRotation(data["angle"].GetDouble());
}
