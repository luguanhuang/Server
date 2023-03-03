#ifndef __RPCC2M_GETLEAGUETEAMINFO_H__
#define __RPCC2M_GETLEAGUETEAMINFO_H__

// generate by ProtoGen at date: 2017/1/4 15:25:09

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETLEAGUETEAMINFO_TYPE 12488

class RpcC2M_GetLeagueTeamInfo : public CRpc
{
private:

	RpcC2M_GetLeagueTeamInfo():CRpc(RPCC2M_GETLEAGUETEAMINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetLeagueTeamInfo *CreateRpc()
	{
		return new RpcC2M_GetLeagueTeamInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetLeagueTeamInfoArg &roArg, const GetLeagueTeamInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetLeagueTeamInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetLeagueTeamInfoArg &roArg, GetLeagueTeamInfoRes &roRes);
	void OnDelayReplyRpc(const GetLeagueTeamInfoArg &roArg, GetLeagueTeamInfoRes &roRes, const CUserData &roUserData);

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
	GetLeagueTeamInfoArg m_oArg;
	GetLeagueTeamInfoRes m_oRes;
};

#endif
