#ifndef __RPCC2M_TEAMREQUESTC2M_H__
#define __RPCC2M_TEAMREQUESTC2M_H__

// generate by ProtoGen at date: 2016/8/18 17:18:40

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_TEAMREQUESTC2M_TYPE 30954

class RpcC2M_TeamRequestC2M : public CRpc
{
private:

	RpcC2M_TeamRequestC2M():CRpc(RPCC2M_TEAMREQUESTC2M_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_TeamRequestC2M *CreateRpc()
	{
		return new RpcC2M_TeamRequestC2M;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TeamOPArg &roArg, const TeamOPRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TeamOPArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TeamOPArg &roArg, TeamOPRes &roRes);
	void OnDelayReplyRpc(const TeamOPArg &roArg, TeamOPRes &roRes, const CUserData &roUserData);

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
	TeamOPArg m_oArg;
	TeamOPRes m_oRes;
};

#endif
