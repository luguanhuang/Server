#ifndef __RPCC2G_TAKEPARTNERCHEST_H__
#define __RPCC2G_TAKEPARTNERCHEST_H__

// generate by ProtoGen at date: 2016/12/22 9:47:58

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_TAKEPARTNERCHEST_TYPE 42982

class RpcC2G_TakePartnerChest : public CRpc
{
private:

	RpcC2G_TakePartnerChest():CRpc(RPCC2G_TAKEPARTNERCHEST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_TakePartnerChest *CreateRpc()
	{
		return new RpcC2G_TakePartnerChest;
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
