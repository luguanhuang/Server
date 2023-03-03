#ifndef __RPCG2D_READHIDDENROLES_H__
#define __RPCG2D_READHIDDENROLES_H__

// generate by ProtoGen at date: 2017/1/12 17:26:14

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2D_READHIDDENROLES_TYPE 7384

class RpcG2D_ReadHiddenRoles : public CRpc
{
private:

	RpcG2D_ReadHiddenRoles():CRpc(RPCG2D_READHIDDENROLES_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2D_ReadHiddenRoles *CreateRpc()
	{
		return new RpcG2D_ReadHiddenRoles;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReadHiddenRolesArg &roArg, const ReadHiddenRolesRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReadHiddenRolesArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReadHiddenRolesArg &roArg, ReadHiddenRolesRes &roRes);
	void OnDelayReplyRpc(const ReadHiddenRolesArg &roArg, ReadHiddenRolesRes &roRes, const CUserData &roUserData);

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
	ReadHiddenRolesArg m_oArg;
	ReadHiddenRolesRes m_oRes;
};

#endif
