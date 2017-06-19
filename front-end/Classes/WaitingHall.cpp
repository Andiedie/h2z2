#include "WaitingHall.h"
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

	auto client = GSocket;
	client->onConnection([](GameSocket* client) {

	});

	auto menu = Menu::create();

	auto playerLabel = Label::createWithSystemFont("", "Microsoft YaHei UI", 18);
	playerLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	playerLabel->setString("this is label");

	client->on("playerList", [=](GameSocket* client, Document& dom) {
		playerLabel->setString(dom["data"].GetString());
	});

	this->addChild(playerLabel, 1);

    return true;
}


void WaitingHall::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
