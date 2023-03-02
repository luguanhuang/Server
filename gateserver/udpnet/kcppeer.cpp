#include "pch.h"
#include "kcppeer.h"
#include "udpnetwork.h"


CKcpPeer::CKcpPeer()
:m_kcp(NULL)
,m_conv(0)
,m_pRecvBuff(NULL)
,m_dwRecvBuffLen(0)
{
}

CKcpPeer::~CKcpPeer()
{
}

void CKcpPeer::Init(UINT32 conv, CUdpNetWork* poNetwork)
{
	m_kcp = ikcp_create(conv, this);
	m_kcp->output = CUdpNetWork::udp_output;
	m_conv = conv;
	m_poNetwork = poNetwork;


	m_dwRecvBuffLen = 1024 * 32;
	m_pRecvBuff = new char[m_dwRecvBuffLen];
}

void CKcpPeer::UnInit()
{
	if(m_pRecvBuff != NULL)
	{
		delete [] m_pRecvBuff;
		m_pRecvBuff = NULL;
	}
	if(m_kcp != NULL)
	{
		ikcp_release(m_kcp);
		m_kcp = NULL;
	}
}

bool CKcpPeer::Send(const char* pcData, int nLen)
{
	if(ikcp_send(kcp(), pcData, nLen) < 0)
	{
		printf("send failed to peer id %u, data size maybe too long", conv());
		return false;
	}

	return true;
}

void CKcpPeer::Input(const char* pcData, int nLen)
{
	ikcp_input(kcp(), pcData, nLen);
}

void CKcpPeer::Update(UINT32 dwCurrent)
{
	ikcp_update(kcp(), dwCurrent);

	//check recv
	int nRet = ikcp_recv(kcp(), m_pRecvBuff, (int)m_dwRecvBuffLen);
	if(nRet <= 0)
	{
		return;
	}

	//write data
	GetNetwork()->GetHandler().OnRecv(conv(), m_pRecvBuff, (UINT32)nRet);
}
