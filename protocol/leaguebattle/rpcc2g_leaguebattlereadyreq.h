#ifndef __RPCC2G_LEAGUEBATTLEREADYREQ_H__
#define __RPCC2G_LEAGUEBATTLEREADYREQ_H__

// generate by ProtoGen at date: 2017/1/11 16:51:08

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_LEAGUEBATTLEREADYREQ_TYPE 15873

class RpcC2G_LeagueBattleReadyReq : public CRpc
{
private:

	RpcC2G_LeagueBattleReadyReq():CRpc(RPCC2G_LEAGUEBATTLEREADYREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_LeagueBattleReadyReq *CreateRpc()
	{
		return new RpcC2G_LeagueBattleReadyReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LeagueBattleReadyReqArg &roArg, const LeagueBattleReadyReqRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LeagueBattleReadyReqArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LeagueBattleReadyReqArg &roArg, LeagueBattleReadyReqRes &roRes);
	void OnDelayReplyRpc(const LeagueBattleReadyReqArg &roArg, LeagueBattleReadyReqRes &roRes, const CUserData &roUserData);

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
	LeagueBattleReadyReqArg m_oArg;
	LeagueBattleReadyReqRes m_oRes;
};

#endif
