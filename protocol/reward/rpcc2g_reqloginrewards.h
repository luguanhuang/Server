#ifndef __RPCC2G_REQLOGINREWARDS_H__
#define __RPCC2G_REQLOGINREWARDS_H__

// generate by ProtoGen at date: 2016/6/16 14:25:59

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_REQLOGINREWARDS_TYPE 49761

class RpcC2G_ReqLoginRewards : public CRpc
{
private:

	RpcC2G_ReqLoginRewards():CRpc(RPCC2G_REQLOGINREWARDS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ReqLoginRewards *CreateRpc()
	{
		return new RpcC2G_ReqLoginRewards;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LoginRewardReq &roArg, const LoginRewardRet &roRes, const CUserData &roUserData);
	void OnTimeout(const LoginRewardReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LoginRewardReq &roArg, LoginRewardRet &roRes);
	void OnDelayReplyRpc(const LoginRewardReq &roArg, LoginRewardRet &roRes, const CUserData &roUserData);

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
	LoginRewardReq m_oArg;
	LoginRewardRet m_oRes;
};

#endif
