#ifndef __RPCC2G_GETGUILDQADATAREQ_H__
#define __RPCC2G_GETGUILDQADATAREQ_H__

// generate by ProtoGen at date: 2016/7/9 14:33:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETGUILDQADATAREQ_TYPE 35568

class RpcC2G_GetGuildQADataReq : public CRpc
{
private:

	RpcC2G_GetGuildQADataReq():CRpc(RPCC2G_GETGUILDQADATAREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetGuildQADataReq *CreateRpc()
	{
		return new RpcC2G_GetGuildQADataReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGuildQADataReq &roArg, const GetGuildQADataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGuildQADataReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGuildQADataReq &roArg, GetGuildQADataRes &roRes);
	void OnDelayReplyRpc(const GetGuildQADataReq &roArg, GetGuildQADataRes &roRes, const CUserData &roUserData);

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
	GetGuildQADataReq m_oArg;
	GetGuildQADataRes m_oRes;
};

#endif
