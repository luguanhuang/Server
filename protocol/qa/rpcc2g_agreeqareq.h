#ifndef __RPCC2G_AGREEQAREQ_H__
#define __RPCC2G_AGREEQAREQ_H__

// generate by ProtoGen at date: 2016/7/6 21:15:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_AGREEQAREQ_TYPE 43200

class RpcC2G_AgreeQAReq : public CRpc
{
private:

	RpcC2G_AgreeQAReq():CRpc(RPCC2G_AGREEQAREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_AgreeQAReq *CreateRpc()
	{
		return new RpcC2G_AgreeQAReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AgreeQAReq &roArg, const AgreeQARes &roRes, const CUserData &roUserData);
	void OnTimeout(const AgreeQAReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AgreeQAReq &roArg, AgreeQARes &roRes);
	void OnDelayReplyRpc(const AgreeQAReq &roArg, AgreeQARes &roRes, const CUserData &roUserData);

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
	AgreeQAReq m_oArg;
	AgreeQARes m_oRes;
};

#endif
