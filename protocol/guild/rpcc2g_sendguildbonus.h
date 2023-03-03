#ifndef __RPCC2G_SENDGUILDBONUS_H__
#define __RPCC2G_SENDGUILDBONUS_H__

// generate by ProtoGen at date: 2016/6/16 14:25:50

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SENDGUILDBONUS_TYPE 61243

class RpcC2G_SendGuildBonus : public CRpc
{
private:

	RpcC2G_SendGuildBonus():CRpc(RPCC2G_SENDGUILDBONUS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SendGuildBonus *CreateRpc()
	{
		return new RpcC2G_SendGuildBonus;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SendGuildBonusArg &roArg, const SendGuildBonusRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SendGuildBonusArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SendGuildBonusArg &roArg, SendGuildBonusRes &roRes);
	void OnDelayReplyRpc(const SendGuildBonusArg &roArg, SendGuildBonusRes &roRes, const CUserData &roUserData);

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
	SendGuildBonusArg m_oArg;
	SendGuildBonusRes m_oRes;
};

#endif
