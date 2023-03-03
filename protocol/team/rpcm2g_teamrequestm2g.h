#ifndef __RPCM2G_TEAMREQUESTM2G_H__
#define __RPCM2G_TEAMREQUESTM2G_H__

// generate by ProtoGen at date: 2017/3/31 17:13:50

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_TEAMREQUESTM2G_TYPE 35416

class RpcM2G_TeamRequestM2G : public CRpc
{
private:

	RpcM2G_TeamRequestM2G():CRpc(RPCM2G_TEAMREQUESTM2G_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_TeamRequestM2G *CreateRpc()
	{
		return new RpcM2G_TeamRequestM2G;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TeamOPM2GArg &roArg, const TeamOPM2GRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TeamOPM2GArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TeamOPM2GArg &roArg, TeamOPM2GRes &roRes);
	void OnDelayReplyRpc(const TeamOPM2GArg &roArg, TeamOPM2GRes &roRes, const CUserData &roUserData);

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
	TeamOPM2GArg m_oArg;
	TeamOPM2GRes m_oRes;
};

#endif
