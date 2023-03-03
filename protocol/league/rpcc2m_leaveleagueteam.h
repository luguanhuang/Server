#ifndef __RPCC2M_LEAVELEAGUETEAM_H__
#define __RPCC2M_LEAVELEAGUETEAM_H__

// generate by ProtoGen at date: 2017/1/5 20:47:19

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_LEAVELEAGUETEAM_TYPE 47239

class RpcC2M_LeaveLeagueTeam : public CRpc
{
private:

	RpcC2M_LeaveLeagueTeam():CRpc(RPCC2M_LEAVELEAGUETEAM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_LeaveLeagueTeam *CreateRpc()
	{
		return new RpcC2M_LeaveLeagueTeam;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LeaveLeagueTeamArg &roArg, const LeaveLeagueTeamRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LeaveLeagueTeamArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LeaveLeagueTeamArg &roArg, LeaveLeagueTeamRes &roRes);
	void OnDelayReplyRpc(const LeaveLeagueTeamArg &roArg, LeaveLeagueTeamRes &roRes, const CUserData &roUserData);

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
	LeaveLeagueTeamArg m_oArg;
	LeaveLeagueTeamRes m_oRes;
};

#endif
