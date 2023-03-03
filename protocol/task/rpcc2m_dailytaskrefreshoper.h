#ifndef __RPCC2M_DAILYTASKREFRESHOPER_H__
#define __RPCC2M_DAILYTASKREFRESHOPER_H__

// generate by ProtoGen at date: 2017/9/18 1:45:18

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_DAILYTASKREFRESHOPER_TYPE 31675

class RpcC2M_DailyTaskRefreshOper : public CRpc
{
private:

	RpcC2M_DailyTaskRefreshOper():CRpc(RPCC2M_DAILYTASKREFRESHOPER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_DailyTaskRefreshOper *CreateRpc()
	{
		return new RpcC2M_DailyTaskRefreshOper;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DailyTaskRefreshOperArg &roArg, const DailyTaskRefreshOperRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DailyTaskRefreshOperArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DailyTaskRefreshOperArg &roArg, DailyTaskRefreshOperRes &roRes);
	void OnDelayReplyRpc(const DailyTaskRefreshOperArg &roArg, DailyTaskRefreshOperRes &roRes, const CUserData &roUserData);

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
	DailyTaskRefreshOperArg m_oArg;
	DailyTaskRefreshOperRes m_oRes;
};

#endif
