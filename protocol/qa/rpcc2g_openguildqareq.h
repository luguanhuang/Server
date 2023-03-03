#ifndef __RPCC2G_OPENGUILDQAREQ_H__
#define __RPCC2G_OPENGUILDQAREQ_H__

// generate by ProtoGen at date: 2016/7/9 14:31:43

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_OPENGUILDQAREQ_TYPE 62840

class RpcC2G_OpenGuildQAReq : public CRpc
{
private:

	RpcC2G_OpenGuildQAReq():CRpc(RPCC2G_OPENGUILDQAREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_OpenGuildQAReq *CreateRpc()
	{
		return new RpcC2G_OpenGuildQAReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const OpenGuildQAReq &roArg, const OpenGuildQARes &roRes, const CUserData &roUserData);
	void OnTimeout(const OpenGuildQAReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const OpenGuildQAReq &roArg, OpenGuildQARes &roRes);
	void OnDelayReplyRpc(const OpenGuildQAReq &roArg, OpenGuildQARes &roRes, const CUserData &roUserData);

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
	OpenGuildQAReq m_oArg;
	OpenGuildQARes m_oRes;
};

#endif
