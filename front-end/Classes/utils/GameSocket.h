#pragma once

#include "network\WebSocket.h"

using namespace cocos2d::network;

class GameSocket : public WebSocket::Delegate {
	static GameSocket* instance;
	WebSocket* socket;
	GameSocket();
	int f() {
		CCLOG("f");
	}

	void onOpen(WebSocket* ws) override;
	void onMessage(WebSocket* ws, const WebSocket::Data &data);
	void onClose(WebSocket* ws) override;
	void onError(WebSocket* ws, const WebSocket::ErrorCode &data);
public:
	static GameSocket* getInstance();
};

#define GSocket GameSocket::getInstance()