#ifndef __RPCC2M_LEAVEPARTNER_H__
#define __RPCC2M_LEAVEPARTNER_H__

// generate by ProtoGen at date: 2016/12/12 11:09:40

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_LEAVEPARTNER_TYPE 63769

class RpcC2M_LeavePartner : public CRpc
{
private:

	RpcC2M_LeavePartner():CRpc(RPCC2M_LEAVEPARTNER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_LeavePartner *CreateRpc()
	{
		return new RpcC2M_LeavePartner;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LeavePartnerArg &roArg, const LeavePartnerRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LeavePartnerArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LeavePartnerArg &roArg, LeavePartnerRes &roRes);
	void OnDelayReplyRpc(const LeavePartnerArg &roArg, LeavePartnerRes &roRes, const CUserData &roUserData);

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
	LeavePartnerArg m_oArg;
	LeavePartnerRes m_oRes;
};

#endif
