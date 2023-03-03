#ifndef __RPCC2G_FETCHGUILDLIST_H__
#define __RPCC2G_FETCHGUILDLIST_H__

// generate by ProtoGen at date: 2015/9/28 14:47:00

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_FETCHGUILDLIST_TYPE 41432

class RpcC2G_FetchGuildList : public CRpc
{
private:

	RpcC2G_FetchGuildList():CRpc(RPCC2G_FETCHGUILDLIST_TYPE)
	{
        m_dwTimeout = 10000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_FetchGuildList *CreateRpc()
	{
		return new RpcC2G_FetchGuildList;
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
