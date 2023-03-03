#ifndef __RPCC2M_FETCHGUILDHISTORYNEW_H__
#define __RPCC2M_FETCHGUILDHISTORYNEW_H__

// generate by ProtoGen at date: 2016/9/20 14:22:23

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_FETCHGUILDHISTORYNEW_TYPE 26284

class RpcC2M_FetchGuildHistoryNew : public CRpc
{
private:

	RpcC2M_FetchGuildHistoryNew():CRpc(RPCC2M_FETCHGUILDHISTORYNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_FetchGuildHistoryNew *CreateRpc()
	{
		return new RpcC2M_FetchGuildHistoryNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildHistoryArg &roArg, const GuildHistoryRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildHistoryArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildHistoryArg &roArg, GuildHistoryRes &roRes);
	void OnDelayReplyRpc(const GuildHistoryArg &roArg, GuildHistoryRes &roRes, const CUserData &roUserData);

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
	GuildHistoryArg m_oArg;
	GuildHistoryRes m_oRes;
};

#endif
