#include "pch.h"
#include "serverstateinfo.h"
#include "threads.h"
#include "session/sessionmanager.h"
#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#else
typedef int socklen_t;
#endif
#include "config.h"

INSTANCE_SINGLETON(ServerStateInfo)

ServerStateInfo::ServerStateInfo()
{

}

ServerStateInfo::~ServerStateInfo()
{

}

/*
#define BUFSIZE 128

enum ServerState
{
	EMPTY = 0,
	NORMAL = 1,
	FULL = 2,
};

enum ServerFlag
{
	NONE = 0,
	NEW  = 1,
};


class UDPServerStateThread : public Thread
{
public:
	int m_socket;

	virtual void Run()
	{
		const QueryInfo &qInfo = GateConfig::Instance()->GetQueryInfo();

		int flag = 1;
		int ret = 0;
		struct sockaddr_in addr;
		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr("0.0.0.0");
		addr.sin_port = htons(qInfo.port);
		ret = setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, sizeof(flag));
		ret = bind(m_socket, (struct sockaddr *)&addr, sizeof(addr));

		struct sockaddr_in remaddr;
		int addrlen = sizeof(remaddr);
		char buf[BUFSIZE];
		while (!m_isCancel)
		{
			int recvlen = recvfrom(m_socket, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, (socklen_t*)&addrlen);
			if (recvlen == 4)
			{
				buf[4] = GetServerState();
				buf[5] = GetServerFlag();
				sendto(m_socket, buf, 6, 0, (struct sockaddr*)&remaddr, addrlen);
			}
		}

#ifndef WIN32
		close(m_socket);
#else
		closesocket(m_socket);
#endif
	}

	ServerState GetServerState()
	{
		const QueryInfo &qInfo = GateConfig::Instance()->GetQueryInfo();
		int Count = SessionManager::Instance()->GetSessionCount();
		if (Count <= qInfo.normal)
		{
			return EMPTY;
		}
		else if (Count <= qInfo.full)
		{
			return NORMAL;
		}
		else
		{
			return FULL;
		}

		return EMPTY;
	}

	ServerFlag GetServerFlag()
	{
		const QueryInfo &qInfo = GateConfig::Instance()->GetQueryInfo();
		if (qInfo.isNew)
		{
			return NEW;
		}

		return NONE;
	}

};
*/

//static UDPServerStateThread udp_thread;

bool ServerStateInfo::Init()
{
	//udp_thread.Start();
	return true;
}

void ServerStateInfo::Uninit()
{
	//udp_thread.Cancel();
}
