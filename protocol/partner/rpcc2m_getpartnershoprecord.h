#ifndef __RPCC2M_GETPARTNERSHOPRECORD_H__
#define __RPCC2M_GETPARTNERSHOPRECORD_H__

// generate by ProtoGen at date: 2016/12/17 12:14:19

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETPARTNERSHOPRECORD_TYPE 56970

class RpcC2M_GetPartnerShopRecord : public CRpc
{
private:

	RpcC2M_GetPartnerShopRecord():CRpc(RPCC2M_GETPARTNERSHOPRECORD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetPartnerShopRecord *CreateRpc()
	{
		return new RpcC2M_GetPartnerShopRecord;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetPartnerShopRecordArg &roArg, const GetPartnerShopRecordRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetPartnerShopRecordArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetPartnerShopRecordArg &roArg, GetPartnerShopRecordRes &roRes);
	void OnDelayReplyRpc(const GetPartnerShopRecordArg &roArg, GetPartnerShopRecordRes &roRes, const CUserData &roUserData);

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
	GetPartnerShopRecordArg m_oArg;
	GetPartnerShopRecordRes m_oRes;
};

#endif
