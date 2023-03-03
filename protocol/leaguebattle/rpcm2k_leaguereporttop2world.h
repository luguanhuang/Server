#ifndef __RPCM2K_LEAGUEREPORTTOP2WORLD_H__
#define __RPCM2K_LEAGUEREPORTTOP2WORLD_H__

// generate by ProtoGen at date: 2017/3/15 10:57:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2K_LEAGUEREPORTTOP2WORLD_TYPE 41468

class RpcM2K_LeagueReportTop2World : public CRpc
{
private:

	RpcM2K_LeagueReportTop2World():CRpc(RPCM2K_LEAGUEREPORTTOP2WORLD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2K_LeagueReportTop2World *CreateRpc()
	{
		return new RpcM2K_LeagueReportTop2World;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LeagueReportTop2WorldArg &roArg, const LeagueReportTop2WorldRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LeagueReportTop2WorldArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LeagueReportTop2WorldArg &roArg, LeagueReportTop2WorldRes &roRes);
	void OnDelayReplyRpc(const LeagueReportTop2WorldArg &roArg, LeagueReportTop2WorldRes &roRes, const CUserData &roUserData);

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
	LeagueReportTop2WorldArg m_oArg;
	LeagueReportTop2WorldRes m_oRes;
};

#endif
