#ifndef __RPCG2N_CHANGESCENEVERIFY_H__
#define __RPCG2N_CHANGESCENEVERIFY_H__

// generate by ProtoGen at date: 2016/11/5 1:38:51

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2N_CHANGESCENEVERIFY_TYPE 41926

class RpcG2N_ChangeSceneVerify : public CRpc
{
private:

	RpcG2N_ChangeSceneVerify():CRpc(RPCG2N_CHANGESCENEVERIFY_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2N_ChangeSceneVerify *CreateRpc()
	{
		return new RpcG2N_ChangeSceneVerify;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChangeSceneVerifyArg &roArg, const ChangeSceneVerifyRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChangeSceneVerifyArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChangeSceneVerifyArg &roArg, ChangeSceneVerifyRes &roRes);
	void OnDelayReplyRpc(const ChangeSceneVerifyArg &roArg, ChangeSceneVerifyRes &roRes, const CUserData &roUserData);

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
	ChangeSceneVerifyArg m_oArg;
	ChangeSceneVerifyRes m_oRes;
};

#endif
