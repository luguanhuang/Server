#ifndef __RPCC2G_GETNEXTDAYREWARD_H__
#define __RPCC2G_GETNEXTDAYREWARD_H__

// generate by ProtoGen at date: 2015/10/20 18:39:10

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETNEXTDAYREWARD_TYPE 40997

class RpcC2G_GetNextDayReward : public CRpc
{
private:

	RpcC2G_GetNextDayReward():CRpc(RPCC2G_GETNEXTDAYREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetNextDayReward *CreateRpc()
	{
		return new RpcC2G_GetNextDayReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetNextDayRewardArg &roArg, const GetNextDayRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetNextDayRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetNextDayRewardArg &roArg, GetNextDayRewardRes &roRes);
	void OnDelayReplyRpc(const GetNextDayRewardArg &roArg, GetNextDayRewardRes &roRes, const CUserData &roUserData);

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
	GetNextDayRewardArg m_oArg;
	GetNextDayRewardRes m_oRes;
};

#endif
