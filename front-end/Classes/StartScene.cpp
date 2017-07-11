#include "StartScene.h"
#include "WaitingHall.h"
#define db UserDefault::getInstance()

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

	auto title = Label::create("H2Z2", "Arial", 96);
	title->setPosition(visibleSize.width / 2, visibleSize.height - 70.0f);
	this->addChild(title);

	auto hostLabel = Label::create("Host", "Arial", 28);
	hostLabel->setPosition(visibleSize.width / 2 - 75.0f, visibleSize.height / 2 + 30.0f);
	this->addChild(hostLabel);

	hostInput = ui::TextField::create("127.0.0.1", "Arial", 30.0f);
	hostInput->setPosition(Vec2(visibleSize.width / 2 - 75.0f, visibleSize.height / 2));
	hostInput->setEnabled(true);
	auto str = db->getStringForKey("host");
	if (str == "") str = "127.0.0.1";
	hostInput->setText(str);
	this->addChild(hostInput);

	auto portLabel = Label::create("Port", "Arial", 28);
	portLabel->setPosition(visibleSize.width / 2 + 75.0f, visibleSize.height / 2 + 30.0f);
	this->addChild(portLabel);

	portInput = ui::TextField::create("3000", "Arial", 30);
	portInput->setPosition(Vec2(visibleSize.width / 2 + 75.0f, visibleSize.height / 2));
	portInput->setEnabled(true);
	str = db->getStringForKey("port");
	if (str == "") str = "3000";
	portInput->setText(str);
	this->addChild(portInput);

	auto nameLabel = Label::create("Name: ", "Arial", 28);
	nameLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 70.0f);
	this->addChild(nameLabel);

	nameInput = ui::TextField::create("Input name here!", "Arial", 34);
	nameInput->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 130.0f));
	nameInput->setEnabled(true);
	nameInput->setColor(Color3B(204, 204, 51));
	nameInput->setText(db->getStringForKey("name"));
	this->addChild(nameInput);

	auto menu = Menu::create();
	menu->setPosition(visibleSize.width / 2, 150.0f);
	auto connectButton = MenuItemFont::create("Connect", CC_CALLBACK_1(StartScene::connect, this));
	menu->addChild(connectButton);
	this->addChild(menu);

	return true;
}

void StartScene::connect(Ref* sender) {
	auto name = nameInput->getString();
	if (name == "") return;
	auto host = hostInput->getString();
	auto port = portInput->getString();
	db->setStringForKey("name", name);
	db->setStringForKey("host", host);
	db->setStringForKey("port", port);
	//CCLOG("%s:%s", host.c_str(), port.c_str());
	// TODO: host&port validation
	GameSocket::init(host, port);
	GSocket->onConnection([=](GameSocket* socket) {
		Document dom;
		dom.SetObject();
		dom.AddMember("name", StringRef(name.c_str()), dom.GetAllocator());
		GSocket->sendEvent("name", dom);
		Director::getInstance()->pushScene(WaitingHall::createScene());
	});
}
