#ifndef __RPCC2G_TAKEDRAGONGUILDCHEST_H__
#define __RPCC2G_TAKEDRAGONGUILDCHEST_H__

// generate by ProtoGen at date: 2017/9/6 16:13:07

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_TAKEDRAGONGUILDCHEST_TYPE 38031

class RpcC2G_TakeDragonGuildChest : public CRpc
{
private:

	RpcC2G_TakeDragonGuildChest():CRpc(RPCC2G_TAKEDRAGONGUILDCHEST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_TakeDragonGuildChest *CreateRpc()
	{
		return new RpcC2G_TakeDragonGuildChest;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TakePartnerChestArg &roArg, const TakePartnerChestRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TakePartnerChestArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TakePartnerChestArg &roArg, TakePartnerChestRes &roRes);
	void OnDelayReplyRpc(const TakePartnerChestArg &roArg, TakePartnerChestRes &roRes, const CUserData &roUserData);

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
	TakePartnerChestArg m_oArg;
	TakePartnerChestRes m_oRes;
};

#endif
