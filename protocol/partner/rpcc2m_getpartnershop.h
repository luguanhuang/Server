#ifndef __RPCC2M_GETPARTNERSHOP_H__
#define __RPCC2M_GETPARTNERSHOP_H__

// generate by ProtoGen at date: 2016/12/15 11:36:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETPARTNERSHOP_TYPE 46131

class RpcC2M_GetPartnerShop : public CRpc
{
private:

	RpcC2M_GetPartnerShop():CRpc(RPCC2M_GETPARTNERSHOP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetPartnerShop *CreateRpc()
	{
		return new RpcC2M_GetPartnerShop;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetPartnerShopArg &roArg, const GetPartnerShopRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetPartnerShopArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetPartnerShopArg &roArg, GetPartnerShopRes &roRes);
	void OnDelayReplyRpc(const GetPartnerShopArg &roArg, GetPartnerShopRes &roRes, const CUserData &roUserData);

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
	GetPartnerShopArg m_oArg;
	GetPartnerShopRes m_oRes;
};

#endif
