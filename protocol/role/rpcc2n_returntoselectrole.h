#ifndef __RPCC2N_RETURNTOSELECTROLE_H__
#define __RPCC2N_RETURNTOSELECTROLE_H__

// generate by ProtoGen at date: 2016/11/3 2:19:50

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2N_RETURNTOSELECTROLE_TYPE 25477

class RpcC2N_ReturnToSelectRole : public CRpc
{
private:

	RpcC2N_ReturnToSelectRole():CRpc(RPCC2N_RETURNTOSELECTROLE_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2N_ReturnToSelectRole *CreateRpc()
	{
		return new RpcC2N_ReturnToSelectRole;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReturnToSelectRoleArg &roArg, const ReturnToSelectRoleRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReturnToSelectRoleArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReturnToSelectRoleArg &roArg, ReturnToSelectRoleRes &roRes);
	void OnDelayReplyRpc(const ReturnToSelectRoleArg &roArg, ReturnToSelectRoleRes &roRes, const CUserData &roUserData);

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
	ReturnToSelectRoleArg m_oArg;
	ReturnToSelectRoleRes m_oRes;
};

#endif
