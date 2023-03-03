#ifndef __RPCC2G_TURNOVERWEEKLYTASKITEM_H__
#define __RPCC2G_TURNOVERWEEKLYTASKITEM_H__

// generate by ProtoGen at date: 2017/9/8 9:57:25

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_TURNOVERWEEKLYTASKITEM_TYPE 19937

class RpcC2G_TurnOverWeeklyTaskItem : public CRpc
{
private:

	RpcC2G_TurnOverWeeklyTaskItem():CRpc(RPCC2G_TURNOVERWEEKLYTASKITEM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_TurnOverWeeklyTaskItem *CreateRpc()
	{
		return new RpcC2G_TurnOverWeeklyTaskItem;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TurnOverWeeklyTaskItemArg &roArg, const TurnOverWeeklyTaskItemRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TurnOverWeeklyTaskItemArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TurnOverWeeklyTaskItemArg &roArg, TurnOverWeeklyTaskItemRes &roRes);
	void OnDelayReplyRpc(const TurnOverWeeklyTaskItemArg &roArg, TurnOverWeeklyTaskItemRes &roRes, const CUserData &roUserData);

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
	TurnOverWeeklyTaskItemArg m_oArg;
	TurnOverWeeklyTaskItemRes m_oRes;
};

#endif
