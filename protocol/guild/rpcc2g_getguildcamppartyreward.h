#ifndef __RPCC2G_GETGUILDCAMPPARTYREWARD_H__
#define __RPCC2G_GETGUILDCAMPPARTYREWARD_H__

// generate by ProtoGen at date: 2017/5/6 22:16:09

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETGUILDCAMPPARTYREWARD_TYPE 58935

class RpcC2G_GetGuildCampPartyReward : public CRpc
{
private:

	RpcC2G_GetGuildCampPartyReward():CRpc(RPCC2G_GETGUILDCAMPPARTYREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetGuildCampPartyReward *CreateRpc()
	{
		return new RpcC2G_GetGuildCampPartyReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGuildCampPartyRewardArg &roArg, const GetGuildCampPartyRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGuildCampPartyRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGuildCampPartyRewardArg &roArg, GetGuildCampPartyRewardRes &roRes);
	void OnDelayReplyRpc(const GetGuildCampPartyRewardArg &roArg, GetGuildCampPartyRewardRes &roRes, const CUserData &roUserData);

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
	GetGuildCampPartyRewardArg m_oArg;
	GetGuildCampPartyRewardRes m_oRes;
};

#endif
