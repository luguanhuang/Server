#ifndef __RPCN2D_CREATEROLEONDBNEW_H__
#define __RPCN2D_CREATEROLEONDBNEW_H__

// generate by ProtoGen at date: 2016/11/9 14:34:56

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCN2D_CREATEROLEONDBNEW_TYPE 8069

class RpcN2D_CreateRoleOnDBNew : public CRpc
{
private:

	RpcN2D_CreateRoleOnDBNew():CRpc(RPCN2D_CREATEROLEONDBNEW_TYPE)
	{
        m_dwTimeout = 600000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcN2D_CreateRoleOnDBNew *CreateRpc()
	{
		return new RpcN2D_CreateRoleOnDBNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CreateRoleOnDBNewArg &roArg, const CreateRoleOnDBNewRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CreateRoleOnDBNewArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CreateRoleOnDBNewArg &roArg, CreateRoleOnDBNewRes &roRes);
	void OnDelayReplyRpc(const CreateRoleOnDBNewArg &roArg, CreateRoleOnDBNewRes &roRes, const CUserData &roUserData);

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
	CreateRoleOnDBNewArg m_oArg;
	CreateRoleOnDBNewRes m_oRes;
};

#endif
