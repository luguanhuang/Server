#ifndef __RPCC2G_GETFIRSTPASSREWARD_H__
#define __RPCC2G_GETFIRSTPASSREWARD_H__

// generate by ProtoGen at date: 2016/8/24 17:19:45

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETFIRSTPASSREWARD_TYPE 12301

class RpcC2G_GetFirstPassReward : public CRpc
{
private:

	RpcC2G_GetFirstPassReward():CRpc(RPCC2G_GETFIRSTPASSREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetFirstPassReward *CreateRpc()
	{
		return new RpcC2G_GetFirstPassReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetFirstPassRewardArg &roArg, const GetFirstPassRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetFirstPassRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetFirstPassRewardArg &roArg, GetFirstPassRewardRes &roRes);
	void OnDelayReplyRpc(const GetFirstPassRewardArg &roArg, GetFirstPassRewardRes &roRes, const CUserData &roUserData);

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
	GetFirstPassRewardArg m_oArg;
	GetFirstPassRewardRes m_oRes;
};

#endif
