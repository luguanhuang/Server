#ifndef __RPCC2M_GETQADATAREQ_H__
#define __RPCC2M_GETQADATAREQ_H__

// generate by ProtoGen at date: 2016/10/9 16:27:30

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETQADATAREQ_TYPE 26871

class RpcC2M_GetQADataReq : public CRpc
{
private:

	RpcC2M_GetQADataReq():CRpc(RPCC2M_GETQADATAREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetQADataReq *CreateRpc()
	{
		return new RpcC2M_GetQADataReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetQADataReq &roArg, const GetQADataRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetQADataReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetQADataReq &roArg, GetQADataRes &roRes);
	void OnDelayReplyRpc(const GetQADataReq &roArg, GetQADataRes &roRes, const CUserData &roUserData);

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
	GetQADataReq m_oArg;
	GetQADataRes m_oRes;
};

#endif
