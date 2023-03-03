#ifndef __RPCC2M_GETOTHERMENTORSTATUS_H__
#define __RPCC2M_GETOTHERMENTORSTATUS_H__

// generate by ProtoGen at date: 2016/12/9 17:05:33

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETOTHERMENTORSTATUS_TYPE 4896

class RpcC2M_GetOtherMentorStatus : public CRpc
{
private:

	RpcC2M_GetOtherMentorStatus():CRpc(RPCC2M_GETOTHERMENTORSTATUS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetOtherMentorStatus *CreateRpc()
	{
		return new RpcC2M_GetOtherMentorStatus;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetOtherMentorStatusArg &roArg, const GetOtherMentorStatusRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetOtherMentorStatusArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetOtherMentorStatusArg &roArg, GetOtherMentorStatusRes &roRes);
	void OnDelayReplyRpc(const GetOtherMentorStatusArg &roArg, GetOtherMentorStatusRes &roRes, const CUserData &roUserData);

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
	GetOtherMentorStatusArg m_oArg;
	GetOtherMentorStatusRes m_oRes;
};

#endif
