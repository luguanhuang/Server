#ifndef __RPCC2G_REPLYPARTYEXCHANGEITEMOPT_H__
#define __RPCC2G_REPLYPARTYEXCHANGEITEMOPT_H__

// generate by ProtoGen at date: 2017/5/6 18:50:33

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_REPLYPARTYEXCHANGEITEMOPT_TYPE 13740

class RpcC2G_ReplyPartyExchangeItemOpt : public CRpc
{
private:

	RpcC2G_ReplyPartyExchangeItemOpt():CRpc(RPCC2G_REPLYPARTYEXCHANGEITEMOPT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ReplyPartyExchangeItemOpt *CreateRpc()
	{
		return new RpcC2G_ReplyPartyExchangeItemOpt;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReplyPartyExchangeItemOptArg &roArg, const ReplyPartyExchangeItemOptRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReplyPartyExchangeItemOptArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReplyPartyExchangeItemOptArg &roArg, ReplyPartyExchangeItemOptRes &roRes);
	void OnDelayReplyRpc(const ReplyPartyExchangeItemOptArg &roArg, ReplyPartyExchangeItemOptRes &roRes, const CUserData &roUserData);

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
	ReplyPartyExchangeItemOptArg m_oArg;
	ReplyPartyExchangeItemOptRes m_oRes;
};

#endif
