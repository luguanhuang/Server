#ifndef __RPCC2G_BINDSKILL_H__
#define __RPCC2G_BINDSKILL_H__

// generate by ProtoGen at date: 2015/4/14 16:22:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BINDSKILL_TYPE 48236

class RpcC2G_BindSkill : public CRpc
{
private:

	RpcC2G_BindSkill():CRpc(RPCC2G_BINDSKILL_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_BindSkill *CreateRpc()
	{
		return new RpcC2G_BindSkill;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BingSkillArg &roArg, const BindSkillRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BingSkillArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BingSkillArg &roArg, BindSkillRes &roRes);
	void OnDelayReplyRpc(const BingSkillArg &roArg, BindSkillRes &roRes, const CUserData &roUserData);

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
	BingSkillArg m_oArg;
	BindSkillRes m_oRes;
};

#endif
