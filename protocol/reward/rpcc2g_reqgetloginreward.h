#ifndef __RPCC2G_REQGETLOGINREWARD_H__
#define __RPCC2G_REQGETLOGINREWARD_H__

// generate by ProtoGen at date: 2016/6/16 14:25:57

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_REQGETLOGINREWARD_TYPE 44079

class RpcC2G_ReqGetLoginReward : public CRpc
{
private:

	RpcC2G_ReqGetLoginReward():CRpc(RPCC2G_REQGETLOGINREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ReqGetLoginReward *CreateRpc()
	{
		return new RpcC2G_ReqGetLoginReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LoginRewardGetReq &roArg, const LoginRewardGetRet &roRes, const CUserData &roUserData);
	void OnTimeout(const LoginRewardGetReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LoginRewardGetReq &roArg, LoginRewardGetRet &roRes);
	void OnDelayReplyRpc(const LoginRewardGetReq &roArg, LoginRewardGetRet &roRes, const CUserData &roUserData);

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
	LoginRewardGetReq m_oArg;
	LoginRewardGetRet m_oRes;
};

#endif
