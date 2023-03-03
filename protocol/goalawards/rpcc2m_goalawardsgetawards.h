#ifndef __RPCC2M_GOALAWARDSGETAWARDS_H__
#define __RPCC2M_GOALAWARDSGETAWARDS_H__

// generate by ProtoGen at date: 2017/9/1 19:46:26

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GOALAWARDSGETAWARDS_TYPE 4985

class RpcC2M_GoalAwardsGetAwards : public CRpc
{
private:

	RpcC2M_GoalAwardsGetAwards():CRpc(RPCC2M_GOALAWARDSGETAWARDS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GoalAwardsGetAwards *CreateRpc()
	{
		return new RpcC2M_GoalAwardsGetAwards;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GoalAwardsGetAwards_C2M &roArg, const GoalAwardsGetAwards_M2C &roRes, const CUserData &roUserData);
	void OnTimeout(const GoalAwardsGetAwards_C2M &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GoalAwardsGetAwards_C2M &roArg, GoalAwardsGetAwards_M2C &roRes);
	void OnDelayReplyRpc(const GoalAwardsGetAwards_C2M &roArg, GoalAwardsGetAwards_M2C &roRes, const CUserData &roUserData);

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
	GoalAwardsGetAwards_C2M m_oArg;
	GoalAwardsGetAwards_M2C m_oRes;
};

#endif
