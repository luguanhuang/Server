#ifndef __RPCC2M_MENTORMYBEAPPLIEDMSG_H__
#define __RPCC2M_MENTORMYBEAPPLIEDMSG_H__

// generate by ProtoGen at date: 2016/12/9 22:40:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_MENTORMYBEAPPLIEDMSG_TYPE 45205

class RpcC2M_MentorMyBeAppliedMsg : public CRpc
{
private:

	RpcC2M_MentorMyBeAppliedMsg():CRpc(RPCC2M_MENTORMYBEAPPLIEDMSG_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_MentorMyBeAppliedMsg *CreateRpc()
	{
		return new RpcC2M_MentorMyBeAppliedMsg;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const MentorMyBeAppliedMsgArg &roArg, const MentorMyBeAppliedMsgRes &roRes, const CUserData &roUserData);
	void OnTimeout(const MentorMyBeAppliedMsgArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const MentorMyBeAppliedMsgArg &roArg, MentorMyBeAppliedMsgRes &roRes);
	void OnDelayReplyRpc(const MentorMyBeAppliedMsgArg &roArg, MentorMyBeAppliedMsgRes &roRes, const CUserData &roUserData);

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
	MentorMyBeAppliedMsgArg m_oArg;
	MentorMyBeAppliedMsgRes m_oRes;
};

#endif
