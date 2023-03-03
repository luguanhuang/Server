#ifndef __RPCC2G_SETROLECONFIG_H__
#define __RPCC2G_SETROLECONFIG_H__

// generate by ProtoGen at date: 2016/1/21 16:36:01

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SETROLECONFIG_TYPE 35306

class RpcC2G_SetRoleConfig : public CRpc
{
private:

	RpcC2G_SetRoleConfig():CRpc(RPCC2G_SETROLECONFIG_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SetRoleConfig *CreateRpc()
	{
		return new RpcC2G_SetRoleConfig;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SetRoleConfigReq &roArg, const SetRoleConfigRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SetRoleConfigReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SetRoleConfigReq &roArg, SetRoleConfigRes &roRes);
	void OnDelayReplyRpc(const SetRoleConfigReq &roArg, SetRoleConfigRes &roRes, const CUserData &roUserData);

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
	SetRoleConfigReq m_oArg;
	SetRoleConfigRes m_oRes;
};

#endif
