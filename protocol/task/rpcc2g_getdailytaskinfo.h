#ifndef __RPCC2G_GETDAILYTASKINFO_H__
#define __RPCC2G_GETDAILYTASKINFO_H__

// generate by ProtoGen at date: 2016/11/14 19:56:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETDAILYTASKINFO_TYPE 52480

class RpcC2G_GetDailyTaskInfo : public CRpc
{
private:

	RpcC2G_GetDailyTaskInfo():CRpc(RPCC2G_GETDAILYTASKINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetDailyTaskInfo *CreateRpc()
	{
		return new RpcC2G_GetDailyTaskInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetDailyTaskInfoArg &roArg, const GetDailyTaskInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetDailyTaskInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetDailyTaskInfoArg &roArg, GetDailyTaskInfoRes &roRes);
	void OnDelayReplyRpc(const GetDailyTaskInfoArg &roArg, GetDailyTaskInfoRes &roRes, const CUserData &roUserData);

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
	GetDailyTaskInfoArg m_oArg;
	GetDailyTaskInfoRes m_oRes;
};

#endif
