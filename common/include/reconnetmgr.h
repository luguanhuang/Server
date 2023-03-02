#ifndef __RECONNECT_MGR_H__
#define __RECONNECT_MGR_H__

#include "define.h"
#include "singleton.h"
#include <string>
#include <list>


enum NetAction 
{
	NA_NONE,
	NA_LISTEN,
	NA_CONNECT,
};

enum ConnectState
{
	PS_NONE,
	PS_CONNECTING,
	PS_CONNECT_OK,
	PS_CONNECT_FAILED,
};

struct PeerInfo
{
	PeerInfo()
	{
		m_action = NA_NONE;
		m_state = PS_NONE;
		m_dwSendBuffSize = 0;
		m_dwRecvBuffSize = 0;
	}

	NetAction   m_action;    
	ConnectState m_state;  // 处理connect断线后的重连
	IPAddress   m_address;
	std::string m_handler;
	UINT32		m_dwSendBuffSize;
	UINT32		m_dwRecvBuffSize;

	bool operator == (const PeerInfo& other) const
	{
		if (m_action != other.m_action)
		{
			return false;
		}
		if (m_address.m_ip != other.m_address.m_ip)
		{
			return false;
		}
		if (m_address.m_port != other.m_address.m_port)
		{
			return false;
		}
		return true;
	}
};

class INetWork;
class BaseLink;
class IPacketCoder;

class ReconnectManager
{
	ReconnectManager();
	~ReconnectManager(); 

	DECLARE_SINGLETON(ReconnectManager) 

public:
	bool Init();
	void UnInit();

	bool InitNetwork(INetWork *network, IPacketCoder *coder);
	void SetLinkState(BaseLink *link, PeerInfo& peerInfo, ConnectState newState);
	void CheckLinkState();
	void AddPeerInfo(PeerInfo &info);
	void RemovePeerInfo(PeerInfo& info);

private:
	std::list<PeerInfo>  m_PeersInfo;
	INetWork            *m_Network;
	IPacketCoder        *m_PacketCoder;
};


#endif