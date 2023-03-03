#ifndef __RPCC2N_SELECTROLENEW_H__
#define __RPCC2N_SELECTROLENEW_H__

// generate by ProtoGen at date: 2016/11/3 20:16:20

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2N_SELECTROLENEW_TYPE 217

class RpcC2N_SelectRoleNew : public CRpc
{
private:

	RpcC2N_SelectRoleNew():CRpc(RPCC2N_SELECTROLENEW_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2N_SelectRoleNew *CreateRpc()
	{
		return new RpcC2N_SelectRoleNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SelectRoleNewArg &roArg, const SelectRoleNewRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SelectRoleNewArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SelectRoleNewArg &roArg, SelectRoleNewRes &roRes);
	void OnDelayReplyRpc(const SelectRoleNewArg &roArg, SelectRoleNewRes &roRes, const CUserData &roUserData);

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
	SelectRoleNewArg m_oArg;
	SelectRoleNewRes m_oRes;
};

#endif
