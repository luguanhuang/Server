#ifndef __RPCC2G_BUYIBITEM_H__
#define __RPCC2G_BUYIBITEM_H__

// generate by ProtoGen at date: 2016/9/1 15:44:55

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BUYIBITEM_TYPE 11547

class RpcC2G_BuyIBItem : public CRpc
{
private:

	RpcC2G_BuyIBItem():CRpc(RPCC2G_BUYIBITEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_BuyIBItem *CreateRpc()
	{
		return new RpcC2G_BuyIBItem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const IBBuyItemReq &roArg, const IBBuyItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const IBBuyItemReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const IBBuyItemReq &roArg, IBBuyItemRes &roRes);
	void OnDelayReplyRpc(const IBBuyItemReq &roArg, IBBuyItemRes &roRes, const CUserData &roUserData);

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
	IBBuyItemReq m_oArg;
	IBBuyItemRes m_oRes;
};

#endif
