#ifndef _H_Udp_Network_H__
#define _H_Udp_Network_H__

#include "kcppeer.h"
#include "udphandler.h"


class CUdpNetWork
{
public:
	CUdpNetWork();
	~CUdpNetWork();

	bool Init(const std::string& strLocalIP, int nPort, INT32 nRecvBuffSize = 1024 * 1024);
	void UnInit();

	void Update();
	bool SendTo(UINT32 dwPeerID, const char* pcData, UINT32 dwLen);

	CKcpPeer* GetPeer(UINT32 dwPeerID);
	CKcpPeer* CreatePeer(UINT32 dwPeerID);
	void	  ReleasePeer(UINT32 dwPeerID);

#ifdef WIN32
	SOCKET GetSocket() const { return m_hSocket; }
#else
	int GetSocket() const { return m_hSocket; }
#endif
	static int udp_output(const char *buf, int len, ikcpcb *kcp, void *user);

	CUdpDataHandler& GetHandler() { return m_oHandler; }
private:
	void CloseSocket();
	bool SetSocketNonBlock(INT32 nEnable);

private:
	typedef std::map<UINT32, CKcpPeer*>	CPeerMap;

#ifdef WIN32
	SOCKET		m_hSocket;
#else
	int			m_hSocket;
#endif
	CPeerMap	m_oPeers;
	CUdpDataHandler	m_oHandler;
};

#endif
