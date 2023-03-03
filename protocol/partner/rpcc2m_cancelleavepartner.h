#ifndef __RPCC2M_CANCELLEAVEPARTNER_H__
#define __RPCC2M_CANCELLEAVEPARTNER_H__

// generate by ProtoGen at date: 2016/12/12 15:47:10

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_CANCELLEAVEPARTNER_TYPE 27794

class RpcC2M_CancelLeavePartner : public CRpc
{
private:

	RpcC2M_CancelLeavePartner():CRpc(RPCC2M_CANCELLEAVEPARTNER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_CancelLeavePartner *CreateRpc()
	{
		return new RpcC2M_CancelLeavePartner;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CancelLeavePartnerArg &roArg, const CancelLeavePartnerRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CancelLeavePartnerArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CancelLeavePartnerArg &roArg, CancelLeavePartnerRes &roRes);
	void OnDelayReplyRpc(const CancelLeavePartnerArg &roArg, CancelLeavePartnerRes &roRes, const CUserData &roUserData);

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
	CancelLeavePartnerArg m_oArg;
	CancelLeavePartnerRes m_oRes;
};

#endif
