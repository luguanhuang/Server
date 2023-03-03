#ifndef __RPCC2M_REQGUILDBOSSINFO_H__
#define __RPCC2M_REQGUILDBOSSINFO_H__

// generate by ProtoGen at date: 2016/9/5 16:23:13

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_REQGUILDBOSSINFO_TYPE 38917

class RpcC2M_ReqGuildBossInfo : public CRpc
{
private:

	RpcC2M_ReqGuildBossInfo():CRpc(RPCC2M_REQGUILDBOSSINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ReqGuildBossInfo *CreateRpc()
	{
		return new RpcC2M_ReqGuildBossInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AskGuildBossInfoArg &roArg, const AskGuildBossInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AskGuildBossInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AskGuildBossInfoArg &roArg, AskGuildBossInfoRes &roRes);
	void OnDelayReplyRpc(const AskGuildBossInfoArg &roArg, AskGuildBossInfoRes &roRes, const CUserData &roUserData);

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
	AskGuildBossInfoArg m_oArg;
	AskGuildBossInfoRes m_oRes;
};

#endif
