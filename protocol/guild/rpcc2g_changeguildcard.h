#ifndef __RPCC2G_CHANGEGUILDCARD_H__
#define __RPCC2G_CHANGEGUILDCARD_H__

// generate by ProtoGen at date: 2015/10/2 10:43:56

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_CHANGEGUILDCARD_TYPE 55997

class RpcC2G_ChangeGuildCard : public CRpc
{
private:

	RpcC2G_ChangeGuildCard():CRpc(RPCC2G_CHANGEGUILDCARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ChangeGuildCard *CreateRpc()
	{
		return new RpcC2G_ChangeGuildCard;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChangeGuildCardArg &roArg, const ChangeGuildCardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChangeGuildCardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChangeGuildCardArg &roArg, ChangeGuildCardRes &roRes);
	void OnDelayReplyRpc(const ChangeGuildCardArg &roArg, ChangeGuildCardRes &roRes, const CUserData &roUserData);

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
	ChangeGuildCardArg m_oArg;
	ChangeGuildCardRes m_oRes;
};

#endif
