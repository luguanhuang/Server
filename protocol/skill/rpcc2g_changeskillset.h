#ifndef __RPCC2G_CHANGESKILLSET_H__
#define __RPCC2G_CHANGESKILLSET_H__

// generate by ProtoGen at date: 2017/2/3 21:24:45

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_CHANGESKILLSET_TYPE 51116

class RpcC2G_ChangeSkillSet : public CRpc
{
private:

	RpcC2G_ChangeSkillSet():CRpc(RPCC2G_CHANGESKILLSET_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ChangeSkillSet *CreateRpc()
	{
		return new RpcC2G_ChangeSkillSet;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChangeSkillSetArg &roArg, const ChangeSkillSetRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChangeSkillSetArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChangeSkillSetArg &roArg, ChangeSkillSetRes &roRes);
	void OnDelayReplyRpc(const ChangeSkillSetArg &roArg, ChangeSkillSetRes &roRes, const CUserData &roUserData);

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
	ChangeSkillSetArg m_oArg;
	ChangeSkillSetRes m_oRes;
};

#endif
