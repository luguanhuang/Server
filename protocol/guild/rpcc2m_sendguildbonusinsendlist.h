#ifndef __RPCC2M_SENDGUILDBONUSINSENDLIST_H__
#define __RPCC2M_SENDGUILDBONUSINSENDLIST_H__

// generate by ProtoGen at date: 2016/11/2 10:28:20

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_SENDGUILDBONUSINSENDLIST_TYPE 64498

class RpcC2M_SendGuildBonusInSendList : public CRpc
{
private:

	RpcC2M_SendGuildBonusInSendList():CRpc(RPCC2M_SENDGUILDBONUSINSENDLIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_SendGuildBonusInSendList *CreateRpc()
	{
		return new RpcC2M_SendGuildBonusInSendList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SendGuildBonusInSendListArg &roArg, const SendGuildBonusInSendListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SendGuildBonusInSendListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SendGuildBonusInSendListArg &roArg, SendGuildBonusInSendListRes &roRes);
	void OnDelayReplyRpc(const SendGuildBonusInSendListArg &roArg, SendGuildBonusInSendListRes &roRes, const CUserData &roUserData);

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
	SendGuildBonusInSendListArg m_oArg;
	SendGuildBonusInSendListRes m_oRes;
};

#endif
