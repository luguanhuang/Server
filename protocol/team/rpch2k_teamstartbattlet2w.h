#ifndef __RPCH2K_TEAMSTARTBATTLET2W_H__
#define __RPCH2K_TEAMSTARTBATTLET2W_H__

// generate by ProtoGen at date: 2017/6/27 11:42:48

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCH2K_TEAMSTARTBATTLET2W_TYPE 693

class RpcH2K_TeamStartBattleT2W : public CRpc
{
private:

	RpcH2K_TeamStartBattleT2W():CRpc(RPCH2K_TEAMSTARTBATTLET2W_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcH2K_TeamStartBattleT2W *CreateRpc()
	{
		return new RpcH2K_TeamStartBattleT2W;
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
