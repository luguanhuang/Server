#ifndef __RPCM2G_CRASHRECOVERM2G_H__
#define __RPCM2G_CRASHRECOVERM2G_H__

// generate by ProtoGen at date: 2016/11/22 21:09:23

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_CRASHRECOVERM2G_TYPE 26149

class RpcM2G_CrashRecoverM2G : public CRpc
{
private:

	RpcM2G_CrashRecoverM2G():CRpc(RPCM2G_CRASHRECOVERM2G_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_CrashRecoverM2G *CreateRpc()
	{
		return new RpcM2G_CrashRecoverM2G;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CrashRecoverM2GArg &roArg, const CrashRecoverM2GRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CrashRecoverM2GArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CrashRecoverM2GArg &roArg, CrashRecoverM2GRes &roRes);
	void OnDelayReplyRpc(const CrashRecoverM2GArg &roArg, CrashRecoverM2GRes &roRes, const CUserData &roUserData);

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
	CrashRecoverM2GArg m_oArg;
	CrashRecoverM2GRes m_oRes;
};

#endif
