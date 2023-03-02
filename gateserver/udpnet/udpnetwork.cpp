#include "pch.h"
#include "udpnetwork.h"
#include "kcppeer.h"

#ifndef WIN32
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif


CUdpNetWork::CUdpNetWork()
{
#ifdef WIN32
		m_hSocket = INVALID_SOCKET;
#else
		m_hSocket = -1;
#endif
}

CUdpNetWork::~CUdpNetWork()
{
}

bool CUdpNetWork::Init(const std::string& strLocalIP, int nPort, INT32 nRecvBuffSize)
{
#ifdef WIN32
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		printf("Error at WSAStartup: %d\n", WSAGetLastError());
		return false;
	}
#endif

	if(nRecvBuffSize <= 0) nRecvBuffSize = 1024 * 1024;

	UINT32 dwLocalIP = inet_addr(strLocalIP.c_str());
	UINT16 wPort = htons(nPort);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = dwLocalIP;
	addr.sin_port = wPort;
#ifdef WIN32
	m_hSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(m_hSocket == INVALID_SOCKET)
	{
		return false;
	}

	if(setsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuffSize, sizeof(nRecvBuffSize)) == SOCKET_ERROR)
	{
		CloseSocket();
		return false;
	}

	if(!SetSocketNonBlock(1))
	{
		CloseSocket();
		return false;
	}

	if(::bind(m_hSocket, (sockaddr*)&addr, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		CloseSocket();
		return false;
	}
#else
	m_hSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(m_hSocket == -1)
	{
		printf("create socket error:%s\n", strerror(errno));
		return false;
	}

	socklen_t optlen = sizeof(nRecvBuffSize);
	if(setsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&nRecvBuffSize, optlen) == -1)
	{
		printf("set socket option error:%s\n", strerror(errno));
		return false;
	}

	if(!SetSocketNonBlock(1))
	{
		CloseSocket();
		return false;
	}

	if(bind(m_hSocket, (sockaddr*)&addr, sizeof(sockaddr)) == -1)
	{
		printf("socket error:%s\n", strerror(errno));
		return false;
	}
#endif

	return true;
}

void CUdpNetWork::UnInit()
{
	CloseSocket();
#ifdef WIN32
	WSACleanup();
#endif
}

