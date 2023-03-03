#ifndef __RPCC2G_ENTERWATCHBATTLE_H__
#define __RPCC2G_ENTERWATCHBATTLE_H__

// generate by ProtoGen at date: 2016/10/31 16:27:59

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ENTERWATCHBATTLE_TYPE 47590

class RpcC2G_EnterWatchBattle : public CRpc
{
private:

	RpcC2G_EnterWatchBattle():CRpc(RPCC2G_ENTERWATCHBATTLE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_EnterWatchBattle *CreateRpc()
	{
		return new RpcC2G_EnterWatchBattle;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EnterWatchBattleArg &roArg, const EnterWatchBattleRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EnterWatchBattleArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EnterWatchBattleArg &roArg, EnterWatchBattleRes &roRes);
	void OnDelayReplyRpc(const EnterWatchBattleArg &roArg, EnterWatchBattleRes &roRes, const CUserData &roUserData);

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
	EnterWatchBattleArg m_oArg;
	EnterWatchBattleRes m_oRes;
};

#endif
