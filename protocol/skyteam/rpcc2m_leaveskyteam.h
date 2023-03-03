#ifndef __RPCC2M_LEAVESKYTEAM_H__
#define __RPCC2M_LEAVESKYTEAM_H__

// generate by ProtoGen at date: 2017/3/31 22:15:25

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_LEAVESKYTEAM_TYPE 26181

class RpcC2M_LeaveSkyTeam : public CRpc
{
private:

	RpcC2M_LeaveSkyTeam():CRpc(RPCC2M_LEAVESKYTEAM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_LeaveSkyTeam *CreateRpc()
	{
		return new RpcC2M_LeaveSkyTeam;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LeaveSkyTeamArg &roArg, const LeaveSkyTeamRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LeaveSkyTeamArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LeaveSkyTeamArg &roArg, LeaveSkyTeamRes &roRes);
	void OnDelayReplyRpc(const LeaveSkyTeamArg &roArg, LeaveSkyTeamRes &roRes, const CUserData &roUserData);

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
	LeaveSkyTeamArg m_oArg;
	LeaveSkyTeamRes m_oRes;
};

#endif
