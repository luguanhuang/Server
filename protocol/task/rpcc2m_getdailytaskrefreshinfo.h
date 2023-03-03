#ifndef __RPCC2M_GETDAILYTASKREFRESHINFO_H__
#define __RPCC2M_GETDAILYTASKREFRESHINFO_H__

// generate by ProtoGen at date: 2017/9/18 1:24:06

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETDAILYTASKREFRESHINFO_TYPE 42385

class RpcC2M_GetDailyTaskRefreshInfo : public CRpc
{
private:

	RpcC2M_GetDailyTaskRefreshInfo():CRpc(RPCC2M_GETDAILYTASKREFRESHINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetDailyTaskRefreshInfo *CreateRpc()
	{
		return new RpcC2M_GetDailyTaskRefreshInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetDailyTaskRefreshInfoArg &roArg, const GetDailyTaskRefreshInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetDailyTaskRefreshInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetDailyTaskRefreshInfoArg &roArg, GetDailyTaskRefreshInfoRes &roRes);
	void OnDelayReplyRpc(const GetDailyTaskRefreshInfoArg &roArg, GetDailyTaskRefreshInfoRes &roRes, const CUserData &roUserData);

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
	GetDailyTaskRefreshInfoArg m_oArg;
	GetDailyTaskRefreshInfoRes m_oRes;
};

#endif
