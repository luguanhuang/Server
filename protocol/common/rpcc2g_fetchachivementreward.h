﻿#ifndef __RPCC2G_FETCHACHIVEMENTREWARD_H__
#define __RPCC2G_FETCHACHIVEMENTREWARD_H__

// generate by ProtoGen at date: 2016/11/16 19:30:01

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_FETCHACHIVEMENTREWARD_TYPE 47094

class RpcC2G_FetchAchivementReward : public CRpc
{
private:

	RpcC2G_FetchAchivementReward():CRpc(RPCC2G_FETCHACHIVEMENTREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_FetchAchivementReward *CreateRpc()
	{
		return new RpcC2G_FetchAchivementReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FetchAchiveArg &roArg, const FetchAchiveRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FetchAchiveArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FetchAchiveArg &roArg, FetchAchiveRes &roRes);
	void OnDelayReplyRpc(const FetchAchiveArg &roArg, FetchAchiveRes &roRes, const CUserData &roUserData);

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
	FetchAchiveArg m_oArg;
	FetchAchiveRes m_oRes;
};

#endif
