#ifndef __RPCC2M_BUYDRAGONGUILDSHOPITEM_H__
#define __RPCC2M_BUYDRAGONGUILDSHOPITEM_H__

// generate by ProtoGen at date: 2017/9/12 18:30:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_BUYDRAGONGUILDSHOPITEM_TYPE 24893

class RpcC2M_BuyDragonGuildShopItem : public CRpc
{
private:

	RpcC2M_BuyDragonGuildShopItem():CRpc(RPCC2M_BUYDRAGONGUILDSHOPITEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_BuyDragonGuildShopItem *CreateRpc()
	{
		return new RpcC2M_BuyDragonGuildShopItem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BuyDragonGuildShopItemArg &roArg, const BuyDragonGuildShopItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BuyDragonGuildShopItemArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BuyDragonGuildShopItemArg &roArg, BuyDragonGuildShopItemRes &roRes);
	void OnDelayReplyRpc(const BuyDragonGuildShopItemArg &roArg, BuyDragonGuildShopItemRes &roRes, const CUserData &roUserData);

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
	BuyDragonGuildShopItemArg m_oArg;
	BuyDragonGuildShopItemRes m_oRes;
};

#endif
