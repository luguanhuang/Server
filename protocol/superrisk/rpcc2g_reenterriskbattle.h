#ifndef __RPCC2G_REENTERRISKBATTLE_H__
#define __RPCC2G_REENTERRISKBATTLE_H__

// generate by ProtoGen at date: 2016/9/5 16:21:07

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_REENTERRISKBATTLE_TYPE 1615

class RpcC2G_ReEnterRiskBattle : public CRpc
{
private:

	RpcC2G_ReEnterRiskBattle():CRpc(RPCC2G_REENTERRISKBATTLE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ReEnterRiskBattle *CreateRpc()
	{
		return new RpcC2G_ReEnterRiskBattle;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReEnterRiskBattleArg &roArg, const ReEnterRiskBattleRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReEnterRiskBattleArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReEnterRiskBattleArg &roArg, ReEnterRiskBattleRes &roRes);
	void OnDelayReplyRpc(const ReEnterRiskBattleArg &roArg, ReEnterRiskBattleRes &roRes, const CUserData &roUserData);

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
	ReEnterRiskBattleArg m_oArg;
	ReEnterRiskBattleRes m_oRes;
};

#endif
