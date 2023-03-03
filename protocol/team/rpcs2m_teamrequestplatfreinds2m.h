#ifndef __RPCS2M_TEAMREQUESTPLATFREINDS2M_H__
#define __RPCS2M_TEAMREQUESTPLATFREINDS2M_H__

// generate by ProtoGen at date: 2017/5/3 15:27:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCS2M_TEAMREQUESTPLATFREINDS2M_TYPE 27908

class RpcS2M_TeamRequestPlatFreindS2M : public CRpc
{
private:

	RpcS2M_TeamRequestPlatFreindS2M():CRpc(RPCS2M_TEAMREQUESTPLATFREINDS2M_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcS2M_TeamRequestPlatFreindS2M *CreateRpc()
	{
		return new RpcS2M_TeamRequestPlatFreindS2M;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TeamRequestPlatFreindS2MArg &roArg, const TeamRequestPlatFreindS2MRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TeamRequestPlatFreindS2MArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TeamRequestPlatFreindS2MArg &roArg, TeamRequestPlatFreindS2MRes &roRes);
	void OnDelayReplyRpc(const TeamRequestPlatFreindS2MArg &roArg, TeamRequestPlatFreindS2MRes &roRes, const CUserData &roUserData);

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
	TeamRequestPlatFreindS2MArg m_oArg;
	TeamRequestPlatFreindS2MRes m_oRes;
};

#endif
