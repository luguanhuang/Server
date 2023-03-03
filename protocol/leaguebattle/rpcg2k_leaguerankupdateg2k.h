#ifndef __RPCG2K_LEAGUERANKUPDATEG2K_H__
#define __RPCG2K_LEAGUERANKUPDATEG2K_H__

// generate by ProtoGen at date: 2017/2/3 23:15:48

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2K_LEAGUERANKUPDATEG2K_TYPE 35073

class RpcG2K_LeagueRankUpdateG2K : public CRpc
{
private:

	RpcG2K_LeagueRankUpdateG2K():CRpc(RPCG2K_LEAGUERANKUPDATEG2K_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2K_LeagueRankUpdateG2K *CreateRpc()
	{
		return new RpcG2K_LeagueRankUpdateG2K;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LeagueRankUpdateG2MArg &roArg, const LeagueRankUpdateG2MRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LeagueRankUpdateG2MArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LeagueRankUpdateG2MArg &roArg, LeagueRankUpdateG2MRes &roRes);
	void OnDelayReplyRpc(const LeagueRankUpdateG2MArg &roArg, LeagueRankUpdateG2MRes &roRes, const CUserData &roUserData);

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
	LeagueRankUpdateG2MArg m_oArg;
	LeagueRankUpdateG2MRes m_oRes;
};

#endif
