#ifndef __RPCC2G_SETHEROINHEROBATTLE_H__
#define __RPCC2G_SETHEROINHEROBATTLE_H__

// generate by ProtoGen at date: 2016/12/21 21:21:01

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SETHEROINHEROBATTLE_TYPE 18341

class RpcC2G_SetHeroInHeroBattle : public CRpc
{
private:

	RpcC2G_SetHeroInHeroBattle():CRpc(RPCC2G_SETHEROINHEROBATTLE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SetHeroInHeroBattle *CreateRpc()
	{
		return new RpcC2G_SetHeroInHeroBattle;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SetHeroInHeroBattleArg &roArg, const SetHeroInHeroBattleRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SetHeroInHeroBattleArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SetHeroInHeroBattleArg &roArg, SetHeroInHeroBattleRes &roRes);
	void OnDelayReplyRpc(const SetHeroInHeroBattleArg &roArg, SetHeroInHeroBattleRes &roRes, const CUserData &roUserData);

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
	SetHeroInHeroBattleArg m_oArg;
	SetHeroInHeroBattleRes m_oRes;
};

#endif
