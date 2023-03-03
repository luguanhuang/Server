#ifndef __RPCD2R_REGISTERDB2ROUTER_H__
#define __RPCD2R_REGISTERDB2ROUTER_H__

// generate by ProtoGen at date: 2016/12/3 17:01:12

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCD2R_REGISTERDB2ROUTER_TYPE 46405

class RpcD2R_RegisterDb2Router : public CRpc
{
private:

	RpcD2R_RegisterDb2Router():CRpc(RPCD2R_REGISTERDB2ROUTER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcD2R_RegisterDb2Router *CreateRpc()
	{
		return new RpcD2R_RegisterDb2Router;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterDb2RouterArg &roArg, const RegisterDb2RouterRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterDb2RouterArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterDb2RouterArg &roArg, RegisterDb2RouterRes &roRes);
	void OnDelayReplyRpc(const RegisterDb2RouterArg &roArg, RegisterDb2RouterRes &roRes, const CUserData &roUserData);

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
	RegisterDb2RouterArg m_oArg;
	RegisterDb2RouterRes m_oRes;
};

#endif
