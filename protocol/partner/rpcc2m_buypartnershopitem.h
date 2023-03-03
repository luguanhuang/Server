#ifndef __RPCC2M_BUYPARTNERSHOPITEM_H__
#define __RPCC2M_BUYPARTNERSHOPITEM_H__

// generate by ProtoGen at date: 2016/12/15 16:10:25

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_BUYPARTNERSHOPITEM_TYPE 14493

class RpcC2M_BuyPartnerShopItem : public CRpc
{
private:

	RpcC2M_BuyPartnerShopItem():CRpc(RPCC2M_BUYPARTNERSHOPITEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_BuyPartnerShopItem *CreateRpc()
	{
		return new RpcC2M_BuyPartnerShopItem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BuyPartnerShopItemArg &roArg, const BuyPartnerShopItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BuyPartnerShopItemArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BuyPartnerShopItemArg &roArg, BuyPartnerShopItemRes &roRes);
	void OnDelayReplyRpc(const BuyPartnerShopItemArg &roArg, BuyPartnerShopItemRes &roRes, const CUserData &roUserData);

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
	BuyPartnerShopItemArg m_oArg;
	BuyPartnerShopItemRes m_oRes;
};

#endif
