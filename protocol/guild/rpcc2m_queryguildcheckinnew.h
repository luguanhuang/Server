#ifndef __RPCC2M_QUERYGUILDCHECKINNEW_H__
#define __RPCC2M_QUERYGUILDCHECKINNEW_H__

// generate by ProtoGen at date: 2016/9/16 21:06:11

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_QUERYGUILDCHECKINNEW_TYPE 56433

class RpcC2M_QueryGuildCheckinNew : public CRpc
{
private:

	RpcC2M_QueryGuildCheckinNew():CRpc(RPCC2M_QUERYGUILDCHECKINNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_QueryGuildCheckinNew *CreateRpc()
	{
		return new RpcC2M_QueryGuildCheckinNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QueryGuildCheckinArg &roArg, const QueryGuildCheckinRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QueryGuildCheckinArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QueryGuildCheckinArg &roArg, QueryGuildCheckinRes &roRes);
	void OnDelayReplyRpc(const QueryGuildCheckinArg &roArg, QueryGuildCheckinRes &roRes, const CUserData &roUserData);

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
	QueryGuildCheckinArg m_oArg;
	QueryGuildCheckinRes m_oRes;
};

#endif
