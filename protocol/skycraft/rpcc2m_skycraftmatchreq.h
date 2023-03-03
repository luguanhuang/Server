#ifndef __RPCC2M_SKYCRAFTMATCHREQ_H__
#define __RPCC2M_SKYCRAFTMATCHREQ_H__

// generate by ProtoGen at date: 2017/4/1 11:56:04

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_SKYCRAFTMATCHREQ_TYPE 26016

class RpcC2M_SkyCraftMatchReq : public CRpc
{
private:

	RpcC2M_SkyCraftMatchReq():CRpc(RPCC2M_SKYCRAFTMATCHREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_SkyCraftMatchReq *CreateRpc()
	{
		return new RpcC2M_SkyCraftMatchReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SkyCraftMatchReq &roArg, const SkyCraftMatchRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SkyCraftMatchReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SkyCraftMatchReq &roArg, SkyCraftMatchRes &roRes);
	void OnDelayReplyRpc(const SkyCraftMatchReq &roArg, SkyCraftMatchRes &roRes, const CUserData &roUserData);

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
	SkyCraftMatchReq m_oArg;
	SkyCraftMatchRes m_oRes;
};

#endif
