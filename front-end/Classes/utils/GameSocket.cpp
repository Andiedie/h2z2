#include "cocos2d.h"
#include "GameSocket.h"

USING_NS_CC;
#define SERVER_URL "ws://localhost:3000"

GameSocket* GameSocket::instance = nullptr;

GameSocket* GameSocket::getInstance() {
	if (!instance) instance = new GameSocket();
	return instance;
}

GameSocket::GameSocket() {
	socket = new WebSocket();
	socket->init(*this, SERVER_URL);
}

void GameSocket::onOpen(WebSocket* ws) {
	CCLOG("GameSocket: open");
	if (connectionCallback) connectionCallback(this);
}

void GameSocket::onError(WebSocket* ws, const WebSocket::ErrorCode &code) {
	CCLOG("GameSocket: error, code %d", code);
}

void GameSocket::onMessage(WebSocket* ws, const WebSocket::Data &data) {
	CCLOG("GameSocket: message %s", data.bytes);
	Document dom;
	dom.Parse(data.bytes);
	std::string e = dom.HasMember("event") ? dom["event"].GetString() : "message";
	auto it = eventPool.find(e);
	if (it != eventPool.end()) it->second.operator()(this, dom);
}

void GameSocket::onClose(WebSocket* ws) {
	CCLOG("GameSocket: close");
}

void GameSocket::emit(const std::string& eventName, Document& dom) {
	Document toSend;
	toSend.SetObject();
	auto& allocator = toSend.GetAllocator();
	toSend.AddMember("event", rapidjson::Value().SetString(eventName.c_str(), allocator), allocator);
	toSend.AddMember("data", dom, allocator);
	std::string str = stringifyDom(toSend);
	CCLOG("stringified: %s", str.c_str());
	socket->send(str);
}

void GameSocket::onConnection(std::function<void(GameSocket*)> fn) {
	connectionCallback = fn;
}

void GameSocket::on(const std::string& eventName, std::function<void(GameSocket*, Document&)> fn) {
	// not replacing
	eventPool.insert(std::make_pair(eventName, fn));
}

std::string stringifyDom(const Document& dom) {
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	dom.Accept(writer);
	return buffer.GetString();
}