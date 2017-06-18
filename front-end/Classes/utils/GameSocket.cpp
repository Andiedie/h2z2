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
