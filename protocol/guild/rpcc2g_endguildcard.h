#ifndef __RPCC2G_ENDGUILDCARD_H__
#define __RPCC2G_ENDGUILDCARD_H__

// generate by ProtoGen at date: 2015/10/2 10:45:21

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ENDGUILDCARD_TYPE 13212

class RpcC2G_EndGuildCard : public CRpc
{
private:

	RpcC2G_EndGuildCard():CRpc(RPCC2G_ENDGUILDCARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_EndGuildCard *CreateRpc()
	{
		return new RpcC2G_EndGuildCard;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EndGuildCardArg &roArg, const EndGuildCardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EndGuildCardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EndGuildCardArg &roArg, EndGuildCardRes &roRes);
	void OnDelayReplyRpc(const EndGuildCardArg &roArg, EndGuildCardRes &roRes, const CUserData &roUserData);

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
	EndGuildCardArg m_oArg;
	EndGuildCardRes m_oRes;
};

#endif
