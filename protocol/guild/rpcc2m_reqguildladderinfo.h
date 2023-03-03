#ifndef __RPCC2M_REQGUILDLADDERINFO_H__
#define __RPCC2M_REQGUILDLADDERINFO_H__

// generate by ProtoGen at date: 2016/9/30 11:18:03

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_REQGUILDLADDERINFO_TYPE 44006

class RpcC2M_ReqGuildLadderInfo : public CRpc
{
private:

	RpcC2M_ReqGuildLadderInfo():CRpc(RPCC2M_REQGUILDLADDERINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ReqGuildLadderInfo *CreateRpc()
	{
		return new RpcC2M_ReqGuildLadderInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqGuildLadderInfoAgr &roArg, const ReqGuildLadderInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqGuildLadderInfoAgr &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqGuildLadderInfoAgr &roArg, ReqGuildLadderInfoRes &roRes);
	void OnDelayReplyRpc(const ReqGuildLadderInfoAgr &roArg, ReqGuildLadderInfoRes &roRes, const CUserData &roUserData);

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
	ReqGuildLadderInfoAgr m_oArg;
	ReqGuildLadderInfoRes m_oRes;
};

#endif
