#include "cocos2d.h"
#include "GameSocket.h"

USING_NS_CC;
#define SERVER_URL "ws://localhost:3000"

GameSocket* GameSocket::instance = nullptr;

GameSocket* GameSocket::getInstance() {
	return instance;
}

GameSocket::GameSocket() {
	socket = new WebSocket();
	socket->init(*this, SERVER_URL);
}

void GameSocket::init(std::string host, std::string port) {
	instance = new GameSocket(host, port);
}

GameSocket::GameSocket(std::string host, std::string port) {
	char buffer[50];
	sprintf(buffer, "ws://%s:%s", host.c_str(), port.c_str());
	socket = new WebSocket();
	socket->init(*this, buffer);
};

void GameSocket::onOpen(WebSocket* ws) {
	//CCLOG("GameSocket: open");
	if (connectionCallback) connectionCallback(this);
}

void GameSocket::onError(WebSocket* ws, const WebSocket::ErrorCode &code) {
	//CCLOG("GameSocket: error, code %d", code);
}

void GameSocket::onMessage(WebSocket* ws, const WebSocket::Data &data) {
	// CCLOG("GameSocket: message %s", data.bytes);
	Document dom;
	dom.Parse(data.bytes);
	std::string e = dom.HasMember("event") ? dom["event"].GetString() : "message";
	auto it = eventPool.find(e);
	if (it != eventPool.end()) {
		if (dom.HasMember("data")) it->second.operator()(this, dom["data"]);
		else it->second.operator()(this, Document());
	}
}

void GameSocket::onClose(WebSocket* ws) {
	//CCLOG("GameSocket: close");
}

void GameSocket::sendEvent(const std::string& eventName, GenericValue<UTF8<>>& dom) {
	Document toSend;
	toSend.SetObject();
	auto& allocator = toSend.GetAllocator();
	toSend.AddMember("event", rapidjson::Value().SetString(eventName.c_str(), allocator), allocator);
	toSend.AddMember("data", dom, allocator);
	std::string str = stringifyDom(toSend);
	// CCLOG("stringified: %s", str.c_str());
	socket->send(str);
}

void GameSocket::sendEvent(const std::string& eventName) {
	Document toSend;
	toSend.SetObject();
	auto& allocator = toSend.GetAllocator();
	toSend.AddMember("event", rapidjson::Value().SetString(eventName.c_str(), allocator), allocator);
	std::string str = stringifyDom(toSend);
	// CCLOG("stringified: %s", str.c_str());
	socket->send(str);
}

void GameSocket::onConnection(std::function<void(GameSocket*)> fn) {
	connectionCallback = fn;
}

void GameSocket::on(const std::string& eventName, std::function<void(GameSocket*, GenericValue<UTF8<>>&)> fn) {
	// with replacing
	auto it = eventPool.find(eventName);
	if (it != eventPool.end()) it->second = fn;
	else eventPool.insert(std::make_pair(eventName, fn));
}

void GameSocket::removeEventHandler(const std::string& eventName) {
	eventPool.erase(eventName);
}

std::string GameSocket::stringifyDom(const Document& dom) {
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	dom.Accept(writer);
	return buffer.GetString();
}