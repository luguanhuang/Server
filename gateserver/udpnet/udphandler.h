#ifndef _H_Udp_Handler_H__
#define _H_Udp_Handler_H__


class CUdpDataHandler
{
public:
	CUdpDataHandler();
	~CUdpDataHandler();

	void OnRecv(UINT32 dwPeerID, const char* pcData, UINT32 dwLen);
};

#endif