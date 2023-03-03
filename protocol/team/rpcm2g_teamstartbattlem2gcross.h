#ifndef __RPCM2G_TEAMSTARTBATTLEM2GCROSS_H__
#define __RPCM2G_TEAMSTARTBATTLEM2GCROSS_H__

// generate by ProtoGen at date: 2017/6/29 17:25:35

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_TEAMSTARTBATTLEM2GCROSS_TYPE 18793

class RpcM2G_TeamStartBattleM2GCross : public CRpc
{
private:

	RpcM2G_TeamStartBattleM2GCross():CRpc(RPCM2G_TEAMSTARTBATTLEM2GCROSS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_TeamStartBattleM2GCross *CreateRpc()
	{
		return new RpcM2G_TeamStartBattleM2GCross;
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
