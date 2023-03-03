#ifndef __RPCG2K_TEAMGOTONEXTSCENEREQ_H__
#define __RPCG2K_TEAMGOTONEXTSCENEREQ_H__

// generate by ProtoGen at date: 2017/10/16 17:03:04

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2K_TEAMGOTONEXTSCENEREQ_TYPE 52067

class RpcG2K_TeamGoToNextSceneReq : public CRpc
{
private:

	RpcG2K_TeamGoToNextSceneReq():CRpc(RPCG2K_TEAMGOTONEXTSCENEREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2K_TeamGoToNextSceneReq *CreateRpc()
	{
		return new RpcG2K_TeamGoToNextSceneReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TeamStartBattleArg &roArg, const TeamStartBattleRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TeamStartBattleArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TeamStartBattleArg &roArg, TeamStartBattleRes &roRes);
	void OnDelayReplyRpc(const TeamStartBattleArg &roArg, TeamStartBattleRes &roRes, const CUserData &roUserData);

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
	TeamStartBattleArg m_oArg;
	TeamStartBattleRes m_oRes;
};

#endif
