#ifndef __RPCM2G_TEAMSTARTBATTLEM2G_H__
#define __RPCM2G_TEAMSTARTBATTLEM2G_H__

// generate by ProtoGen at date: 2016/8/18 17:31:11

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_TEAMSTARTBATTLEM2G_TYPE 3613

class RpcM2G_TeamStartBattleM2G : public CRpc
{
private:

	RpcM2G_TeamStartBattleM2G():CRpc(RPCM2G_TEAMSTARTBATTLEM2G_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_TeamStartBattleM2G *CreateRpc()
	{
		return new RpcM2G_TeamStartBattleM2G;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TeamStartBattleM2GArg &roArg, const TeamStartBattleM2GRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TeamStartBattleM2GArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TeamStartBattleM2GArg &roArg, TeamStartBattleM2GRes &roRes);
	void OnDelayReplyRpc(const TeamStartBattleM2GArg &roArg, TeamStartBattleM2GRes &roRes, const CUserData &roUserData);

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
	TeamStartBattleM2GArg m_oArg;
	TeamStartBattleM2GRes m_oRes;
};

#endif
