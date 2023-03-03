#ifndef __RPCM2G_QUERYROLECARDMATCH_H__
#define __RPCM2G_QUERYROLECARDMATCH_H__

// generate by ProtoGen at date: 2016/10/26 21:19:07

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_QUERYROLECARDMATCH_TYPE 42441

class RpcM2G_QueryRoleCardMatch : public CRpc
{
private:

	RpcM2G_QueryRoleCardMatch():CRpc(RPCM2G_QUERYROLECARDMATCH_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_QueryRoleCardMatch *CreateRpc()
	{
		return new RpcM2G_QueryRoleCardMatch;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QueryRoleCardMatchArg &roArg, const QueryRoleCardMatchRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QueryRoleCardMatchArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QueryRoleCardMatchArg &roArg, QueryRoleCardMatchRes &roRes);
	void OnDelayReplyRpc(const QueryRoleCardMatchArg &roArg, QueryRoleCardMatchRes &roRes, const CUserData &roUserData);

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
	QueryRoleCardMatchArg m_oArg;
	QueryRoleCardMatchRes m_oRes;
};

#endif
