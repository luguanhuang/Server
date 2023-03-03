#ifndef __RPCC2M_REQGUILDLIST_H__
#define __RPCC2M_REQGUILDLIST_H__

// generate by ProtoGen at date: 2016/9/1 11:04:39

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_REQGUILDLIST_TYPE 46835

class RpcC2M_ReqGuildList : public CRpc
{
private:

	RpcC2M_ReqGuildList():CRpc(RPCC2M_REQGUILDLIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ReqGuildList *CreateRpc()
	{
		return new RpcC2M_ReqGuildList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FetchGuildListArg &roArg, const FetchGuildListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FetchGuildListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FetchGuildListArg &roArg, FetchGuildListRes &roRes);
	void OnDelayReplyRpc(const FetchGuildListArg &roArg, FetchGuildListRes &roRes, const CUserData &roUserData);

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
	FetchGuildListArg m_oArg;
	FetchGuildListRes m_oRes;
};

#endif
