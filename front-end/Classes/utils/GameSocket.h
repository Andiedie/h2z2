#pragma once

#include "network\WebSocket.h"
#include "cocos2d\external\json\document.h"
#include "cocos2d\external\json\writer.h"
#include "cocos2d\external\json\stringbuffer.h"
#include "cocos2d\external\json\rapidjson.h"
#include <functional>
#include <map>

using namespace cocos2d::network;
using namespace rapidjson;

class GameSocket : public WebSocket::Delegate {
	static GameSocket* instance;
	WebSocket* socket;
	std::map<std::string, std::function<void(GameSocket*, Document&)>> eventPool;
	std::function<void(GameSocket*)> connectionCallback;

	GameSocket();
	void onOpen(WebSocket* ws) override;
	void onMessage(WebSocket* ws, const WebSocket::Data &data);
	void onClose(WebSocket* ws) override;
	void onError(WebSocket* ws, const WebSocket::ErrorCode &data);
public:
	static GameSocket* getInstance();

	void emit(const std::string& eventName, Document& dom);
	void on(const std::string& eventName, std::function<void(GameSocket*, Document&)> fn);
	void onConnection(std::function<void(GameSocket*)> fn);
};

std::string stringifyDom(const Document& dom);

#define GSocket GameSocket::getInstance()