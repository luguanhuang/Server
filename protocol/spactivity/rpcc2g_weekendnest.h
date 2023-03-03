#ifndef __RPCC2G_WEEKENDNEST_H__
#define __RPCC2G_WEEKENDNEST_H__

// generate by ProtoGen at date: 2017/8/22 16:57:14

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_WEEKENDNEST_TYPE 8058

class RpcC2G_WeekEndNest : public CRpc
{
private:

	RpcC2G_WeekEndNest():CRpc(RPCC2G_WEEKENDNEST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_WeekEndNest *CreateRpc()
	{
		return new RpcC2G_WeekEndNest;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const WeekEndNestArg &roArg, const WeekEndNestRes &roRes, const CUserData &roUserData);
	void OnTimeout(const WeekEndNestArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const WeekEndNestArg &roArg, WeekEndNestRes &roRes);
	void OnDelayReplyRpc(const WeekEndNestArg &roArg, WeekEndNestRes &roRes, const CUserData &roUserData);

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
	WeekEndNestArg m_oArg;
	WeekEndNestRes m_oRes;
};

#endif
