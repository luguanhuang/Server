#ifndef __RPCC2M_LEAVEFROMGUILD_H__
#define __RPCC2M_LEAVEFROMGUILD_H__

// generate by ProtoGen at date: 2016/8/27 10:34:46

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_LEAVEFROMGUILD_TYPE 2565

class RpcC2M_LeaveFromGuild : public CRpc
{
private:

	RpcC2M_LeaveFromGuild():CRpc(RPCC2M_LEAVEFROMGUILD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_LeaveFromGuild *CreateRpc()
	{
		return new RpcC2M_LeaveFromGuild;
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
