#ifndef __RPCC2G_GETTOWERFIRSTPASSREWARD_H__
#define __RPCC2G_GETTOWERFIRSTPASSREWARD_H__

// generate by ProtoGen at date: 2016/9/19 11:53:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETTOWERFIRSTPASSREWARD_TYPE 55009

class RpcC2G_GetTowerFirstPassReward : public CRpc
{
private:

	RpcC2G_GetTowerFirstPassReward():CRpc(RPCC2G_GETTOWERFIRSTPASSREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetTowerFirstPassReward *CreateRpc()
	{
		return new RpcC2G_GetTowerFirstPassReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetTowerFirstPassRewardArg &roArg, const GetTowerFirstPassRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetTowerFirstPassRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetTowerFirstPassRewardArg &roArg, GetTowerFirstPassRewardRes &roRes);
	void OnDelayReplyRpc(const GetTowerFirstPassRewardArg &roArg, GetTowerFirstPassRewardRes &roRes, const CUserData &roUserData);

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
	GetTowerFirstPassRewardArg m_oArg;
	GetTowerFirstPassRewardRes m_oRes;
};

#endif
