#ifndef __RPCC2G_GETGUILDBONUSREWARD_H__
#define __RPCC2G_GETGUILDBONUSREWARD_H__

// generate by ProtoGen at date: 2015/10/8 20:17:14

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETGUILDBONUSREWARD_TYPE 55720

class RpcC2G_GetGuildBonusReward : public CRpc
{
private:

	RpcC2G_GetGuildBonusReward():CRpc(RPCC2G_GETGUILDBONUSREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetGuildBonusReward *CreateRpc()
	{
		return new RpcC2G_GetGuildBonusReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGuildBonusRewardArg &roArg, const GetGuildBonusRewardResult &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGuildBonusRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGuildBonusRewardArg &roArg, GetGuildBonusRewardResult &roRes);
	void OnDelayReplyRpc(const GetGuildBonusRewardArg &roArg, GetGuildBonusRewardResult &roRes, const CUserData &roUserData);

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
	GetGuildBonusRewardArg m_oArg;
	GetGuildBonusRewardResult m_oRes;
};

#endif
