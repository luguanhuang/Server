#ifndef __RPCC2G_GETACHIEVEPOINTREWARDREQ_H__
#define __RPCC2G_GETACHIEVEPOINTREWARDREQ_H__

// generate by ProtoGen at date: 2016/6/16 14:26:01

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETACHIEVEPOINTREWARDREQ_TYPE 13722

class RpcC2G_GetAchievePointRewardReq : public CRpc
{
private:

	RpcC2G_GetAchievePointRewardReq():CRpc(RPCC2G_GETACHIEVEPOINTREWARDREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetAchievePointRewardReq *CreateRpc()
	{
		return new RpcC2G_GetAchievePointRewardReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetAchievePointRewardReq &roArg, const GetAchievePointRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetAchievePointRewardReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetAchievePointRewardReq &roArg, GetAchievePointRewardRes &roRes);
	void OnDelayReplyRpc(const GetAchievePointRewardReq &roArg, GetAchievePointRewardRes &roRes, const CUserData &roUserData);

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
	GetAchievePointRewardReq m_oArg;
	GetAchievePointRewardRes m_oRes;
};

#endif
