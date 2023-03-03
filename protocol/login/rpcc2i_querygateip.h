#ifndef __RPCC2I_QUERYGATEIP_H__
#define __RPCC2I_QUERYGATEIP_H__

// generate by ProtoGen at date: 2016/3/17 17:55:18

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2I_QUERYGATEIP_TYPE 9179

class RpcC2I_QueryGateIP : public CRpc
{
private:

	RpcC2I_QueryGateIP():CRpc(RPCC2I_QUERYGATEIP_TYPE)
	{
        m_dwTimeout = 15000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2I_QueryGateIP *CreateRpc()
	{
		return new RpcC2I_QueryGateIP;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QueryGateArg &roArg, const QueryGateRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QueryGateArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QueryGateArg &roArg, QueryGateRes &roRes);
	void OnDelayReplyRpc(const QueryGateArg &roArg, QueryGateRes &roRes, const CUserData &roUserData);

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
	QueryGateArg m_oArg;
	QueryGateRes m_oRes;
};

#endif
