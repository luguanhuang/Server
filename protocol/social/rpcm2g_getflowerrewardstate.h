#ifndef __RPCM2G_GETFLOWERREWARDSTATE_H__
#define __RPCM2G_GETFLOWERREWARDSTATE_H__

// generate by ProtoGen at date: 2016/8/19 10:11:35

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_GETFLOWERREWARDSTATE_TYPE 34241

class RpcM2G_GetFlowerRewardState : public CRpc
{
private:

	RpcM2G_GetFlowerRewardState():CRpc(RPCM2G_GETFLOWERREWARDSTATE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_GetFlowerRewardState *CreateRpc()
	{
		return new RpcM2G_GetFlowerRewardState;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetFlowerRewardStateArg &roArg, const GetFlowerRewardStateRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetFlowerRewardStateArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetFlowerRewardStateArg &roArg, GetFlowerRewardStateRes &roRes);
	void OnDelayReplyRpc(const GetFlowerRewardStateArg &roArg, GetFlowerRewardStateRes &roRes, const CUserData &roUserData);

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
	GetFlowerRewardStateArg m_oArg;
	GetFlowerRewardStateRes m_oRes;
};

#endif
