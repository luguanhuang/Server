#ifndef __RPCC2M_ASKGUILDSKILLINFONEW_H__
#define __RPCC2M_ASKGUILDSKILLINFONEW_H__

// generate by ProtoGen at date: 2016/9/8 16:31:16

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ASKGUILDSKILLINFONEW_TYPE 35479

class RpcC2M_AskGuildSkillInfoNew : public CRpc
{
private:

	RpcC2M_AskGuildSkillInfoNew():CRpc(RPCC2M_ASKGUILDSKILLINFONEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_AskGuildSkillInfoNew *CreateRpc()
	{
		return new RpcC2M_AskGuildSkillInfoNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AskGuildSkillInfoArg &roArg, const AskGuildSkillInfoReq &roRes, const CUserData &roUserData);
	void OnTimeout(const AskGuildSkillInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AskGuildSkillInfoArg &roArg, AskGuildSkillInfoReq &roRes);
	void OnDelayReplyRpc(const AskGuildSkillInfoArg &roArg, AskGuildSkillInfoReq &roRes, const CUserData &roUserData);

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
	AskGuildSkillInfoArg m_oArg;
	AskGuildSkillInfoReq m_oRes;
};

#endif
