#include "GameScene.h"
#include "utils\global.h"
#include <chrono>
#include <algorithm>

#define FOLLOW_TAG 10086

USING_NS_CC;
using namespace std::chrono;
using namespace std;

Scene* GameScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();

	scene->getPhysicsWorld()->setAutoStep(false);
	scene->getPhysicsWorld()->setGravity(Vec2::ZERO);

	// 'layer' is an autorelease object
	auto gameLayer = GameScene::create();
	gameLayer->mWorld = scene->getPhysicsWorld();
	gameLayer->uiLayer = Layer::create();
	gameLayer->weaponLabel = Label::createWithSystemFont("", "Arial", 30);
	gameLayer->weaponLabel->setPosition(gameLayer->visibleSize.width - 100.0f, 20.0f);
	gameLayer->hpLabel = Label::createWithSystemFont("", "Arial", 30);
	gameLayer->hpLabel->setPosition(gameLayer->visibleSize.width / 2, 20.0f);
	gameLayer->infoLabel = Label::createWithSystemFont("", "Arial", 24);
	gameLayer->aliveLabel = Label::createWithSystemFont("", "Arial", 30);
	gameLayer->aliveLabel->setPosition(gameLayer->visibleSize.width - 50.0f, gameLayer->visibleSize.height - 20.0f);
	gameLayer->uiLayer->addChild(gameLayer->weaponLabel);
	gameLayer->uiLayer->addChild(gameLayer->hpLabel);
	gameLayer->uiLayer->addChild(gameLayer->infoLabel);
	gameLayer->uiLayer->addChild(gameLayer->aliveLabel);

	// add layer as a child to scene
	scene->addChild(gameLayer);
	scene->addChild(gameLayer->uiLayer, 1);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init() {
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
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
	Weapon::init();
	addChild(Poison::init(gameArea), 6);

	// received as the game starts
	GSocket->on("initData", [=](GameSocket* client, GenericValue<UTF8<>>& data) {
		AUDIO->playEffect("sound/start.wav");
		this->selfId = data["selfId"].GetString();
		auto& arr = data["players"];
		aliveLabel->setString(to_string(arr.Size()) + " Alive");
		for (SizeType i = 0; i < arr.Size(); i++) {
			const string& id = arr[i]["id"].GetString();
			auto& colorData = arr[i]["color"];
			const Color3B color = Color3B(colorData["r"].GetInt(), colorData["g"].GetInt(), colorData["b"].GetInt());
			if (id == selfId) {
				auto& selfPos = data["selfPos"];
				selfPlayer = Player::create(color, Vec2(selfPos["x"].GetDouble(), selfPos["y"].GetDouble()));
				this->addChild(selfPlayer, 1);
				updateHpLabel();
				//测试用的枪 懒得满图找
				//addChild(Weapons::create(0, "0", selfPlayer->getPosition() + Vec2(-200, 0)));
				//addChild(Weapons::create(1, "1", selfPlayer->getPosition() + Vec2(-100, 0)));
				//addChild(Weapons::create(2, "2", selfPlayer->getPosition() + Vec2(0, 100)));
				//addChild(Weapons::create(3, "3", selfPlayer->getPosition() + Vec2(100, 0)));
				//addChild(Weapons::create(4, "4", selfPlayer->getPosition() + Vec2(200, 0)));

				// make the camera follow the player
				Action* follow = Follow::create(selfPlayer);
				follow->setTag(FOLLOW_TAG);
				this->runAction(follow);
			} else {
				auto name = Label::createWithSystemFont(arr[i]["name"].GetString(), "Arial", 24);
				addChild(name, 5);
				auto player = Player::create(color);
				player->name = name;
				this->addChild(player, 1);
				this->otherPlayers.insert(make_pair(id, player));
			}
		}

		auto& packs = data["healPacks"];
		for (SizeType i = 0; i < packs.Size(); i++) {
			auto pack = HealPack::create(packs[i]);
			this->addChild(pack, 0);
		}

		auto& weapons = data["weapons"];
		for (SizeType i = 0; i < weapons.Size(); i++) {
			this->addChild(Weapons::create(weapons[i]["type"].GetInt(), weapons[i]["id"].GetString(), Vec2(weapons[i]["posX"].GetDouble(), weapons[i]["posY"].GetDouble())));
		}

		auto& walls = data["walls"];
		for (SizeType i = 0; i < walls.Size(); i++) {
			auto wall = Wall::create(walls[i]);
			this->addChild(wall, 1);
		}
		started = true;
	});

	// received periodically, like every x frames
	GSocket->on("sync", [=](GameSocket* client, GenericValue<UTF8<>>& arr) {
		for (SizeType i = 0; i < arr.Size(); i++) {
			auto& data = arr[i]["data"];

			// check ping
			// milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
			// CCLOG("ping: %lld", ms.count() - data["timestamp"].GetInt64());

			const string& id = arr[i]["id"].GetString();
			auto it = this->otherPlayers.find(id);
			if (it == this->otherPlayers.end()) continue; // data of selfPlayer, just ignore it

			auto player = it->second;
			player->sync(data);
		}
	});

	GSocket->on("gameover", [=](GameSocket* client, GenericValue<UTF8<>>&data) {
		auto winner = data["winner"].GetString();
		if (winner == selfId) {
			// you win
			auto info = Label::create("You Win!", "Arial", 72);
			info->setPosition(visibleSize.width / 2, visibleSize.height - 100.0f);
			uiLayer->addChild(info);
		}
		started = false;
		_eventDispatcher->removeEventListenersForType(EventListener::Type::KEYBOARD);
		_eventDispatcher->removeEventListenersForType(EventListener::Type::MOUSE);
		GSocket->removeEventHandler("initData");
		GSocket->removeEventHandler("sync");
		GSocket->removeEventHandler("logout");
		GSocket->removeEventHandler("fire");
		GSocket->removeEventHandler("dead");
		GSocket->removeEventHandler("eatPack");
		GSocket->removeEventHandler("takeWeapon");
		GSocket->removeEventHandler("dropWeapon");
		this->runAction(Sequence::create(
			DelayTime::create(3.0f),
			CallFunc::create([=]() {
			    AUDIO->stopAllEffects();
				GSocket->removeEventHandler("gameover");
				Director::getInstance()->popScene();
			}),
			NULL
		));
	});

	GSocket->on("logout", [=](GameSocket* client, GenericValue<UTF8<>> &data) {
		//CCLOG("logout");
		auto who = data.GetString();
		auto it = otherPlayers.find(who);
		if (it != otherPlayers.end()) {
			auto player = it->second;
			updateDeadLabel(player->name->getString() + " leave the game");
			otherPlayers.erase(it);
			aliveLabel->setString(to_string(otherPlayers.size() + 1) + " Alive");
			player->removeFromParentAndCleanup(true);
		}
	});

	// deal with other players' shots
	GSocket->on("fire", [=](GameSocket* client, GenericValue<UTF8<>>& data) {
		auto weapon = Weapons::getById(data["weaponId"].GetString());
		weapon->fire(true);
	});

	GSocket->on("dead", [=](GameSocket* client, GenericValue<UTF8<>>& data) {
		auto who = data["from"].GetString();
		auto it = otherPlayers.find(who);
		if (it != otherPlayers.end()) {
			updateDeadLabel(it->second->name->getString() + " is killed");
			it->second->removeFromParentAndCleanup(true);
			otherPlayers.erase(it);
			aliveLabel->setString(to_string(otherPlayers.size() + 1) + " Alive");
		}
	});

	GSocket->on("eatPack", [=](GameSocket* client, GenericValue<UTF8<>>& data) {
		HealPack::remove(data["packId"].GetString());
	});

	GSocket->on("takeWeapon", [=](GameSocket* client, GenericValue<UTF8<>>& data) {
		auto player = getPlayerById(data["from"].GetString());
		auto weapon = Weapons::getById(data["weaponId"].GetString());
		if (player != nullptr && weapon != nullptr) {
			player->takeWeapon(weapon);
		}
	});

	GSocket->on("dropWeapon", [=](GameSocket* client, GenericValue<UTF8<>>& data) {
		auto player = getPlayerById(data["from"].GetString());
		if (player != nullptr) {
			auto w = player->dropWeapon();
			w->current = data["current"].GetInt();
			this->addChild(w);
		}
	});

	// manually update the physics world
	this->schedule(schedule_selector(GameScene::update), 1.0f / FRAME_RATE, kRepeatForever, 0.1f);
	return true;
}

