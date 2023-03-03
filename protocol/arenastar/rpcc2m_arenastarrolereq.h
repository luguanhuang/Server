#ifndef __RPCC2M_ARENASTARROLEREQ_H__
#define __RPCC2M_ARENASTARROLEREQ_H__

// generate by ProtoGen at date: 2017/3/29 20:29:34

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ARENASTARROLEREQ_TYPE 53598

class RpcC2M_ArenaStarRoleReq : public CRpc
{
private:

	RpcC2M_ArenaStarRoleReq():CRpc(RPCC2M_ARENASTARROLEREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ArenaStarRoleReq *CreateRpc()
	{
		return new RpcC2M_ArenaStarRoleReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ArenaStarReqArg &roArg, const ArenaStarReqRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ArenaStarReqArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ArenaStarReqArg &roArg, ArenaStarReqRes &roRes);
	void OnDelayReplyRpc(const ArenaStarReqArg &roArg, ArenaStarReqRes &roRes, const CUserData &roUserData);

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
	ArenaStarReqArg m_oArg;
	ArenaStarReqRes m_oRes;
};

#endif
