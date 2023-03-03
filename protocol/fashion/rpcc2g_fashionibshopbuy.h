#ifndef __RPCC2G_FASHIONIBSHOPBUY_H__
#define __RPCC2G_FASHIONIBSHOPBUY_H__

// generate by ProtoGen at date: 2017/10/9 11:53:39

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_FASHIONIBSHOPBUY_TYPE 9349

class RpcC2G_FashionIBShopBuy : public CRpc
{
private:

	RpcC2G_FashionIBShopBuy():CRpc(RPCC2G_FASHIONIBSHOPBUY_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_FashionIBShopBuy *CreateRpc()
	{
		return new RpcC2G_FashionIBShopBuy;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FashionIBShopBuyArg &roArg, const FashionIBShopBuyRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FashionIBShopBuyArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FashionIBShopBuyArg &roArg, FashionIBShopBuyRes &roRes);
	void OnDelayReplyRpc(const FashionIBShopBuyArg &roArg, FashionIBShopBuyRes &roRes, const CUserData &roUserData);

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
	FashionIBShopBuyArg m_oArg;
	FashionIBShopBuyRes m_oRes;
};

#endif
