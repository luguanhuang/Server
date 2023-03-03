#ifndef __RPCC2G_QUERYSHOPITEM_H__
#define __RPCC2G_QUERYSHOPITEM_H__

// generate by ProtoGen at date: 2015/7/13 17:19:11

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_QUERYSHOPITEM_TYPE 18079

class RpcC2G_QueryShopItem : public CRpc
{
private:

	RpcC2G_QueryShopItem():CRpc(RPCC2G_QUERYSHOPITEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_QueryShopItem *CreateRpc()
	{
		return new RpcC2G_QueryShopItem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QueryShopItemArg &roArg, const QueryShopItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QueryShopItemArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QueryShopItemArg &roArg, QueryShopItemRes &roRes);
	void OnDelayReplyRpc(const QueryShopItemArg &roArg, QueryShopItemRes &roRes, const CUserData &roUserData);

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
	QueryShopItemArg m_oArg;
	QueryShopItemRes m_oRes;
};

#endif
