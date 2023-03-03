#ifndef __RPCC2M_GETWORLDBOSSTIMELEFT_H__
#define __RPCC2M_GETWORLDBOSSTIMELEFT_H__

// generate by ProtoGen at date: 2016/8/9 0:43:42

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETWORLDBOSSTIMELEFT_TYPE 23195

class RpcC2M_GetWorldBossTimeLeft : public CRpc
{
private:

	RpcC2M_GetWorldBossTimeLeft():CRpc(RPCC2M_GETWORLDBOSSTIMELEFT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetWorldBossTimeLeft *CreateRpc()
	{
		return new RpcC2M_GetWorldBossTimeLeft;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetWorldBossTimeLeftArg &roArg, const GetWorldBossTimeLeftRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetWorldBossTimeLeftArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetWorldBossTimeLeftArg &roArg, GetWorldBossTimeLeftRes &roRes);
	void OnDelayReplyRpc(const GetWorldBossTimeLeftArg &roArg, GetWorldBossTimeLeftRes &roRes, const CUserData &roUserData);

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
	GetWorldBossTimeLeftArg m_oArg;
	GetWorldBossTimeLeftRes m_oRes;
};

#endif
