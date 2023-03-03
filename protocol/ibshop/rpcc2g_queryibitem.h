#ifndef __RPCC2G_QUERYIBITEM_H__
#define __RPCC2G_QUERYIBITEM_H__

// generate by ProtoGen at date: 2016/9/1 16:58:01

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_QUERYIBITEM_TYPE 23880

class RpcC2G_QueryIBItem : public CRpc
{
private:

	RpcC2G_QueryIBItem():CRpc(RPCC2G_QUERYIBITEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_QueryIBItem *CreateRpc()
	{
		return new RpcC2G_QueryIBItem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const IBQueryItemReq &roArg, const IBQueryItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const IBQueryItemReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const IBQueryItemReq &roArg, IBQueryItemRes &roRes);
	void OnDelayReplyRpc(const IBQueryItemReq &roArg, IBQueryItemRes &roRes, const CUserData &roUserData);

	virtual void OnReplyTimeout() 
	{ 
		OnTimeout(m_oArg, m_oUser); 
	}

	virtual void OnDelayRpc(const CUserData &userData)
	{
		OnDelayReplyRpc( m_oArg, m_oRes, userData);
	}

	virtual CRpc *NewRpc() 
	{
		return CreateRpc();
	}

public:
	IBQueryItemReq m_oArg;
	IBQueryItemRes m_oRes;
};

#endif
