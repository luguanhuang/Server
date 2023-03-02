#ifndef __GSLINK_H__
#define __GSLINK_H__

#include "linelink.h"
#include "pb/enum.pb.h"
#include "version/version.h"


class CProtocol;

class GSLink : public LineLink, public ForwardInterface
{
	GSLink();
	~GSLink();
	DECLARE_SINGLETON(GSLink)
public:

	virtual void OnPassiveConnect(UINT32 dwConnID);
	virtual void OnClose(UINT32 dwConnID, int err);
	virtual const std::string &GetName();

	virtual UINT32 GetForwardConnID(UINT32 dwConnID, const ProtocolHead &head);
	virtual UINT32 GetForwardConnID(UINT32 dwConnID, const RpcHead &head);

	virtual bool IsChangeHead() { return true; }
	virtual void GetNewHeadInfo(const ProtocolHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData);
	virtual void GetNewHeadInfo(const RpcHead& srcHead, const char* pcData, UINT32 dwDataLen, HeadInfo& newHead, DataBlob& newData);


	///new
	virtual void OnLineConnected(UINT32 dwLine);
	virtual void OnLineClosed(UINT32 dwLine);
	KKSG::ErrorCode AddGameServer(UINT32 dwConnID, UINT32 dwServerID, UINT32 dwLine);

private:
	SessionType GetGateServerMask();
	SessionType m_GateServerMask;

private:
	ProtocolHead PtcChangedHead;
	RpcHead RpcChangedHead;
};

#endif