#ifndef __RPCC2G_GETNEWZONEBENEFIT_H__
#define __RPCC2G_GETNEWZONEBENEFIT_H__

// generate by ProtoGen at date: 2017/10/24 10:40:03

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETNEWZONEBENEFIT_TYPE 17236

class RpcC2G_GetNewZoneBenefit : public CRpc
{
private:

	RpcC2G_GetNewZoneBenefit():CRpc(RPCC2G_GETNEWZONEBENEFIT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetNewZoneBenefit *CreateRpc()
	{
		return new RpcC2G_GetNewZoneBenefit;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetNewZoneBenefitArg &roArg, const GetNewZoneBenefitRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetNewZoneBenefitArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetNewZoneBenefitArg &roArg, GetNewZoneBenefitRes &roRes);
	void OnDelayReplyRpc(const GetNewZoneBenefitArg &roArg, GetNewZoneBenefitRes &roRes, const CUserData &roUserData);

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
	GetNewZoneBenefitArg m_oArg;
	GetNewZoneBenefitRes m_oRes;
};

#endif
