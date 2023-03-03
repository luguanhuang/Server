#ifndef __RPCC2G_LEARNGUILDSKILL_H__
#define __RPCC2G_LEARNGUILDSKILL_H__

// generate by ProtoGen at date: 2016/9/12 11:37:12

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_LEARNGUILDSKILL_TYPE 62806

class RpcC2G_LearnGuildSkill : public CRpc
{
private:

	RpcC2G_LearnGuildSkill():CRpc(RPCC2G_LEARNGUILDSKILL_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_LearnGuildSkill *CreateRpc()
	{
		return new RpcC2G_LearnGuildSkill;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LearnGuildSkillAgr &roArg, const LearnGuildSkillRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LearnGuildSkillAgr &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LearnGuildSkillAgr &roArg, LearnGuildSkillRes &roRes);
	void OnDelayReplyRpc(const LearnGuildSkillAgr &roArg, LearnGuildSkillRes &roRes, const CUserData &roUserData);

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
	LearnGuildSkillAgr m_oArg;
	LearnGuildSkillRes m_oRes;
};

#endif
