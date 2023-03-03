#ifndef __RPCC2G_GETSWEEPTOWERREWARD_H__
#define __RPCC2G_GETSWEEPTOWERREWARD_H__

// generate by ProtoGen at date: 2016/9/12 11:53:57

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETSWEEPTOWERREWARD_TYPE 23703

class RpcC2G_GetSweepTowerReward : public CRpc
{
private:

	RpcC2G_GetSweepTowerReward():CRpc(RPCC2G_GETSWEEPTOWERREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetSweepTowerReward *CreateRpc()
	{
		return new RpcC2G_GetSweepTowerReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetSweepTowerRewardArg &roArg, const GetSweepTowerRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetSweepTowerRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetSweepTowerRewardArg &roArg, GetSweepTowerRewardRes &roRes);
	void OnDelayReplyRpc(const GetSweepTowerRewardArg &roArg, GetSweepTowerRewardRes &roRes, const CUserData &roUserData);

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
	GetSweepTowerRewardArg m_oArg;
	GetSweepTowerRewardRes m_oRes;
};

#endif
