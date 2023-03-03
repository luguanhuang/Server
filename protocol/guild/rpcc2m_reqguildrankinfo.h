#ifndef __RPCC2M_REQGUILDRANKINFO_H__
#define __RPCC2M_REQGUILDRANKINFO_H__

// generate by ProtoGen at date: 2016/10/10 17:23:35

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_REQGUILDRANKINFO_TYPE 48521

class RpcC2M_ReqGuildRankInfo : public CRpc
{
private:

	RpcC2M_ReqGuildRankInfo():CRpc(RPCC2M_REQGUILDRANKINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ReqGuildRankInfo *CreateRpc()
	{
		return new RpcC2M_ReqGuildRankInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqGuildRankInfoArg &roArg, const ReqGuildRankInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqGuildRankInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqGuildRankInfoArg &roArg, ReqGuildRankInfoRes &roRes);
	void OnDelayReplyRpc(const ReqGuildRankInfoArg &roArg, ReqGuildRankInfoRes &roRes, const CUserData &roUserData);

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
	ReqGuildRankInfoArg m_oArg;
	ReqGuildRankInfoRes m_oRes;
};

#endif
