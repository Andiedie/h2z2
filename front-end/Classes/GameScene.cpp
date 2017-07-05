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

	GSocket->on("initData", [=](GameSocket* client, Document& dom) {
		this->selfId = dom["data"]["selfId"].GetString();
		auto& arr = dom["data"]["players"];
		for (SizeType i = 0; i < arr.Size(); i++) {
			const std::string& id = arr[i].GetString();
			if (id == selfId) {
				this->selfPlayer = createPlayer();
				this->selfPlayer->setPosition(visibleSize / 2);
				this->addChild(selfPlayer, 1);
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

	GSocket->on("sync", [=](GameSocket* client, Document& dom) {
		auto& arr = dom["data"];
		for (SizeType i = 0; i < arr.Size(); i++) {
			// check ping
			// milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
			auto& data = arr[i]["data"];
			// CCLOG("ping: %lld", ms.count() - data["timestamp"].GetInt64());

			const std::string& id = arr[i]["id"].GetString();
			auto it = this->otherPlayers.find(id);
			if (it == this->otherPlayers.end()) continue; // data of selfPlayer, just ignore it

			auto player = it->second;
			auto body = player->getPhysicsBody();
			body->setVelocity(Vec2(data["speedX"].GetDouble(), data["speedY"].GetDouble()));
			player->setPosition(data["posX"].GetDouble(), data["posY"].GetDouble());
		}
	});

	schedule(schedule_selector(GameScene::update), 1.0f / FRAME_RATE, kRepeatForever, 0.1f);

    return true;
}

void GameScene::update(float dt) {
	if (!started) return;
	static int frameCounter = 0;
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
		setSpeedY(selfPlayer, 150.0f);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		setSpeedY(selfPlayer, -150.0f);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		setSpeedX(selfPlayer, -150.0f);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		setSpeedX(selfPlayer, 150.0f);
		break;
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		setSpeedY(selfPlayer, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		setSpeedY(selfPlayer, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		setSpeedX(selfPlayer, 0);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		setSpeedX(selfPlayer, 0);
		break;
	}
}

void GameScene::addListener() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

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
	player->setScale(0.7f);
	auto playerBody = PhysicsBody::createBox(player->getContentSize(), PhysicsMaterial(10.0f, 0.0f, 0.0f));
	player->setPhysicsBody(playerBody);
	return player;
}

Document createSyncData(Sprite* player) {
	Document dom;
	auto body = player->getPhysicsBody();
	dom.SetObject();
	rapidjson::Value speedX, speedY, posX, posY;
	auto speed = body->getVelocity();
	auto pos = body->getPosition();
	speedX.SetDouble(speed.x);
	speedY.SetDouble(speed.y);
	posX.SetDouble(pos.x);
	posY.SetDouble(pos.y);
	dom.AddMember("speedX", speedX, dom.GetAllocator());
	dom.AddMember("speedY", speedY, dom.GetAllocator());
	dom.AddMember("posX", posX, dom.GetAllocator());
	dom.AddMember("posY", posY, dom.GetAllocator());
	milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	dom.AddMember("timestamp", ms.count(), dom.GetAllocator());	// used to check ping
	return dom;
}
