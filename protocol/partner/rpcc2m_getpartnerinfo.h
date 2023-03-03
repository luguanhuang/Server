#ifndef __RPCC2M_GETPARTNERINFO_H__
#define __RPCC2M_GETPARTNERINFO_H__

// generate by ProtoGen at date: 2016/12/24 15:19:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GETPARTNERINFO_TYPE 61123

class RpcC2M_GetPartnerInfo : public CRpc
{
private:

	RpcC2M_GetPartnerInfo():CRpc(RPCC2M_GETPARTNERINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GetPartnerInfo *CreateRpc()
	{
		return new RpcC2M_GetPartnerInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetPartnerInfoArg &roArg, const GetPartnerInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetPartnerInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetPartnerInfoArg &roArg, GetPartnerInfoRes &roRes);
	void OnDelayReplyRpc(const GetPartnerInfoArg &roArg, GetPartnerInfoRes &roRes, const CUserData &roUserData);

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
	GetPartnerInfoArg m_oArg;
	GetPartnerInfoRes m_oRes;
};

#endif
