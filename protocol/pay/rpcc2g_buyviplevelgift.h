#ifndef __RPCC2G_BUYVIPLEVELGIFT_H__
#define __RPCC2G_BUYVIPLEVELGIFT_H__

// generate by ProtoGen at date: 2016/8/22 22:36:00

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BUYVIPLEVELGIFT_TYPE 52536

class RpcC2G_BuyVipLevelGift : public CRpc
{
private:

	RpcC2G_BuyVipLevelGift():CRpc(RPCC2G_BUYVIPLEVELGIFT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_BuyVipLevelGift *CreateRpc()
	{
		return new RpcC2G_BuyVipLevelGift;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BuyVipLevelGiftArg &roArg, const BuyVipLevelGiftRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BuyVipLevelGiftArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BuyVipLevelGiftArg &roArg, BuyVipLevelGiftRes &roRes);
	void OnDelayReplyRpc(const BuyVipLevelGiftArg &roArg, BuyVipLevelGiftRes &roRes, const CUserData &roUserData);

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
	BuyVipLevelGiftArg m_oArg;
	BuyVipLevelGiftRes m_oRes;
};

#endif
