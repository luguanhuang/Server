#ifndef __RPCM2D_SAVELEAGUETEAMDATA_H__
#define __RPCM2D_SAVELEAGUETEAMDATA_H__

// generate by ProtoGen at date: 2017/3/31 17:14:39

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_SAVELEAGUETEAMDATA_TYPE 60667

class RpcM2D_SaveLeagueTeamData : public CRpc
{
private:

	RpcM2D_SaveLeagueTeamData():CRpc(RPCM2D_SAVELEAGUETEAMDATA_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_SaveLeagueTeamData *CreateRpc()
	{
		return new RpcM2D_SaveLeagueTeamData;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SaveLeagueTeamDataArg &roArg, const SaveLeagueTeamDataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SaveLeagueTeamDataArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SaveLeagueTeamDataArg &roArg, SaveLeagueTeamDataRes &roRes);
	void OnDelayReplyRpc(const SaveLeagueTeamDataArg &roArg, SaveLeagueTeamDataRes &roRes, const CUserData &roUserData);

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
	SaveLeagueTeamDataArg m_oArg;
	SaveLeagueTeamDataRes m_oRes;
};

#endif
