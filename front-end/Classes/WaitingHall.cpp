#include "WaitingHall.h"
#include "GameScene.h"
#include "utils\global.h"

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
    
    this->visibleSize = Director::getInstance()->getVisibleSize();
    this->origin = Director::getInstance()->getVisibleOrigin();

	GSocket->onConnection([](GameSocket* client) {

	});

	GSocket->on("error", [](GameSocket* client, Document& dom) {
		CCLOG("manual-error: %s", dom["data"].GetString());
	});

	auto playerLabel = Label::createWithSystemFont("", "Microsoft YaHei UI", 18);
	playerLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	playerLabel->setString("this is label");

	GSocket->on("playerList", [=](GameSocket* client, Document& dom) {
		auto& arr = dom["data"];
		playerLabel->setString("Current player: " + std::to_string(arr.Size()));
	});
	GSocket->on("gameStart", [=](GameSocket* client, Document& dom) {
		CCLOG("game start!");
		// switch to game scene
		Director::getInstance()->pushScene(GameScene::createScene());
	});

	auto menu = Menu::create();
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 200.0f);
	auto startGameButton = MenuItemFont::create("START", [=](Ref* sender) {
		CCLOG("clicked");
		GSocket->sendEvent("requireGameStart");
	});
	menu->addChild(startGameButton);

	this->addChild(playerLabel, 1);
	this->addChild(menu, 2);

    return true;
}


void WaitingHall::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
