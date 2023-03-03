#ifndef __RPCG2M_GUILDCARDSYNCMS_H__
#define __RPCG2M_GUILDCARDSYNCMS_H__

// generate by ProtoGen at date: 2016/9/17 19:33:47

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_GUILDCARDSYNCMS_TYPE 24005

class RpcG2M_GuildCardSyncMs : public CRpc
{
private:

	RpcG2M_GuildCardSyncMs():CRpc(RPCG2M_GUILDCARDSYNCMS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_GuildCardSyncMs *CreateRpc()
	{
		return new RpcG2M_GuildCardSyncMs;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildCardSyncMsArg &roArg, const GuildCardSyncMsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildCardSyncMsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildCardSyncMsArg &roArg, GuildCardSyncMsRes &roRes);
	void OnDelayReplyRpc(const GuildCardSyncMsArg &roArg, GuildCardSyncMsRes &roRes, const CUserData &roUserData);

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
	GuildCardSyncMsArg m_oArg;
	GuildCardSyncMsRes m_oRes;
};

#endif
