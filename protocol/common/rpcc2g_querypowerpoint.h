#ifndef __RPCC2G_QUERYPOWERPOINT_H__
#define __RPCC2G_QUERYPOWERPOINT_H__

// generate by ProtoGen at date: 2016/8/8 21:01:12

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_QUERYPOWERPOINT_TYPE 44381

class RpcC2G_QueryPowerPoint : public CRpc
{
private:

	RpcC2G_QueryPowerPoint():CRpc(RPCC2G_QUERYPOWERPOINT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_QueryPowerPoint *CreateRpc()
	{
		return new RpcC2G_QueryPowerPoint;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QueryPowerPointArg &roArg, const QueryPowerPointRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QueryPowerPointArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QueryPowerPointArg &roArg, QueryPowerPointRes &roRes);
	void OnDelayReplyRpc(const QueryPowerPointArg &roArg, QueryPowerPointRes &roRes, const CUserData &roUserData);

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
	QueryPowerPointArg m_oArg;
	QueryPowerPointRes m_oRes;
};

#endif
