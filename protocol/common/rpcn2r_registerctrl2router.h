#ifndef __RPCN2R_REGISTERCTRL2ROUTER_H__
#define __RPCN2R_REGISTERCTRL2ROUTER_H__

// generate by ProtoGen at date: 2016/11/5 15:58:48

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCN2R_REGISTERCTRL2ROUTER_TYPE 22908

class RpcN2R_RegisterCtrl2Router : public CRpc
{
private:

	RpcN2R_RegisterCtrl2Router():CRpc(RPCN2R_REGISTERCTRL2ROUTER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcN2R_RegisterCtrl2Router *CreateRpc()
	{
		return new RpcN2R_RegisterCtrl2Router;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterCtrl2RouterArg &roArg, const RegisterCtrl2RouterRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterCtrl2RouterArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterCtrl2RouterArg &roArg, RegisterCtrl2RouterRes &roRes);
	void OnDelayReplyRpc(const RegisterCtrl2RouterArg &roArg, RegisterCtrl2RouterRes &roRes, const CUserData &roUserData);

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
	RegisterCtrl2RouterArg m_oArg;
	RegisterCtrl2RouterRes m_oRes;
};

#endif
