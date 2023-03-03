#ifndef __RPCM2G_NPCFLREQM2G_H__
#define __RPCM2G_NPCFLREQM2G_H__

// generate by ProtoGen at date: 2017/9/11 10:37:15

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_NPCFLREQM2G_TYPE 3920

class RpcM2G_NpcFlReqM2G : public CRpc
{
private:

	RpcM2G_NpcFlReqM2G():CRpc(RPCM2G_NPCFLREQM2G_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_NpcFlReqM2G *CreateRpc()
	{
		return new RpcM2G_NpcFlReqM2G;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const NpcFlArgM2G &roArg, const NpcFlResM2G &roRes, const CUserData &roUserData);
	void OnTimeout(const NpcFlArgM2G &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const NpcFlArgM2G &roArg, NpcFlResM2G &roRes);
	void OnDelayReplyRpc(const NpcFlArgM2G &roArg, NpcFlResM2G &roRes, const CUserData &roUserData);

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
	NpcFlArgM2G m_oArg;
	NpcFlResM2G m_oRes;
};

#endif
