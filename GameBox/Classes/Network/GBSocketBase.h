#ifndef __GB_SOCKET_BASE_H__
#define __GB_SOCKET_BASE_H__

#include "cocos2d.h"
#include <list>
#include <thread>

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#define HSocket SOCKET

#else

#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>		// for inet_**
#include <netdb.h>			// for gethost**
#include <netinet/in.h>		// for sockaddr_in
#include <netinet/tcp.h>
#include <sys/types.h>		// for socket
#include <sys/socket.h>		// for socket
#include <unistd.h>
#include <stdio.h>		    // for printf
#include <stdlib.h>			// for exit
#include <string.h>			// for bzero
#define HSocket int

#endif

enum GBMessageType
{
	DISCONNECT,
	RECEIVE,
	NEW_CONNECTION
};

class GBSocketMessage
{
private:
	GBMessageType msgType;
	Data* msgData;

public:
	GBSocketMessage(GBMessageType type, unsigned char * pData, int len)
	{
		msgType = type;
		msgData = new Data();
		msgData->copy(pData, len);
	}

	GBSocketMessage(GBMessageType type)
	{
		msgType = type;
		msgData = nullptr;
	}

	Data* getMsgData() { return msgData; }
	GBMessageType getMsgType() { return msgType; }

	~GBSocketMessage()
	{
		CC_SAFE_DELETE(msgData);			
	}
};

class GBSocketBase
{
public:
	GBSocketBase();
	~GBSocketBase();

	bool nonBlock(HSocket socket);

protected:
	bool netCheckClientInConn(HSocket socket);
	void closeConnect(HSocket socket);
	bool error(HSocket socket);
	
protected:
	std::mutex _mutex;

private:
	bool _bInitSuccess;
};

#endif