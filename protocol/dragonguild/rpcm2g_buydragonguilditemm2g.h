#ifndef __RPCM2G_BUYDRAGONGUILDITEMM2G_H__
#define __RPCM2G_BUYDRAGONGUILDITEMM2G_H__

// generate by ProtoGen at date: 2017/9/12 18:33:58

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_BUYDRAGONGUILDITEMM2G_TYPE 26961

class RpcM2G_BuyDragonGuildItemM2G : public CRpc
{
private:

	RpcM2G_BuyDragonGuildItemM2G():CRpc(RPCM2G_BUYDRAGONGUILDITEMM2G_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_BuyDragonGuildItemM2G *CreateRpc()
	{
		return new RpcM2G_BuyDragonGuildItemM2G;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BuyDragonGuildShopItemM2GArg &roArg, const BuyDragonGuildShopItemM2GRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BuyDragonGuildShopItemM2GArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BuyDragonGuildShopItemM2GArg &roArg, BuyDragonGuildShopItemM2GRes &roRes);
	void OnDelayReplyRpc(const BuyDragonGuildShopItemM2GArg &roArg, BuyDragonGuildShopItemM2GRes &roRes, const CUserData &roUserData);

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
	BuyDragonGuildShopItemM2GArg m_oArg;
	BuyDragonGuildShopItemM2GRes m_oRes;
};

#endif
