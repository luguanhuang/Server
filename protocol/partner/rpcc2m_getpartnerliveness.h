#ifndef __RPCC2M_GETPARTNERLIVENESS_H__
#define __RPCC2M_GETPARTNERLIVENESS_H__

// generate by ProtoGen at date: 2016/12/15 20:15:56

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETPARTNERLIVENESS_TYPE 18784

class RpcC2M_GetPartnerLiveness : public CRpc
{
private:

	RpcC2M_GetPartnerLiveness():CRpc(RPCC2M_GETPARTNERLIVENESS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetPartnerLiveness *CreateRpc()
	{
		return new RpcC2M_GetPartnerLiveness;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetPartnerLivenessArg &roArg, const GetPartnerLivenessRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetPartnerLivenessArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetPartnerLivenessArg &roArg, GetPartnerLivenessRes &roRes);
	void OnDelayReplyRpc(const GetPartnerLivenessArg &roArg, GetPartnerLivenessRes &roRes, const CUserData &roUserData);

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
	GetPartnerLivenessArg m_oArg;
	GetPartnerLivenessRes m_oRes;
};

#endif
