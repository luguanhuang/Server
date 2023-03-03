#ifndef __RPCC2N_CREATEROLENEW_H__
#define __RPCC2N_CREATEROLENEW_H__

// generate by ProtoGen at date: 2016/11/3 20:22:32

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2N_CREATEROLENEW_TYPE 13034

class RpcC2N_CreateRoleNew : public CRpc
{
private:

	RpcC2N_CreateRoleNew():CRpc(RPCC2N_CREATEROLENEW_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2N_CreateRoleNew *CreateRpc()
	{
		return new RpcC2N_CreateRoleNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CreateRoleNewArg &roArg, const CreateRoleNewRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CreateRoleNewArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CreateRoleNewArg &roArg, CreateRoleNewRes &roRes);
	void OnDelayReplyRpc(const CreateRoleNewArg &roArg, CreateRoleNewRes &roRes, const CUserData &roUserData);

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
	CreateRoleNewArg m_oArg;
	CreateRoleNewRes m_oRes;
};

#endif
