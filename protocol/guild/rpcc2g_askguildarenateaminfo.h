#ifndef __RPCC2G_ASKGUILDARENATEAMINFO_H__
#define __RPCC2G_ASKGUILDARENATEAMINFO_H__

// generate by ProtoGen at date: 2016/7/19 16:00:26

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ASKGUILDARENATEAMINFO_TYPE 59672

class RpcC2G_AskGuildArenaTeamInfo : public CRpc
{
private:

	RpcC2G_AskGuildArenaTeamInfo():CRpc(RPCC2G_ASKGUILDARENATEAMINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_AskGuildArenaTeamInfo *CreateRpc()
	{
		return new RpcC2G_AskGuildArenaTeamInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AskGuildArenaTeamInfoArg &roArg, const AskGuildArenaTeamInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AskGuildArenaTeamInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AskGuildArenaTeamInfoArg &roArg, AskGuildArenaTeamInfoRes &roRes);
	void OnDelayReplyRpc(const AskGuildArenaTeamInfoArg &roArg, AskGuildArenaTeamInfoRes &roRes, const CUserData &roUserData);

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
	AskGuildArenaTeamInfoArg m_oArg;
	AskGuildArenaTeamInfoRes m_oRes;
};

#endif
