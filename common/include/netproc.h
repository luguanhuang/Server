#ifndef _Net_Processor_H__
#define _Net_Processor_H__

#include "coder.h"
#include "netprocess.h"
#include "singleton.h"
#include <unordered_map>


class INetWork;
class CProtocol;

enum ProtocolCountType
{
	PCT_SEND,
	PCT_RECV,
	PCT_FORWARD,
};


class BaseLink;

class CNetProcessor : public INetWorkProcess
{
	CNetProcessor();
	~CNetProcessor(); 

	DECLARE_SINGLETON(CNetProcessor) 
public: 
	void Init(INetWork* poNetWork);
	void UnInit();


	//Send Protocol
	bool Send(UINT32 dwConnID, const CProtocol& roPtc);
	bool Send(UINT32 dwConnID, CRpc &roRpc);
	bool Send(UINT32 dwConnID, const void* pData, UINT32 dwLen);
	bool GetRemoteAddr(UINT32 dwConnID, UINT32& rdwIP, UINT16& rwPort);
	void Close(UINT32 dwConnID);
	void ShowNetWorkState();

public:
	virtual void OnConnectSucceed(UINT32 dwConnID, void* pUserData);
	virtual void OnConnectFailed(INT32 nNetErrCode, INT32 nSysErrCode, void* pUserData);

	virtual void OnPassiveConnect(UINT32 dwConnID, void* pUserData); 
	virtual void OnClose(UINT32 dwConnID, INT32 nNetErrCode, INT32 nSysErrCode, void* pUserData); 

	virtual void OnError(INT32 nNetErrCode, INT32 nSysErrCode); 
	virtual void OnRecv(UINT32 dwConnID, const char* pcData, UINT32 dwLen, void* pUserData); 

private:

	std::unordered_map<UINT32, BaseLink*> m_Links;
	INetWork*		m_poNetwork;
};

#endif
