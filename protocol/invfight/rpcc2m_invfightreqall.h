#ifndef __RPCC2M_INVFIGHTREQALL_H__
#define __RPCC2M_INVFIGHTREQALL_H__

// generate by ProtoGen at date: 2016/12/13 23:25:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_INVFIGHTREQALL_TYPE 56726

class RpcC2M_InvFightReqAll : public CRpc
{
private:

	RpcC2M_InvFightReqAll():CRpc(RPCC2M_INVFIGHTREQALL_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_InvFightReqAll *CreateRpc()
	{
		return new RpcC2M_InvFightReqAll;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const InvFightArg &roArg, const InvFightRes &roRes, const CUserData &roUserData);
	void OnTimeout(const InvFightArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const InvFightArg &roArg, InvFightRes &roRes);
	void OnDelayReplyRpc(const InvFightArg &roArg, InvFightRes &roRes, const CUserData &roUserData);

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
	InvFightArg m_oArg;
	InvFightRes m_oRes;
};

#endif
