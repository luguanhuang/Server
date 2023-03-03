#ifndef __RPCC2G_COMMENDWATCHBATTLE_H__
#define __RPCC2G_COMMENDWATCHBATTLE_H__

// generate by ProtoGen at date: 2016/7/11 16:35:56

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_COMMENDWATCHBATTLE_TYPE 1476

class RpcC2G_CommendWatchBattle : public CRpc
{
private:

	RpcC2G_CommendWatchBattle():CRpc(RPCC2G_COMMENDWATCHBATTLE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_CommendWatchBattle *CreateRpc()
	{
		return new RpcC2G_CommendWatchBattle;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CommendWatchBattleArg &roArg, const CommendWatchBattleRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CommendWatchBattleArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CommendWatchBattleArg &roArg, CommendWatchBattleRes &roRes);
	void OnDelayReplyRpc(const CommendWatchBattleArg &roArg, CommendWatchBattleRes &roRes, const CUserData &roUserData);

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
	CommendWatchBattleArg m_oArg;
	CommendWatchBattleRes m_oRes;
};

#endif
