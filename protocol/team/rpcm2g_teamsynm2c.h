#ifndef __RPCM2G_TEAMSYNM2C_H__
#define __RPCM2G_TEAMSYNM2C_H__

// generate by ProtoGen at date: 2016/8/16 20:52:03

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_TEAMSYNM2C_TYPE 36098

class RpcM2G_TeamSynM2C : public CRpc
{
private:

	RpcM2G_TeamSynM2C():CRpc(RPCM2G_TEAMSYNM2C_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_TeamSynM2C *CreateRpc()
	{
		return new RpcM2G_TeamSynM2C;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TeamSynPara &roArg, const TeamSynRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TeamSynPara &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TeamSynPara &roArg, TeamSynRes &roRes);
	void OnDelayReplyRpc(const TeamSynPara &roArg, TeamSynRes &roRes, const CUserData &roUserData);

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
	TeamSynPara m_oArg;
	TeamSynRes m_oRes;
};

#endif
