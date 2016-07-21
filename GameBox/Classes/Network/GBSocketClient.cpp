#include "GBSocketClient.h"

GBSocketClient::GBSocketClient(void) :
	onRecv(nullptr),
	_socketServer(0),
	_status(DISCONNECT)
{
	
}

GBSocketClient::~GBSocketClient(void)
{
	clear();
}

void GBSocketClient::clear()
{
	if (_socketClient != 0)
	{
		_mutex.lock();
		this->closeConnect(_socketClient);
		_mutex.unlock();
	}

	for (auto msg : _messageQueue)
	{
		CC_SAFE_DELETE(msg);
	}
	_messageQueue.clear();

	Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
}

bool GBSocketClient::initClient()
{
	this->clear();

	_socketClient = socket(AF_INET, SOCK_STREAM, 0);
	if (error(_socketServer))
	{
		log("init client error!");
		_socketClient = 0;
		return false;
	}

	struct timeval timeout = { 3, 0 };
	setsockopt(_socketClient, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));

	Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
	
	return true;
}

bool GBSocketClient::connectServer(const char* serverIP, unsigned short port)
{
	if (!this->initClient())
	{
		return false;
	}

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(struct sockaddr_in));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(serverIP);

	int ret = 0;
	ret = connect(_socketClient, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr));
	if (ret < 0)
	{
		_socketClient = 0;
		return false;
	}

	std::lock_guard<std::mutex> lk(_messageQueueMutex);
	GBSocketMessage * msg = new GBSocketMessage(NEW_CONNECTION);
	_messageQueue.push_back(msg);

	std::thread netcheckThread(&GBSocketClient::netCheck, this);
	netcheckThread.detach();

	std::thread recvThread(&GBSocketClient::recvMessage, this);
	recvThread.detach();

	return true;
}

void GBSocketClient::disconnectServer()
{
	if (_status == DISCONNECT)
		return;

	_mutex.lock();
	this->closeConnect(_socketClient);

	std::lock_guard<std::mutex> lk(_messageQueueMutex);
	GBSocketMessage * msg = new GBSocketMessage(DISCONNECT);
	_messageQueue.push_back(msg);

	_socketClient = 0;
	_mutex.unlock();
}

void GBSocketClient::recvMessage()
{
	char recvBuf[4096];
	ssize_t ret = 0;
	while (true)
	{		
		ret = recv(_socketClient, recvBuf, sizeof(recvBuf), 0);
		if (ret < 0)
		{
			log("recv error!");
			break;
		}
		else if (ret > 0)
		{
			std::lock_guard<std::mutex> lk(_messageQueueMutex);
			GBSocketMessage * msg = new GBSocketMessage(RECEIVE, (unsigned char*)recvBuf, (int)ret);
			_messageQueue.push_back(msg);
		}
		else
		{
			log("socket close");
			break;
		}
	}
	disconnectServer();
}

void GBSocketClient::netCheck()
{
	while (true)
	{
		if (!netCheckClientInConn(_socketClient))
			break;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		Sleep(1000);
#else
		sleep(1000);
#endif
	}
	disconnectServer();
}

bool GBSocketClient::sendMessage(const char* data, int count)
{
	if (_socketClient != 0)
	{		
		ssize_t ret = send(_socketClient, data, count, 0);
		if (ret < 0)
		{
			log("send error!");
			return false;
		}
		return true;
	}

	return false;
}

void GBSocketClient::update(float dt)
{
	if (_messageQueue.size() == 0)
	{
		return;
	}

	_messageQueueMutex.lock();

	if (_messageQueue.size() == 0)
	{
		_messageQueueMutex.unlock();
		return;
	}

	GBSocketMessage *msg = *(_messageQueue.begin());
	_messageQueue.pop_front();

	switch (msg->getMsgType())
	{
	case NEW_CONNECTION:
		break;
	case DISCONNECT:
		if (onDisconnect && _status != DISCONNECT)
		{
			this->onDisconnect();
		}		
		break;
	case RECEIVE:
		if (onRecv)
		{
			this->onRecv((const char*)msg->getMsgData()->getBytes(), (int)msg->getMsgData()->getSize());
		}
		break;
	default:
		break;
	}
	_status = msg->getMsgType();

	CC_SAFE_DELETE(msg);
	_messageQueueMutex.unlock();
}

int32_t GBSocketClient::swapInt32(int32_t value)
{
	return ((value & 0x000000FF) << 24) |
		((value & 0x0000FF00) << 8) |
		((value & 0x00FF0000) >> 8) |
		((value & 0xFF000000) >> 24);
}