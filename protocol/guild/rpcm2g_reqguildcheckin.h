#ifndef __RPCM2G_REQGUILDCHECKIN_H__
#define __RPCM2G_REQGUILDCHECKIN_H__

// generate by ProtoGen at date: 2016/9/18 10:29:18

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_REQGUILDCHECKIN_TYPE 59822

class RpcM2G_ReqGuildCheckin : public CRpc
{
private:

	RpcM2G_ReqGuildCheckin():CRpc(RPCM2G_REQGUILDCHECKIN_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_ReqGuildCheckin *CreateRpc()
	{
		return new RpcM2G_ReqGuildCheckin;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqGuildCheckinArg &roArg, const ReqGuildCheckinRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqGuildCheckinArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqGuildCheckinArg &roArg, ReqGuildCheckinRes &roRes);
	void OnDelayReplyRpc(const ReqGuildCheckinArg &roArg, ReqGuildCheckinRes &roRes, const CUserData &roUserData);

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
	ReqGuildCheckinArg m_oArg;
	ReqGuildCheckinRes m_oRes;
};

#endif
