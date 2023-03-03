#ifndef __RPCC2M_REQGUILDTERRCHALLINFO_H__
#define __RPCC2M_REQGUILDTERRCHALLINFO_H__

// generate by ProtoGen at date: 2016/12/16 20:22:56

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_REQGUILDTERRCHALLINFO_TYPE 9791

class RpcC2M_ReqGuildTerrChallInfo : public CRpc
{
private:

	RpcC2M_ReqGuildTerrChallInfo():CRpc(RPCC2M_REQGUILDTERRCHALLINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ReqGuildTerrChallInfo *CreateRpc()
	{
		return new RpcC2M_ReqGuildTerrChallInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqGuildTerrChallInfoArg &roArg, const ReqGuildTerrChallInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqGuildTerrChallInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqGuildTerrChallInfoArg &roArg, ReqGuildTerrChallInfoRes &roRes);
	void OnDelayReplyRpc(const ReqGuildTerrChallInfoArg &roArg, ReqGuildTerrChallInfoRes &roRes, const CUserData &roUserData);

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
	ReqGuildTerrChallInfoArg m_oArg;
	ReqGuildTerrChallInfoRes m_oRes;
};

#endif