void CUdpNetWork::CloseSocket()
{
#ifdef WIN32
	if(m_hSocket != INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
#else
	if(m_hSocket != -1)
	{
		close(m_hSocket);
		m_hSocket = -1;
	}
#endif
}

bool CUdpNetWork::SetSocketNonBlock(INT32 nEnable)
{
#ifdef WIN32
	u_long param = nEnable;
	return ioctlsocket(m_hSocket, FIONBIO, &param) == 0;
#else
	int32_t opt = 0;
	if((opt = fcntl(m_hSocket, F_GETFL)) < 0)
	{
		printf("fcntl socket error:%s\n", strerror(errno));
		return false;
	}

	if (nEnable)
	{
		opt |= O_NONBLOCK;
	}
	else
	{
		opt &= ~O_NONBLOCK;
	}

	if(fcntl(m_hSocket, F_SETFL, opt) < 0)
	{
		printf("fcntl socket error:%s\n", strerror(errno));
		return false;
	}
	return true;
#endif 
}


void CUdpNetWork::Update()
{
	sockaddr_in client_addr;

	static char sRecvBuff[1024 * 256];
	INT32 nBuffLen = 1024 * 256;

	while(1)
	{
#ifdef WIN32
		int len = sizeof(sockaddr_in);
		int nRet = recvfrom(m_hSocket, sRecvBuff, nBuffLen, 0, (sockaddr*)&client_addr, &len);
		if(nRet == SOCKET_ERROR)
		{
			INT32 nErrCode = WSAGetLastError();
			if(nErrCode == WSAEWOULDBLOCK) break;
			else if(nErrCode == WSAEMSGSIZE) continue;
			else break;
		}
#else
		socklen_t len = sizeof(sockaddr_in);
		int nRet = recvfrom(m_hSocket, sRecvBuff, nBuffLen, 0, (sockaddr*)&client_addr, &len);
		if(nRet == -1)
		{
			if(errno == EINTR) continue;
			else break;
		}
#endif
		else if(nRet > 0)
		{
			if(nRet < KCP_HEADER_SIZE)
			{
				printf("Error kcp packet\n");
				continue;
			}

			//get kcp conv
			//printf("recv udp data size [%u]\n", nRet);
			UINT32 conv = *(UINT32*)sRecvBuff;
			CKcpPeer* poPeer = GetPeer(conv);
			if(poPeer == NULL)
			{
				printf("invalid kcp peer %u", conv);
				continue;
			}
			
			//kcp input
			//printf("recv kcp data [ peer: %u, size: %u]\n", conv, nRet);
			poPeer->Input(sRecvBuff, nRet);
			poPeer->SetRemoteAddr(client_addr);
		}
	}

	//update all
	IUINT32 currTime = iclock();
	for(CPeerMap::iterator it = m_oPeers.begin(); it != m_oPeers.end(); ++it)
	{
		it->second->Update(currTime);
	}
}

bool CUdpNetWork::SendTo(UINT32 dwPeerID, const char* pcData, UINT32 dwLen)
{
	if(pcData == NULL || dwLen > MAX_UDP_PACKET_SIZE)
	{
		return false;
	}

	CKcpPeer* poPeer = GetPeer(dwPeerID);
	if(poPeer == NULL)
	{
		printf("invalid peer id %u", dwPeerID);
		return false;
	}

	return poPeer->Send(pcData, (int)dwLen);
}

CKcpPeer* CUdpNetWork::GetPeer(UINT32 dwPeerID)
{
	CPeerMap::iterator it = m_oPeers.find(dwPeerID);
	return it == m_oPeers.end() ? NULL : it->second;
}

CKcpPeer* CUdpNetWork::CreatePeer(UINT32 dwPeerID)
{
	if(GetPeer(dwPeerID) != NULL)
	{
		return NULL;
	}
  
	CKcpPeer* poPeer = new CKcpPeer();
	poPeer->Init(dwPeerID, this);
	m_oPeers.insert(std::make_pair(dwPeerID, poPeer));
	return poPeer;
}

void CUdpNetWork::ReleasePeer(UINT32 dwPeerID)
{
	CPeerMap::iterator it = m_oPeers.find(dwPeerID);
	if(it == m_oPeers.end())
	{
		return;
	}

	it->second->UnInit();
	delete it->second;
	m_oPeers.erase(it);
}

int CUdpNetWork::udp_output(const char *buf, int len, ikcpcb *kcp, void *user)
{
#ifdef WIN32
	CKcpPeer* poPeer = (CKcpPeer*)user;
	sendto(poPeer->GetNetwork()->GetSocket(), buf, len, 0, (sockaddr*)poPeer->GetRemoteAddr(), sizeof(sockaddr));
#else
	CKcpPeer* poPeer = (CKcpPeer*)user;
	sendto(poPeer->GetNetwork()->GetSocket(), buf, len, 0, (sockaddr*)poPeer->GetRemoteAddr(), sizeof(sockaddr));
#endif
	/*while (send_len < len)
	{
		//真实发送数据
		int nRet = sendto(,buf + send_len,	len - send_len,	0,
			(struct sockaddr*) &(ctx->dst_addr),sizeof(ctx->dst_addr));
		if (res <= 0)
		{			
			if (errno == EINTR){//中断继续写			
				continue;
			} 
			else if (errno == EAGAIN){ // EAGAIN : Resource temporarily unavailable
				continue;
			}
			else{		
				printf("socket send error:%s\n", strerror(errno));
				return -1;
			}			
		}

		send_len += res;
	}*/

	return 0;
}
