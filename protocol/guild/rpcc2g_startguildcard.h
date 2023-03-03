#ifndef __RPCC2G_STARTGUILDCARD_H__
#define __RPCC2G_STARTGUILDCARD_H__

// generate by ProtoGen at date: 2015/10/2 10:40:50

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_STARTGUILDCARD_TYPE 35743

class RpcC2G_StartGuildCard : public CRpc
{
private:

	RpcC2G_StartGuildCard():CRpc(RPCC2G_STARTGUILDCARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_StartGuildCard *CreateRpc()
	{
		return new RpcC2G_StartGuildCard;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const StartGuildCardArg &roArg, const StartGuildCardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const StartGuildCardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const StartGuildCardArg &roArg, StartGuildCardRes &roRes);
	void OnDelayReplyRpc(const StartGuildCardArg &roArg, StartGuildCardRes &roRes, const CUserData &roUserData);

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
	StartGuildCardArg m_oArg;
	StartGuildCardRes m_oRes;
};

#endif
