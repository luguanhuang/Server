#ifndef __RPCC2G_DOODADITEMALLSKILLREQ_H__
#define __RPCC2G_DOODADITEMALLSKILLREQ_H__

// generate by ProtoGen at date: 2017/9/23 15:56:58

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_DOODADITEMALLSKILLREQ_TYPE 21002

class RpcC2G_DoodadItemAllSkillReq : public CRpc
{
private:

	RpcC2G_DoodadItemAllSkillReq():CRpc(RPCC2G_DOODADITEMALLSKILLREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_DoodadItemAllSkillReq *CreateRpc()
	{
		return new RpcC2G_DoodadItemAllSkillReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EmptyData &roArg, const DoodadItemAllSkill &roRes, const CUserData &roUserData);
	void OnTimeout(const EmptyData &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EmptyData &roArg, DoodadItemAllSkill &roRes);
	void OnDelayReplyRpc(const EmptyData &roArg, DoodadItemAllSkill &roRes, const CUserData &roUserData);

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
	EmptyData m_oArg;
	DoodadItemAllSkill m_oRes;
};

#endif
