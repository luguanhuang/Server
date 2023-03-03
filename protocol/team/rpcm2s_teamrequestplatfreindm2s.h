#ifndef __RPCM2S_TEAMREQUESTPLATFREINDM2S_H__
#define __RPCM2S_TEAMREQUESTPLATFREINDM2S_H__

// generate by ProtoGen at date: 2017/5/3 15:25:11

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2S_TEAMREQUESTPLATFREINDM2S_TYPE 31392

class RpcM2S_TeamRequestPlatFreindM2S : public CRpc
{
private:

	RpcM2S_TeamRequestPlatFreindM2S():CRpc(RPCM2S_TEAMREQUESTPLATFREINDM2S_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2S_TeamRequestPlatFreindM2S *CreateRpc()
	{
		return new RpcM2S_TeamRequestPlatFreindM2S;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TeamRequestPlatFreindM2SArg &roArg, const TeamRequestPlatFreindM2SRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TeamRequestPlatFreindM2SArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TeamRequestPlatFreindM2SArg &roArg, TeamRequestPlatFreindM2SRes &roRes);
	void OnDelayReplyRpc(const TeamRequestPlatFreindM2SArg &roArg, TeamRequestPlatFreindM2SRes &roRes, const CUserData &roUserData);

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
	TeamRequestPlatFreindM2SArg m_oArg;
	TeamRequestPlatFreindM2SRes m_oRes;
};

#endif
