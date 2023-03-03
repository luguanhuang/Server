#ifndef __RPCC2G_LEAVEGUILD_H__
#define __RPCC2G_LEAVEGUILD_H__

// generate by ProtoGen at date: 2015/10/1 11:42:21

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_LEAVEGUILD_TYPE 37926

class RpcC2G_LeaveGuild : public CRpc
{
private:

	RpcC2G_LeaveGuild():CRpc(RPCC2G_LEAVEGUILD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_LeaveGuild *CreateRpc()
	{
		return new RpcC2G_LeaveGuild;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LeaveGuildArg &roArg, const LeaveGuildRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LeaveGuildArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LeaveGuildArg &roArg, LeaveGuildRes &roRes);
	void OnDelayReplyRpc(const LeaveGuildArg &roArg, LeaveGuildRes &roRes, const CUserData &roUserData);

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
	LeaveGuildArg m_oArg;
	LeaveGuildRes m_oRes;
};

#endif
