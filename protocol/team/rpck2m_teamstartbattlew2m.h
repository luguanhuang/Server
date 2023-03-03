#ifndef __RPCK2M_TEAMSTARTBATTLEW2M_H__
#define __RPCK2M_TEAMSTARTBATTLEW2M_H__

// generate by ProtoGen at date: 2017/6/27 11:38:45

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCK2M_TEAMSTARTBATTLEW2M_TYPE 4910

class RpcK2M_TeamStartBattleW2M : public CRpc
{
private:

	RpcK2M_TeamStartBattleW2M():CRpc(RPCK2M_TEAMSTARTBATTLEW2M_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcK2M_TeamStartBattleW2M *CreateRpc()
	{
		return new RpcK2M_TeamStartBattleW2M;
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
