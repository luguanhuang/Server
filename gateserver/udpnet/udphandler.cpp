#include "pch.h"
#include "udphandler.h"
#include "network/clientlink.h"
#include "network/gslink.h"


CUdpDataHandler::CUdpDataHandler()
{
}

CUdpDataHandler::~CUdpDataHandler()
{
}

void CUdpDataHandler::OnRecv(UINT32 dwPeerID, const char* pcData, UINT32 dwLen)
{
	CClientLink::Instance()->OnRecv(dwPeerID, pcData, dwLen);
}