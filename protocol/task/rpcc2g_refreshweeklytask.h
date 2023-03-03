#ifndef __RPCC2G_REFRESHWEEKLYTASK_H__
#define __RPCC2G_REFRESHWEEKLYTASK_H__

// generate by ProtoGen at date: 2017/9/5 10:05:10

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_REFRESHWEEKLYTASK_TYPE 3384

class RpcC2G_RefreshWeeklyTask : public CRpc
{
private:

	RpcC2G_RefreshWeeklyTask():CRpc(RPCC2G_REFRESHWEEKLYTASK_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_RefreshWeeklyTask *CreateRpc()
	{
		return new RpcC2G_RefreshWeeklyTask;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RefreshWeeklyTaskArg &roArg, const RefreshWeeklyTaskRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RefreshWeeklyTaskArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RefreshWeeklyTaskArg &roArg, RefreshWeeklyTaskRes &roRes);
	void OnDelayReplyRpc(const RefreshWeeklyTaskArg &roArg, RefreshWeeklyTaskRes &roRes, const CUserData &roUserData);

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
	RefreshWeeklyTaskArg m_oArg;
	RefreshWeeklyTaskRes m_oRes;
};

#endif
