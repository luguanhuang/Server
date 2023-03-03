#ifndef __RPCC2M_GOALAWARDSGETLIST_H__
#define __RPCC2M_GOALAWARDSGETLIST_H__

// generate by ProtoGen at date: 2017/9/1 19:44:53

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GOALAWARDSGETLIST_TYPE 36694

class RpcC2M_GoalAwardsGetList : public CRpc
{
private:

	RpcC2M_GoalAwardsGetList():CRpc(RPCC2M_GOALAWARDSGETLIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GoalAwardsGetList *CreateRpc()
	{
		return new RpcC2M_GoalAwardsGetList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GoalAwardsGetList_C2M &roArg, const GoalAwardsGetList_M2C &roRes, const CUserData &roUserData);
	void OnTimeout(const GoalAwardsGetList_C2M &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GoalAwardsGetList_C2M &roArg, GoalAwardsGetList_M2C &roRes);
	void OnDelayReplyRpc(const GoalAwardsGetList_C2M &roArg, GoalAwardsGetList_M2C &roRes, const CUserData &roUserData);

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
	GoalAwardsGetList_C2M m_oArg;
	GoalAwardsGetList_M2C m_oRes;
};

#endif
