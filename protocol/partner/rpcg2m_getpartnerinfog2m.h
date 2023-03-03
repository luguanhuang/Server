#ifndef __RPCG2M_GETPARTNERINFOG2M_H__
#define __RPCG2M_GETPARTNERINFOG2M_H__

// generate by ProtoGen at date: 2016/12/12 11:47:19

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_GETPARTNERINFOG2M_TYPE 40918

class RpcG2M_GetPartnerInfoG2M : public CRpc
{
private:

	RpcG2M_GetPartnerInfoG2M():CRpc(RPCG2M_GETPARTNERINFOG2M_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_GetPartnerInfoG2M *CreateRpc()
	{
		return new RpcG2M_GetPartnerInfoG2M;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetPartnerInfoG2MArg &roArg, const GetPartnerInfoG2MRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetPartnerInfoG2MArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetPartnerInfoG2MArg &roArg, GetPartnerInfoG2MRes &roRes);
	void OnDelayReplyRpc(const GetPartnerInfoG2MArg &roArg, GetPartnerInfoG2MRes &roRes, const CUserData &roUserData);

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
	GetPartnerInfoG2MArg m_oArg;
	GetPartnerInfoG2MRes m_oRes;
};

#endif
