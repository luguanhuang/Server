#ifndef __RPCN2M_CHECKROLENAME_H__
#define __RPCN2M_CHECKROLENAME_H__

// generate by ProtoGen at date: 2016/11/9 0:09:48

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCN2M_CHECKROLENAME_TYPE 19417

class RpcN2M_CheckRoleName : public CRpc
{
private:

	RpcN2M_CheckRoleName():CRpc(RPCN2M_CHECKROLENAME_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcN2M_CheckRoleName *CreateRpc()
	{
		return new RpcN2M_CheckRoleName;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CheckRoleNameArg &roArg, const CheckRoleNameRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CheckRoleNameArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CheckRoleNameArg &roArg, CheckRoleNameRes &roRes);
	void OnDelayReplyRpc(const CheckRoleNameArg &roArg, CheckRoleNameRes &roRes, const CUserData &roUserData);

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
	CheckRoleNameArg m_oArg;
	CheckRoleNameRes m_oRes;
};

#endif
