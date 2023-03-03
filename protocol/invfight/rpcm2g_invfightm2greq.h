#ifndef __RPCM2G_INVFIGHTM2GREQ_H__
#define __RPCM2G_INVFIGHTM2GREQ_H__

// generate by ProtoGen at date: 2016/12/15 16:49:02

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_INVFIGHTM2GREQ_TYPE 53169

class RpcM2G_InvFightM2GReq : public CRpc
{
private:

	RpcM2G_InvFightM2GReq():CRpc(RPCM2G_INVFIGHTM2GREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_InvFightM2GReq *CreateRpc()
	{
		return new RpcM2G_InvFightM2GReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const InvFightM2GArg &roArg, const InvFightM2GRes &roRes, const CUserData &roUserData);
	void OnTimeout(const InvFightM2GArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const InvFightM2GArg &roArg, InvFightM2GRes &roRes);
	void OnDelayReplyRpc(const InvFightM2GArg &roArg, InvFightM2GRes &roRes, const CUserData &roUserData);

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
	InvFightM2GArg m_oArg;
	InvFightM2GRes m_oRes;
};

#endif
