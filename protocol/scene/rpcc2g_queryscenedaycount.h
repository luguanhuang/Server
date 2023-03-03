#ifndef __RPCC2G_QUERYSCENEDAYCOUNT_H__
#define __RPCC2G_QUERYSCENEDAYCOUNT_H__

// generate by ProtoGen at date: 2015/5/27 19:51:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_QUERYSCENEDAYCOUNT_TYPE 1676

class RpcC2G_QuerySceneDayCount : public CRpc
{
private:

	RpcC2G_QuerySceneDayCount():CRpc(RPCC2G_QUERYSCENEDAYCOUNT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_QuerySceneDayCount *CreateRpc()
	{
		return new RpcC2G_QuerySceneDayCount;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QuerySceneDayCountArg &roArg, const QuerySceneDayCountRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QuerySceneDayCountArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QuerySceneDayCountArg &roArg, QuerySceneDayCountRes &roRes);
	void OnDelayReplyRpc(const QuerySceneDayCountArg &roArg, QuerySceneDayCountRes &roRes, const CUserData &roUserData);

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
	QuerySceneDayCountArg m_oArg;
	QuerySceneDayCountRes m_oRes;
};

#endif
