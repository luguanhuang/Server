#ifndef __RPCC2M_LEAGUEBATTLEREQ_H__
#define __RPCC2M_LEAGUEBATTLEREQ_H__

// generate by ProtoGen at date: 2017/1/10 18:14:08

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_LEAGUEBATTLEREQ_TYPE 8012

class RpcC2M_LeagueBattleReq : public CRpc
{
private:

	RpcC2M_LeagueBattleReq():CRpc(RPCC2M_LEAGUEBATTLEREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_LeagueBattleReq *CreateRpc()
	{
		return new RpcC2M_LeagueBattleReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LeagueBattleReqArg &roArg, const LeagueBattleReqRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LeagueBattleReqArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LeagueBattleReqArg &roArg, LeagueBattleReqRes &roRes);
	void OnDelayReplyRpc(const LeagueBattleReqArg &roArg, LeagueBattleReqRes &roRes, const CUserData &roUserData);

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
	LeagueBattleReqArg m_oArg;
	LeagueBattleReqRes m_oRes;
};

#endif
