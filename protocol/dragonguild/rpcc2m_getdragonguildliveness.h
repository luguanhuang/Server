#ifndef __RPCC2M_GETDRAGONGUILDLIVENESS_H__
#define __RPCC2M_GETDRAGONGUILDLIVENESS_H__

// generate by ProtoGen at date: 2017/9/5 14:50:34

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETDRAGONGUILDLIVENESS_TYPE 16507

class RpcC2M_GetDragonGuildLiveness : public CRpc
{
private:

	RpcC2M_GetDragonGuildLiveness():CRpc(RPCC2M_GETDRAGONGUILDLIVENESS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetDragonGuildLiveness *CreateRpc()
	{
		return new RpcC2M_GetDragonGuildLiveness;
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
