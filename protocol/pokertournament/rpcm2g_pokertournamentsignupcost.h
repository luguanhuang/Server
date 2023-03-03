#ifndef __RPCM2G_POKERTOURNAMENTSIGNUPCOST_H__
#define __RPCM2G_POKERTOURNAMENTSIGNUPCOST_H__

// generate by ProtoGen at date: 2016/12/16 15:48:43

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_POKERTOURNAMENTSIGNUPCOST_TYPE 64780

class RpcM2G_PokerTournamentSignUpCost : public CRpc
{
private:

	RpcM2G_PokerTournamentSignUpCost():CRpc(RPCM2G_POKERTOURNAMENTSIGNUPCOST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_PokerTournamentSignUpCost *CreateRpc()
	{
		return new RpcM2G_PokerTournamentSignUpCost;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ConsumeItemsArg &roArg, const PokerTournamentSignUpCostRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ConsumeItemsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ConsumeItemsArg &roArg, PokerTournamentSignUpCostRes &roRes);
	void OnDelayReplyRpc(const ConsumeItemsArg &roArg, PokerTournamentSignUpCostRes &roRes, const CUserData &roUserData);

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
	ConsumeItemsArg m_oArg;
	PokerTournamentSignUpCostRes m_oRes;
};

#endif
