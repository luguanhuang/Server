#ifndef __RPCC2M_GETPARTNERDETAILINFO_H__
#define __RPCC2M_GETPARTNERDETAILINFO_H__

// generate by ProtoGen at date: 2016/12/12 0:15:52

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETPARTNERDETAILINFO_TYPE 31275

class RpcC2M_GetPartnerDetailInfo : public CRpc
{
private:

	RpcC2M_GetPartnerDetailInfo():CRpc(RPCC2M_GETPARTNERDETAILINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetPartnerDetailInfo *CreateRpc()
	{
		return new RpcC2M_GetPartnerDetailInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetPartnerDetailInfoArg &roArg, const GetPartnerDetailInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetPartnerDetailInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetPartnerDetailInfoArg &roArg, GetPartnerDetailInfoRes &roRes);
	void OnDelayReplyRpc(const GetPartnerDetailInfoArg &roArg, GetPartnerDetailInfoRes &roRes, const CUserData &roUserData);

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
	GetPartnerDetailInfoArg m_oArg;
	GetPartnerDetailInfoRes m_oRes;
};

#endif
