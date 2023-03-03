#ifndef __RPCC2M_JOINLEAGUEELEBATTLE_H__
#define __RPCC2M_JOINLEAGUEELEBATTLE_H__

// generate by ProtoGen at date: 2017/1/14 15:20:24

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_JOINLEAGUEELEBATTLE_TYPE 43053

class RpcC2M_JoinLeagueEleBattle : public CRpc
{
private:

	RpcC2M_JoinLeagueEleBattle():CRpc(RPCC2M_JOINLEAGUEELEBATTLE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_JoinLeagueEleBattle *CreateRpc()
	{
		return new RpcC2M_JoinLeagueEleBattle;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const JoinLeagueEleBattleArg &roArg, const JoinLeagueEleBattleRes &roRes, const CUserData &roUserData);
	void OnTimeout(const JoinLeagueEleBattleArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const JoinLeagueEleBattleArg &roArg, JoinLeagueEleBattleRes &roRes);
	void OnDelayReplyRpc(const JoinLeagueEleBattleArg &roArg, JoinLeagueEleBattleRes &roRes, const CUserData &roUserData);

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
	JoinLeagueEleBattleArg m_oArg;
	JoinLeagueEleBattleRes m_oRes;
};

#endif
