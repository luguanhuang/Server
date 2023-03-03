#ifndef __RPCM2G_REQROLEGUILDSKILL_H__
#define __RPCM2G_REQROLEGUILDSKILL_H__

// generate by ProtoGen at date: 2016/9/13 19:17:09

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_REQROLEGUILDSKILL_TYPE 43832

class RpcM2G_ReqRoleGuildSkill : public CRpc
{
private:

	RpcM2G_ReqRoleGuildSkill():CRpc(RPCM2G_REQROLEGUILDSKILL_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_ReqRoleGuildSkill *CreateRpc()
	{
		return new RpcM2G_ReqRoleGuildSkill;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqRoleGuildSkillArg &roArg, const ReqRoleGuildSkillRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqRoleGuildSkillArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqRoleGuildSkillArg &roArg, ReqRoleGuildSkillRes &roRes);
	void OnDelayReplyRpc(const ReqRoleGuildSkillArg &roArg, ReqRoleGuildSkillRes &roRes, const CUserData &roUserData);

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
	ReqRoleGuildSkillArg m_oArg;
	ReqRoleGuildSkillRes m_oRes;
};

#endif
