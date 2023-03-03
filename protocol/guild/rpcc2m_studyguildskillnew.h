#ifndef __RPCC2M_STUDYGUILDSKILLNEW_H__
#define __RPCC2M_STUDYGUILDSKILLNEW_H__

// generate by ProtoGen at date: 2016/9/8 16:42:42

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_STUDYGUILDSKILLNEW_TYPE 45669

class RpcC2M_StudyGuildSkillNew : public CRpc
{
private:

	RpcC2M_StudyGuildSkillNew():CRpc(RPCC2M_STUDYGUILDSKILLNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_StudyGuildSkillNew *CreateRpc()
	{
		return new RpcC2M_StudyGuildSkillNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const StudyGuildSkillArg &roArg, const StudyGuildSkillRes &roRes, const CUserData &roUserData);
	void OnTimeout(const StudyGuildSkillArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const StudyGuildSkillArg &roArg, StudyGuildSkillRes &roRes);
	void OnDelayReplyRpc(const StudyGuildSkillArg &roArg, StudyGuildSkillRes &roRes, const CUserData &roUserData);

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
	StudyGuildSkillArg m_oArg;
	StudyGuildSkillRes m_oRes;
};

#endif
