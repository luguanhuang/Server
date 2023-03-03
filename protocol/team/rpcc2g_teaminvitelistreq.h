#ifndef __RPCC2G_TEAMINVITELISTREQ_H__
#define __RPCC2G_TEAMINVITELISTREQ_H__

// generate by ProtoGen at date: 2016/7/6 10:28:07

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_TEAMINVITELISTREQ_TYPE 11403

class RpcC2G_TeamInviteListReq : public CRpc
{
private:

	RpcC2G_TeamInviteListReq():CRpc(RPCC2G_TEAMINVITELISTREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_TeamInviteListReq *CreateRpc()
	{
		return new RpcC2G_TeamInviteListReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TeamInviteArg &roArg, const TeamInviteRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TeamInviteArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TeamInviteArg &roArg, TeamInviteRes &roRes);
	void OnDelayReplyRpc(const TeamInviteArg &roArg, TeamInviteRes &roRes, const CUserData &roUserData);

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
	TeamInviteArg m_oArg;
	TeamInviteRes m_oRes;
};

#endif
