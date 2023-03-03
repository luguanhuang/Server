#ifndef __RPCM2G_FETCHROLEINFOONMS_H__
#define __RPCM2G_FETCHROLEINFOONMS_H__

// generate by ProtoGen at date: 2016/11/23 15:33:01

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_FETCHROLEINFOONMS_TYPE 47736

class RpcM2G_FetchRoleInfoOnMs : public CRpc
{
private:

	RpcM2G_FetchRoleInfoOnMs():CRpc(RPCM2G_FETCHROLEINFOONMS_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_FetchRoleInfoOnMs *CreateRpc()
	{
		return new RpcM2G_FetchRoleInfoOnMs;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FetchRoleInfoOnMsArg &roArg, const FetchRoleInfoOnMsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FetchRoleInfoOnMsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FetchRoleInfoOnMsArg &roArg, FetchRoleInfoOnMsRes &roRes);
	void OnDelayReplyRpc(const FetchRoleInfoOnMsArg &roArg, FetchRoleInfoOnMsRes &roRes, const CUserData &roUserData);

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
	FetchRoleInfoOnMsArg m_oArg;
	FetchRoleInfoOnMsRes m_oRes;
};

#endif
