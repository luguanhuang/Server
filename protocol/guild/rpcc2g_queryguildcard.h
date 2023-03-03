#ifndef __RPCC2G_QUERYGUILDCARD_H__
#define __RPCC2G_QUERYGUILDCARD_H__

// generate by ProtoGen at date: 2015/10/2 10:42:15

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_QUERYGUILDCARD_TYPE 55524

class RpcC2G_QueryGuildCard : public CRpc
{
private:

	RpcC2G_QueryGuildCard():CRpc(RPCC2G_QUERYGUILDCARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_QueryGuildCard *CreateRpc()
	{
		return new RpcC2G_QueryGuildCard;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QueryGuildCardArg &roArg, const QueryGuildCardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QueryGuildCardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QueryGuildCardArg &roArg, QueryGuildCardRes &roRes);
	void OnDelayReplyRpc(const QueryGuildCardArg &roArg, QueryGuildCardRes &roRes, const CUserData &roUserData);

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
	QueryGuildCardArg m_oArg;
	QueryGuildCardRes m_oRes;
};

#endif
