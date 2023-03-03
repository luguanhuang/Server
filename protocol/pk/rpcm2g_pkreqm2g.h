#ifndef __RPCM2G_PKREQM2G_H__
#define __RPCM2G_PKREQM2G_H__

// generate by ProtoGen at date: 2016/8/25 20:15:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_PKREQM2G_TYPE 14445

class RpcM2G_PkReqM2G : public CRpc
{
private:

	RpcM2G_PkReqM2G():CRpc(RPCM2G_PKREQM2G_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_PkReqM2G *CreateRpc()
	{
		return new RpcM2G_PkReqM2G;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PkReqM2GArg &roArg, const PkReqM2GRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PkReqM2GArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PkReqM2GArg &roArg, PkReqM2GRes &roRes);
	void OnDelayReplyRpc(const PkReqM2GArg &roArg, PkReqM2GRes &roRes, const CUserData &roUserData);

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
	PkReqM2GArg m_oArg;
	PkReqM2GRes m_oRes;
};

#endif
