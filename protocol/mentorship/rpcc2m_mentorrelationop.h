#ifndef __RPCC2M_MENTORRELATIONOP_H__
#define __RPCC2M_MENTORRELATIONOP_H__

// generate by ProtoGen at date: 2016/12/9 22:57:42

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_MENTORRELATIONOP_TYPE 10644

class RpcC2M_MentorRelationOp : public CRpc
{
private:

	RpcC2M_MentorRelationOp():CRpc(RPCC2M_MENTORRELATIONOP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_MentorRelationOp *CreateRpc()
	{
		return new RpcC2M_MentorRelationOp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const MentorRelationOpArg &roArg, const MentorRelationOpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const MentorRelationOpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const MentorRelationOpArg &roArg, MentorRelationOpRes &roRes);
	void OnDelayReplyRpc(const MentorRelationOpArg &roArg, MentorRelationOpRes &roRes, const CUserData &roUserData);

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
	MentorRelationOpArg m_oArg;
	MentorRelationOpRes m_oRes;
};

#endif
