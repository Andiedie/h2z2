#include "GameScene.h"
#include "utils\global.h"
#include <chrono>
#include <algorithm>

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

	auto background = Sprite::create("background.png");
	gameArea = background->getContentSize();
	Bullet::initAutoRemove(gameArea);
	background->setPosition(gameArea / 2);
	this->addChild(background, -1);

	// received as the game starts
	GSocket->on("initData", [=](GameSocket* client, Document& dom) {
		this->selfId = dom["data"]["selfId"].GetString();
		auto& arr = dom["data"]["players"];
		for (SizeType i = 0; i < arr.Size(); i++) {
			const std::string& id = arr[i].GetString();
			if (id == selfId) {
				this->selfPlayer = Player::create(gameArea / 2);
				this->addChild(selfPlayer, 1);

				// make the camera follow the player
				this->runAction(Follow::create(selfPlayer));
			}
			else {
				auto player = Player::create(gameArea / 2);
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
			player->sync(data);
		}
	});

	// deal with other players' shots
	GSocket->on("shoot", [=](GameSocket* client, Document& dom) {
		auto& data = dom["data"];
		auto bullet = Bullet::create(Vec2(data["posX"].GetDouble(), data["posY"].GetDouble()), data["angle"].GetDouble(), data["velocity"].GetDouble(), 0.0f);
		this->addChild(bullet, 1);
	});

	// manually update the physics world
	this->schedule(schedule_selector(GameScene::update), 1.0f / FRAME_RATE, kRepeatForever, 0.1f);

    return true;
}

void GameScene::update(float dt) {
	using std::max;
	using std::min;

	if (!started) return;
	static int frameCounter = 0;
	this->getScene()->getPhysicsWorld()->step(1.0f / FRAME_RATE);
	frameCounter++;
	if (frameCounter == SYNC_LIMIT) {
		frameCounter = 0;
		GSocket->sendEvent("sync", this->selfPlayer->createSyncData());
	}

	// bound the player in the map
	auto pos = selfPlayer->getPosition();
	auto size = selfPlayer->getContentSize() * selfPlayer->getScale();
	pos.x = min(pos.x, gameArea.x - size.width / 2);
	pos.x = max(pos.x, size.width / 2);
	pos.y = min(pos.y, gameArea.y - size.height / 2);
	pos.y = max(pos.y, size.height / 2);
	selfPlayer->setPosition(pos);
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		selfPlayer->setVelocityY(200.0f);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		selfPlayer->setVelocityY(-200.0f);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		selfPlayer->setVelocityX(-200.0f);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		selfPlayer->setVelocityX(200.0f);
		break;
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	auto body = selfPlayer->getPhysicsBody();
	switch (code) {
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		if (body->getVelocity().y < 0.0f) break;
		selfPlayer->setVelocityY(0.0f);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_S:
		if (body->getVelocity().y > 0.0f) break;
		selfPlayer->setVelocityY(0.0f);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		if (body->getVelocity().x > 0.0f) break;
		selfPlayer->setVelocityX(0.0f);
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		if (body->getVelocity().x < 0.0f) break;
		selfPlayer->setVelocityX(0.0f);
		break;
	}
}

void GameScene::onMouseMove(EventMouse* event) {
	auto pos = Vec2(event->getCursorX(), event->getCursorY());
	// position of physicsBody as relative to the camera
	selfPlayer->setRotation(-CC_RADIANS_TO_DEGREES((pos - selfPlayer->getPhysicsBody()->getPosition()).getAngle()) + 90.0f);
}

void GameScene::onMouseDown(EventMouse* event) {
	Bullet* bullet;
	switch (event->getMouseButton()) {
	case EventMouse::MouseButton::BUTTON_LEFT:
		// create a bullet
		bullet = Bullet::create(selfPlayer->getPosition(), selfPlayer->getRotation());
		this->addChild(bullet, 1);
		// broadcast shooting event
		bullet->broadcast();
		break;
	case EventMouse::MouseButton::BUTTON_RIGHT:
	default:
		break;
	}
}

bool GameScene::onContactBegin(PhysicsContact &contact) {
	auto node1 = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto node2 = (Sprite*)contact.getShapeB()->getBody()->getNode();

	// handle player-bullet collision
	if (_handleContact<Player, Bullet>(node1, node2)) return false;

	return false;
}

void GameScene::addListener() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);

	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(GameScene::onMouseMove, this);
	mouseListener->onMouseDown = CC_CALLBACK_1(GameScene::onMouseDown, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void GameScene::handleContact(Player* player, Bullet* bullet) {
	auto boom = Boom::create(player->getPosition());
	this->addChild(boom, 2);
	bullet->removeFromParentAndCleanup(true);
	if (player == selfPlayer) {
		// self-player was hit
		Document dom;
		dom.SetObject();
		dom.AddMember("type", "hit", dom.GetAllocator());
		dom.AddMember("damage", 20.0f, dom.GetAllocator());	// TODO: damage determined by bullet type
		GSocket->sendEvent("broadcast", dom);
	}
}

template<typename Type1, typename Type2>
bool GameScene::_handleContact(cocos2d::Sprite *node1, cocos2d::Sprite *node2) {
	if (isType<Type1>(node1) && isType<Type2>(node2)) {
		handleContact(castType<Type1>(node1), castType<Type2>(node2));
		return true;
	}
	if (isType<Type1>(node2) && isType<Type2>(node1)) {
		handleContact(castType<Type1>(node2), castType<Type2>(node1));
		return true;
	}
	return false;
}


void resetPhysics(Node* node, PhysicsBody* body) {
	node->removeComponent(node->getPhysicsBody());
	node->setPhysicsBody(body);
}
