#ifndef __GB_SOCKET_CLIENT_H__
#define __GB_SOCKET_CLIENT_H__
#include "GBSocketBase.h"

class GBSocketClient : public GBSocketBase
{
public:
	bool connectServer(const char* serverIP, unsigned short port);
	void disconnectServer();
	bool sendMessage(const char* data, int count);

	std::function<void(const char* data, int count)> onRecv;
	std::function<void()> onDisconnect;

	void update(float dt);

CC_CONSTRUCTOR_ACCESS:
	GBSocketClient(void);
	~GBSocketClient(void);

	static int32_t swapInt32(int32_t value);

private:
	bool initClient();
	void recvMessage();
	void netCheck();
	void clear();

private:
	HSocket _socketServer;
	HSocket _socketClient;
	std::list<GBSocketMessage*> _messageQueue;
	std::mutex   _messageQueueMutex;
	int _status;
};

#endif