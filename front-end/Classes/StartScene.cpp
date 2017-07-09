#include "StartScene.h"
#include "WaitingHall.h"

USING_NS_CC;

Scene* StartScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = StartScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init() {
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	this->visibleSize = Director::getInstance()->getVisibleSize();
	this->origin = Director::getInstance()->getVisibleOrigin();

	auto title = Label::create("H2Z2", "Microsoft YaHei UI", 96);
	title->setPosition(visibleSize.width / 2, visibleSize.height - 70.0f);
	this->addChild(title);

	auto hostLabel = Label::create("Host", "Microsoft YaHei UI", 28);
	hostLabel->setPosition(visibleSize.width / 2 - 75.0f, visibleSize.height / 2 + 30.0f);
	this->addChild(hostLabel);

	hostInput = ui::TextField::create("127.0.0.1", "Microsoft YaHei UI", 30.0f);
	hostInput->setPosition(Vec2(visibleSize.width / 2 - 75.0f, visibleSize.height / 2));
	hostInput->setEnabled(true);
	hostInput->setText("127.0.0.1");
	this->addChild(hostInput);

	auto portLabel = Label::create("Port", "Microsoft YaHei UI", 28);
	portLabel->setPosition(visibleSize.width / 2 + 75.0f, visibleSize.height / 2 + 30.0f);
	this->addChild(portLabel);

	portInput = ui::TextField::create("3000", "Microsoft YaHei UI", 30);
	portInput->setPosition(Vec2(visibleSize.width / 2 + 75.0f, visibleSize.height / 2));
	portInput->setEnabled(true);
	portInput->setText("3000");
	this->addChild(portInput);

	auto menu = Menu::create();
	menu->setPosition(visibleSize.width / 2, 150.0f);
	auto connectButton = MenuItemFont::create("Connect", CC_CALLBACK_1(StartScene::connect, this));
	menu->addChild(connectButton);
	this->addChild(menu);

	return true;
}

void StartScene::connect(Ref* sender) {
	auto host = hostInput->getString();
	auto port = portInput->getString();
	CCLOG("%s:%s", host.c_str(), port.c_str());
	// TODO: host&port validation
	GameSocket::init(host, port);
	GSocket->onConnection([=](GameSocket* socket) {
		// connection established
		Director::getInstance()->pushScene(WaitingHall::createScene());
	});
}
