#ifndef __RPCC2M_GETGUILDWAGEREWARD_H__
#define __RPCC2M_GETGUILDWAGEREWARD_H__

// generate by ProtoGen at date: 2016/10/17 5:31:02

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETGUILDWAGEREWARD_TYPE 50133

class RpcC2M_GetGuildWageReward : public CRpc
{
private:

	RpcC2M_GetGuildWageReward():CRpc(RPCC2M_GETGUILDWAGEREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetGuildWageReward *CreateRpc()
	{
		return new RpcC2M_GetGuildWageReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGuildWageRewardArg &roArg, const GetGuildWageReward &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGuildWageRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGuildWageRewardArg &roArg, GetGuildWageReward &roRes);
	void OnDelayReplyRpc(const GetGuildWageRewardArg &roArg, GetGuildWageReward &roRes, const CUserData &roUserData);

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
	GetGuildWageRewardArg m_oArg;
	GetGuildWageReward m_oRes;
};

#endif
