#ifndef _H_Net_Util_H__
#define _H_Net_Util_H__

#ifdef WIN32
#include <Windows.h>
#else
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h>
#endif

#include <string>


class NetUtil
{
public:
	static std::string GetLocalHostIP()
	{
#ifdef WIN32
		char temp[256];
		if(gethostname(temp, 256) == SOCKET_ERROR)
		{
			return "";
		}

		hostent* host = gethostbyname(temp);
		if(host == NULL)
		{
			return "";
		}

		return inet_ntoa(*((in_addr*)(host->h_addr)));
#else
		struct ifaddrs* pIfAddr = NULL;
		getifaddrs(&pIfAddr);

		while(pIfAddr != NULL)
		{
			if(pIfAddr->ifa_addr->sa_family == AF_INET)
			{
				char buffer[INET_ADDRSTRLEN];
				void* ptr = &((sockaddr_in*)pIfAddr->ifa_addr)->sin_addr;
				inet_ntop(AF_INET, ptr, buffer, INET_ADDRSTRLEN);
				if(strcmp(buffer, "127.0.0.1") != 0)
				{
					return std::string(buffer);
				}
			}
			pIfAddr=pIfAddr->ifa_next;
		}
		return "";
#endif
	}
};

#endif
