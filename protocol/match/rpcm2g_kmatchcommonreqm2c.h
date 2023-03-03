#ifndef __RPCM2G_KMATCHCOMMONREQM2C_H__
#define __RPCM2G_KMATCHCOMMONREQM2C_H__

// generate by ProtoGen at date: 2017/5/14 19:48:10

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_KMATCHCOMMONREQM2C_TYPE 54968

class RpcM2G_KMatchCommonReqM2C : public CRpc
{
private:

	RpcM2G_KMatchCommonReqM2C():CRpc(RPCM2G_KMATCHCOMMONREQM2C_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_KMatchCommonReqM2C *CreateRpc()
	{
		return new RpcM2G_KMatchCommonReqM2C;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const KMatchCommonArgM2G &roArg, const KMatchCommonResM2G &roRes, const CUserData &roUserData);
	void OnTimeout(const KMatchCommonArgM2G &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const KMatchCommonArgM2G &roArg, KMatchCommonResM2G &roRes);
	void OnDelayReplyRpc(const KMatchCommonArgM2G &roArg, KMatchCommonResM2G &roRes, const CUserData &roUserData);

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
	KMatchCommonArgM2G m_oArg;
	KMatchCommonResM2G m_oRes;
};

#endif
