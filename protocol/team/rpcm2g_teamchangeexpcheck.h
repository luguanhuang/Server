#ifndef __RPCM2G_TEAMCHANGEEXPCHECK_H__
#define __RPCM2G_TEAMCHANGEEXPCHECK_H__

// generate by ProtoGen at date: 2016/8/24 19:58:05

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_TEAMCHANGEEXPCHECK_TYPE 39450

class RpcM2G_TeamChangeExpCheck : public CRpc
{
private:

	RpcM2G_TeamChangeExpCheck():CRpc(RPCM2G_TEAMCHANGEEXPCHECK_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_TeamChangeExpCheck *CreateRpc()
	{
		return new RpcM2G_TeamChangeExpCheck;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TeamChangeExpArg &roArg, const TeamChangeExpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TeamChangeExpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TeamChangeExpArg &roArg, TeamChangeExpRes &roRes);
	void OnDelayReplyRpc(const TeamChangeExpArg &roArg, TeamChangeExpRes &roRes, const CUserData &roUserData);

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
	TeamChangeExpArg m_oArg;
	TeamChangeExpRes m_oRes;
};

#endif
