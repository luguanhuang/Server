#ifndef _H_Kcp_Peer_H__
#define _H_Kcp_Peer_H__

#include "ikcp.h"
#include "kcputil.h"

class CUdpNetWork;


class CKcpPeer
{
public:
	CKcpPeer();
	~CKcpPeer();

	void Init(UINT32 conv, CUdpNetWork* poNetwork);
	void UnInit();

	ikcpcb* kcp() { return m_kcp; }
	UINT32 conv() { return m_conv; }

	void OnRecv();
	void OnSended();

	bool Send(const char* pcData, int nLen);
	void Input(const char* pcData, int nLen);

	void Update(UINT32 dwCurrent);

	CUdpNetWork* GetNetwork() { return m_poNetwork; }
	sockaddr_in* GetRemoteAddr() { return &m_oRemoteAddr; }
	void SetRemoteAddr(const sockaddr_in& roAddr) { m_oRemoteAddr = roAddr; }
private:
	ikcpcb* m_kcp;
	UINT32	m_conv;
	char*	m_pRecvBuff;
	UINT32	m_dwRecvBuffLen;

	sockaddr_in		m_oRemoteAddr;
	CUdpNetWork*	m_poNetwork;
};

#endif