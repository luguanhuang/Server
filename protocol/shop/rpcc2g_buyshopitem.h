#ifndef __RPCC2G_BUYSHOPITEM_H__
#define __RPCC2G_BUYSHOPITEM_H__

// generate by ProtoGen at date: 2015/7/13 17:39:40

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BUYSHOPITEM_TYPE 33881

class RpcC2G_BuyShopItem : public CRpc
{
private:

	RpcC2G_BuyShopItem():CRpc(RPCC2G_BUYSHOPITEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_BuyShopItem *CreateRpc()
	{
		return new RpcC2G_BuyShopItem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BuyShopItemArg &roArg, const BuyShopItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BuyShopItemArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BuyShopItemArg &roArg, BuyShopItemRes &roRes);
	void OnDelayReplyRpc(const BuyShopItemArg &roArg, BuyShopItemRes &roRes, const CUserData &roUserData);

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
	BuyShopItemArg m_oArg;
	BuyShopItemRes m_oRes;
};

#endif
