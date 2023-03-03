#ifndef __RPCC2G_GETACHIEVEREWARDREQ_H__
#define __RPCC2G_GETACHIEVEREWARDREQ_H__

// generate by ProtoGen at date: 2016/5/31 15:50:58

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETACHIEVEREWARDREQ_TYPE 1577

class RpcC2G_GetAchieveRewardReq : public CRpc
{
private:

	RpcC2G_GetAchieveRewardReq():CRpc(RPCC2G_GETACHIEVEREWARDREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetAchieveRewardReq *CreateRpc()
	{
		return new RpcC2G_GetAchieveRewardReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetAchieveRewardReq &roArg, const GetAchieveRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetAchieveRewardReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetAchieveRewardReq &roArg, GetAchieveRewardRes &roRes);
	void OnDelayReplyRpc(const GetAchieveRewardReq &roArg, GetAchieveRewardRes &roRes, const CUserData &roUserData);

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
	GetAchieveRewardReq m_oArg;
	GetAchieveRewardRes m_oRes;
};

#endif
