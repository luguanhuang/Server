#ifndef __RPCM2S_M2SLOGINANDOUTSYNC_H__
#define __RPCM2S_M2SLOGINANDOUTSYNC_H__

// generate by ProtoGen at date: 2016/10/26 15:44:39

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2S_M2SLOGINANDOUTSYNC_TYPE 9003

class RpcM2S_M2SLoginAndoutSync : public CRpc
{
private:

	RpcM2S_M2SLoginAndoutSync():CRpc(RPCM2S_M2SLOGINANDOUTSYNC_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2S_M2SLoginAndoutSync *CreateRpc()
	{
		return new RpcM2S_M2SLoginAndoutSync;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const M2SLoginAndoutSyncArg &roArg, const M2SLoginAndoutSyncRes &roRes, const CUserData &roUserData);
	void OnTimeout(const M2SLoginAndoutSyncArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const M2SLoginAndoutSyncArg &roArg, M2SLoginAndoutSyncRes &roRes);
	void OnDelayReplyRpc(const M2SLoginAndoutSyncArg &roArg, M2SLoginAndoutSyncRes &roRes, const CUserData &roUserData);

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
	M2SLoginAndoutSyncArg m_oArg;
	M2SLoginAndoutSyncRes m_oRes;
};

#endif
