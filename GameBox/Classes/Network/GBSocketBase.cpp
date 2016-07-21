#include "GBSocketBase.h"

GBSocketBase::GBSocketBase()
{
	_bInitSuccess = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 0);
	WSADATA wsaData;
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if (nRet != 0) 
	{
		fprintf(stderr, "Initilize Error!\n");
		return;
	}
	_bInitSuccess = true;
#endif

}

GBSocketBase::~GBSocketBase()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (_bInitSuccess)
	{
		WSACleanup();
	}
#endif 
}

void GBSocketBase::closeConnect(HSocket socket)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	closesocket(socket);
#else
    close(socket);
#endif
}

bool GBSocketBase::error(HSocket socket)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return socket == SOCKET_ERROR;
#else
	return socket < 0;
#endif
}

bool GBSocketBase::nonBlock(HSocket socket)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	u_long ulOn;
	ulOn = 1;
	if (ioctlsocket(socket, FIONBIO, &ulOn) == SOCKET_ERROR)
	{
		return false;
	}
#else
    int flags;
    flags = fcntl(socket, F_GETFL, 0);
    if (fcntl(socket, F_SETFL, flags) < 0)
    {
        return false;
    }
#endif

	return true;
}

bool GBSocketBase::netCheckClientInConn(HSocket socket)
{
	if (socket == 0)
		return false;
    
	int optval, optlen;
	optlen = sizeof(int);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    int ret = getsockopt(socket, SOL_SOCKET, SO_ERROR, (void*)&optval, (socklen_t *)&optlen);
#else
	int ret = getsockopt(socket, SOL_SOCKET, SO_ERROR, (char*)&optval, &optlen);
#endif
	return (!ret && !optval);
}