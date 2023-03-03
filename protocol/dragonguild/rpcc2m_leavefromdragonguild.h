#ifndef __RPCC2M_LEAVEFROMDRAGONGUILD_H__
#define __RPCC2M_LEAVEFROMDRAGONGUILD_H__

// generate by ProtoGen at date: 2017/9/7 17:24:34

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_LEAVEFROMDRAGONGUILD_TYPE 9882

class RpcC2M_LeaveFromDragonGuild : public CRpc
{
private:

	RpcC2M_LeaveFromDragonGuild():CRpc(RPCC2M_LEAVEFROMDRAGONGUILD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_LeaveFromDragonGuild *CreateRpc()
	{
		return new RpcC2M_LeaveFromDragonGuild;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LeaveDragonGuildArg &roArg, const LeaveDragonGuildRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LeaveDragonGuildArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LeaveDragonGuildArg &roArg, LeaveDragonGuildRes &roRes);
	void OnDelayReplyRpc(const LeaveDragonGuildArg &roArg, LeaveDragonGuildRes &roRes, const CUserData &roUserData);

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
	LeaveDragonGuildArg m_oArg;
	LeaveDragonGuildRes m_oRes;
};

#endif