void GameScene::update(float dt) {
	static int frameCounter = 0;

	auto it = otherPlayers.begin();
	this->getScene()->getPhysicsWorld()->step(1.0f / FRAME_RATE);
	while (it != otherPlayers.end()) {
		auto pos = it->second->getPosition();
		it->second->name->setPosition(Vec2(pos.x, pos.y + 70));
		it++;
	}
	Poison::drawPoison();
	if (!started || !selfPlayer) return;
	updateWeaponLabel();
	frameCounter++;
	if (!selfPlayer) return;
	if (frameCounter >= SYNC_LIMIT) {
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

	// player move
	selfPlayer->setVelocityX(selfPlayer->x * 250.f);
	selfPlayer->setVelocityY(selfPlayer->y * 250.f);

	// poison
	if (Poison::inside(pos)) {
		if (!selfPlayer->damage(Poison::getDamage())) {
			selfPlayer->broadcastDead();
			selfDead();
		}
		updateHpLabel();
	}
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	if (!selfPlayer) return;
	Weapon* w;
	switch (code) {
		case cocos2d::EventKeyboard::KeyCode::KEY_W:
			selfPlayer->y = 1;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_S:
			selfPlayer->y = -1;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_A:
			selfPlayer->x = -1;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_D:
			selfPlayer->x = 1;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_G:
			w = selfPlayer->dropWeapon();
			if (w != nullptr) {
				this->addChild(w);
				w->broadCastDropped();
			}
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_R:
			if (selfPlayer->weapon != nullptr) {
				selfPlayer->weapon->reload();
			}
			break;
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	if (!selfPlayer) return;
	auto body = selfPlayer->getPhysicsBody();
	switch (code) {
		case cocos2d::EventKeyboard::KeyCode::KEY_W:
			if (selfPlayer->y == -1) break;
			selfPlayer->y = 0;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_S:
			if (selfPlayer->y == 1) break;
			selfPlayer->y = 0;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_A:
			if (selfPlayer->x == 1) break;
			selfPlayer->x = 0;
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_D:
			if (selfPlayer->x == -1) break;
			selfPlayer->x = 0;
			break;
	}
}

void GameScene::onMouseMove(EventMouse* event) {
	if (!selfPlayer) return;
	auto pos = Vec2(event->getCursorX(), event->getCursorY());
	// position of physicsBody as relative to the camera
	selfPlayer->setRotation(-CC_RADIANS_TO_DEGREES((pos - selfPlayer->getPhysicsBody()->getPosition()).getAngle()) + 90.0f);
}

void GameScene::onMouseDown(EventMouse* event) {
	if (!started) return;
	switch (event->getMouseButton()) {
		case EventMouse::MouseButton::BUTTON_LEFT:
			if (selfPlayer) {
				if (selfPlayer->weapon != nullptr) {
					if (selfPlayer->weapon->fire())
						selfPlayer->weapon->broadCastFire();
				}
			} else {
				changeView();
			}
			break;
	}
}

bool GameScene::onContactBegin(PhysicsContact &contact) {
	auto node1 = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto node2 = (Sprite*)contact.getShapeB()->getBody()->getNode();

	// handle player-bullet collision
	if (_handleContact<Player, Bullet>(node1, node2)) return false;
	// handle player-healpack collision
	if (_handleContact<Player, HealPack>(node1, node2)) return false;
	// handle player-weapon collision
	if (_handleContact<Player, Weapon>(node1, node2)) return false;
	// handle wall-bullet collision
	if (_handleContact<Wall, Bullet>(node1, node2)) return false;

	return true;
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
	boom->setScale(pow(bullet->getDamage(), 0.4)*0.2);
	addChild(boom, 2);
	if (player == selfPlayer) {
		// self-player was hit
		if (!player->damage(bullet->getDamage())) {
			player->broadcastDead();
			selfDead();
		}
		updateHpLabel();
	}
	bullet->removeFromParentAndCleanup(true);
}

void GameScene::handleContact(Player* player, HealPack* pack) {
	if (player->getHp() >= Player::maxHp) return;
	player->heal(pack->getHp());
	updateHpLabel();
	pack->broadcastEaten();
	HealPack::remove(pack->getId());
}

void GameScene::handleContact(Player* player, Weapon* weapon) {
	if (player == selfPlayer && selfPlayer->weapon == nullptr) {
		player->takeWeapon(weapon);
		weapon->broadCastToken();
	}
}

void GameScene::handleContact(Wall *wall, Bullet *bullet) {
	auto boom = Boom::create(bullet->getPosition());
	boom->setScale(pow(bullet->getDamage(), 0.4)*0.2);
	addChild(boom, 2);
	bullet->removeFromParentAndCleanup(true);
}

void GameScene::selfDead() {
	// UI
	updateDeadLabel("You died!");
	aliveLabel->setString(to_string(otherPlayers.size()) + " Alive");
	if (started) {
		auto info = Label::create("You Died!", "Arial", 72);
		info->setPosition(visibleSize.width / 2, visibleSize.height - 100.0f);
		uiLayer->addChild(info);
	}
	weaponLabel->setString("");
	hpLabel->setString("");
	// 移出键盘监听器
	_eventDispatcher->removeEventListenersForType(EventListener::Type::KEYBOARD);
	//_eventDispatcher->removeEventListenersForType(EventListener::Type::MOUSE);
	// 移除玩家
	selfPlayer->removeFromParentAndCleanup(true);
	selfPlayer = nullptr;
	// 查看其他玩家
	changeView();
}

void GameScene::updateHpLabel() {
	if (!selfPlayer) return;
	hpLabel->setString("HP: " + to_string(selfPlayer->getHp()));
}

void GameScene::updateWeaponLabel() {
	if (!selfPlayer) return;
	static char buffer[40];
	auto w = selfPlayer->weapon;
	if (w == nullptr) {
		weaponLabel->setString("");
	} else {
		if (w->current == -1) sprintf(buffer, "ammo: reloading"); // reloading
		else sprintf(buffer, "ammo: %d/%d", w->current, w->getMagazine());
		weaponLabel->setString(buffer);
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

Player* GameScene::getPlayerById(string id) {
	auto it = otherPlayers.find(id);
	if (it != otherPlayers.end()) return it->second;
	return nullptr;
}

void GameScene::updateDeadLabel(string msg) {
	infoLabel->setVisible(true);
	infoLabel->setString(infoLabel->getString() + "\n" + msg);
	auto size = infoLabel->getContentSize();
	infoLabel->setPosition(size.width/2+20.0f, size.height/2.0+20.0f);
	runAction(Sequence::create(DelayTime::create(10.0f), CallFunc::create([this]() {
		infoLabel->setVisible(false);
	}), nullptr));
}

void GameScene::changeView() {
	if (selfPlayer || otherPlayers.size() <= 1) return;
	static auto it = otherPlayers.begin();
	if (it == otherPlayers.end()) {
		it = otherPlayers.begin();
	}
	this->stopActionByTag(FOLLOW_TAG);
	auto follow = Follow::create(it->second);
	if (!follow) return;
	follow->setTag(FOLLOW_TAG);
	this->runAction(follow);
	hpLabel->setString("Watching \"" + it->second->name->getString() + "\" (click to change)");
	it++;
}
