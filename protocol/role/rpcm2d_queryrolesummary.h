#ifndef __RPCM2D_QUERYROLESUMMARY_H__
#define __RPCM2D_QUERYROLESUMMARY_H__

// generate by ProtoGen at date: 2017/4/26 19:27:56

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2D_QUERYROLESUMMARY_TYPE 32686

class RpcM2D_QueryRolesummary : public CRpc
{
private:

	RpcM2D_QueryRolesummary():CRpc(RPCM2D_QUERYROLESUMMARY_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2D_QueryRolesummary *CreateRpc()
	{
		return new RpcM2D_QueryRolesummary;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QueryRolesummaryArg &roArg, const QueryRolesummaryRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QueryRolesummaryArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QueryRolesummaryArg &roArg, QueryRolesummaryRes &roRes);
	void OnDelayReplyRpc(const QueryRolesummaryArg &roArg, QueryRolesummaryRes &roRes, const CUserData &roUserData);

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
	QueryRolesummaryArg m_oArg;
	QueryRolesummaryRes m_oRes;
};

#endif
