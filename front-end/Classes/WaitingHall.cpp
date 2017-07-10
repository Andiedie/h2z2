#include "WaitingHall.h"
#include "GameScene.h"
#include "utils\global.h"
#include "ui\UITextField.h"
#include "ui\UIEditBox\UIEditBox.h"

USING_NS_CC;

Scene* WaitingHall::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = WaitingHall::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool WaitingHall::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto title = Label::create("H2Z2", "Microsoft YaHei UI", 96);
	title->setPosition(visibleSize.width / 2, visibleSize.height - 70.0f);
	this->addChild(title);

    this->visibleSize = Director::getInstance()->getVisibleSize();
    this->origin = Director::getInstance()->getVisibleOrigin();

	GSocket->on("error", [](GameSocket* client, GenericValue<UTF8<>>& data) {
		//CCLOG("manual-error: %s", data.GetString());
	});

	auto playerLabel = Label::createWithSystemFont("", "Microsoft YaHei UI", 18);
	playerLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 50.0f);
	playerLabel->setString("connecting...");

	GSocket->on("playerList", [=](GameSocket* client, GenericValue<UTF8<>>& arr) {
		playerLabel->setString("Current player: " + std::to_string(arr.Size()));
	});
	GSocket->on("gameStart", [=](GameSocket* client, GenericValue<UTF8<>>& data) {
		//CCLOG("game start!");
		// switch to game scene
		Director::getInstance()->pushScene(GameScene::createScene());
	});

	auto menu = Menu::create();
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 200.0f);
	auto startGameButton = MenuItemFont::create("START", [=](Ref* sender) {
		GSocket->sendEvent("requireGameStart");
	});
	menu->addChild(startGameButton);
	
	this->addChild(playerLabel, 1);

	this->addChild(menu, 2);

    return true;
}
