#ifndef __RPCM2N_KICKACCOUNTFROMMS_H__
#define __RPCM2N_KICKACCOUNTFROMMS_H__

// generate by ProtoGen at date: 2016/11/4 17:38:43

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2N_KICKACCOUNTFROMMS_TYPE 32648

class RpcM2N_KickAccountFromMs : public CRpc
{
private:

	RpcM2N_KickAccountFromMs():CRpc(RPCM2N_KICKACCOUNTFROMMS_TYPE)
	{
        m_dwTimeout = 50000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2N_KickAccountFromMs *CreateRpc()
	{
		return new RpcM2N_KickAccountFromMs;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const KickAccountFromMsArg &roArg, const KickAccountFromMsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const KickAccountFromMsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const KickAccountFromMsArg &roArg, KickAccountFromMsRes &roRes);
	void OnDelayReplyRpc(const KickAccountFromMsArg &roArg, KickAccountFromMsRes &roRes, const CUserData &roUserData);

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
	KickAccountFromMsArg m_oArg;
	KickAccountFromMsRes m_oRes;
};

#endif
