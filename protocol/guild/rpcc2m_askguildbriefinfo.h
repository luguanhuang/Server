#ifndef __RPCC2M_ASKGUILDBRIEFINFO_H__
#define __RPCC2M_ASKGUILDBRIEFINFO_H__

// generate by ProtoGen at date: 2016/8/26 15:44:15

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ASKGUILDBRIEFINFO_TYPE 53355

class RpcC2M_AskGuildBriefInfo : public CRpc
{
private:

	RpcC2M_AskGuildBriefInfo():CRpc(RPCC2M_ASKGUILDBRIEFINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_AskGuildBriefInfo *CreateRpc()
	{
		return new RpcC2M_AskGuildBriefInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GuildBriefArg &roArg, const GuildBriefRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GuildBriefArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GuildBriefArg &roArg, GuildBriefRes &roRes);
	void OnDelayReplyRpc(const GuildBriefArg &roArg, GuildBriefRes &roRes, const CUserData &roUserData);

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
	GuildBriefArg m_oArg;
	GuildBriefRes m_oRes;
};

#endif
