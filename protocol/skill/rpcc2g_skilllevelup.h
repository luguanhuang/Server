#ifndef __RPCC2G_SKILLLEVELUP_H__
#define __RPCC2G_SKILLLEVELUP_H__

// generate by ProtoGen at date: 2017/10/10 16:00:55

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SKILLLEVELUP_TYPE 63698

class RpcC2G_SkillLevelup : public CRpc
{
private:

	RpcC2G_SkillLevelup():CRpc(RPCC2G_SKILLLEVELUP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SkillLevelup *CreateRpc()
	{
		return new RpcC2G_SkillLevelup;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SkillLevelupArg &roArg, const SkillLevelupRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SkillLevelupArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SkillLevelupArg &roArg, SkillLevelupRes &roRes);
	void OnDelayReplyRpc(const SkillLevelupArg &roArg, SkillLevelupRes &roRes, const CUserData &roUserData);

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
	SkillLevelupArg m_oArg;
	SkillLevelupRes m_oRes;
};

#endif
