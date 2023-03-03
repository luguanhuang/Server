#ifndef __RPCM2G_MAKEPARTNERM2G_H__
#define __RPCM2G_MAKEPARTNERM2G_H__

// generate by ProtoGen at date: 2016/12/8 16:06:49

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_MAKEPARTNERM2G_TYPE 27743

class RpcM2G_MakePartnerM2G : public CRpc
{
private:

	RpcM2G_MakePartnerM2G():CRpc(RPCM2G_MAKEPARTNERM2G_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_MakePartnerM2G *CreateRpc()
	{
		return new RpcM2G_MakePartnerM2G;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const MakePartnerM2GArg &roArg, const MakePartnerM2GRes &roRes, const CUserData &roUserData);
	void OnTimeout(const MakePartnerM2GArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const MakePartnerM2GArg &roArg, MakePartnerM2GRes &roRes);
	void OnDelayReplyRpc(const MakePartnerM2GArg &roArg, MakePartnerM2GRes &roRes, const CUserData &roUserData);

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
	MakePartnerM2GArg m_oArg;
	MakePartnerM2GRes m_oRes;
};

#endif
