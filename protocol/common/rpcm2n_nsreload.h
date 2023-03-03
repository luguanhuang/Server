#ifndef __RPCM2N_NSRELOAD_H__
#define __RPCM2N_NSRELOAD_H__

// generate by ProtoGen at date: 2016/11/18 15:04:39

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2N_NSRELOAD_TYPE 36875

class RpcM2N_NsReload : public CRpc
{
private:

	RpcM2N_NsReload():CRpc(RPCM2N_NSRELOAD_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2N_NsReload *CreateRpc()
	{
		return new RpcM2N_NsReload;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GMToolCommandArg &roArg, const GMToolCommandRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GMToolCommandArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GMToolCommandArg &roArg, GMToolCommandRes &roRes);
	void OnDelayReplyRpc(const GMToolCommandArg &roArg, GMToolCommandRes &roRes, const CUserData &roUserData);

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
	GMToolCommandArg m_oArg;
	GMToolCommandRes m_oRes;
};

#endif
