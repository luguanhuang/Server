#ifndef __RPCC2M_GETDRAGONGUILDSHOP_H__
#define __RPCC2M_GETDRAGONGUILDSHOP_H__

// generate by ProtoGen at date: 2017/9/12 18:31:20

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETDRAGONGUILDSHOP_TYPE 6075

class RpcC2M_GetDragonGuildShop : public CRpc
{
private:

	RpcC2M_GetDragonGuildShop():CRpc(RPCC2M_GETDRAGONGUILDSHOP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetDragonGuildShop *CreateRpc()
	{
		return new RpcC2M_GetDragonGuildShop;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetDragonGuildShopArg &roArg, const GetDragonGuildShopRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetDragonGuildShopArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetDragonGuildShopArg &roArg, GetDragonGuildShopRes &roRes);
	void OnDelayReplyRpc(const GetDragonGuildShopArg &roArg, GetDragonGuildShopRes &roRes, const CUserData &roUserData);

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
	GetDragonGuildShopArg m_oArg;
	GetDragonGuildShopRes m_oRes;
};

#endif
