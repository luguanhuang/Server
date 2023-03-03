#ifndef __RPCC2G_GETWEEKLYTASKINFO_H__
#define __RPCC2G_GETWEEKLYTASKINFO_H__

// generate by ProtoGen at date: 2017/9/5 9:59:47

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETWEEKLYTASKINFO_TYPE 44747

class RpcC2G_GetWeeklyTaskInfo : public CRpc
{
private:

	RpcC2G_GetWeeklyTaskInfo():CRpc(RPCC2G_GETWEEKLYTASKINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetWeeklyTaskInfo *CreateRpc()
	{
		return new RpcC2G_GetWeeklyTaskInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetWeeklyTaskInfoArg &roArg, const GetWeeklyTaskInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetWeeklyTaskInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetWeeklyTaskInfoArg &roArg, GetWeeklyTaskInfoRes &roRes);
	void OnDelayReplyRpc(const GetWeeklyTaskInfoArg &roArg, GetWeeklyTaskInfoRes &roRes, const CUserData &roUserData);

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
	GetWeeklyTaskInfoArg m_oArg;
	GetWeeklyTaskInfoRes m_oRes;
};

#endif
