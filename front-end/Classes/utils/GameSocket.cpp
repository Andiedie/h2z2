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
	if (connectionCallback) connectionCallback();
}

void GameSocket::onError(WebSocket* ws, const WebSocket::ErrorCode &code) {
	CCLOG("GameSocket: error, code %d", code);
}

void GameSocket::onMessage(WebSocket* ws, const WebSocket::Data &data) {
	CCLOG("GameSocket: message %s", data.bytes);
}

void GameSocket::onClose(WebSocket* ws) {
	CCLOG("GameSocket: close");
}

void GameSocket::emit(const std::string& eventName, Document& dom) {
	Document toSend;
	toSend.SetObject();
	auto& allocator = toSend.GetAllocator();
	toSend.AddMember("event", rapidjson::Value().SetString(eventName.c_str(), allocator), allocator);
	toSend.AddMember("data", dom, toSend.GetAllocator());
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	toSend.Accept(writer);
	CCLOG(buffer.GetString());
	socket->send(buffer.GetString());
}

void GameSocket::onConnection(std::function<void()> fn) {
	connectionCallback = fn;
}