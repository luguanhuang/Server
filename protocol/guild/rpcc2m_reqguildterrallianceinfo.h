#ifndef __RPCC2M_REQGUILDTERRALLIANCEINFO_H__
#define __RPCC2M_REQGUILDTERRALLIANCEINFO_H__

// generate by ProtoGen at date: 2016/12/15 20:38:00

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_REQGUILDTERRALLIANCEINFO_TYPE 63044

class RpcC2M_ReqGuildTerrAllianceInfo : public CRpc
{
private:

	RpcC2M_ReqGuildTerrAllianceInfo():CRpc(RPCC2M_REQGUILDTERRALLIANCEINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ReqGuildTerrAllianceInfo *CreateRpc()
	{
		return new RpcC2M_ReqGuildTerrAllianceInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqGuildTerrAllianceInfoArg &roArg, const ReqGuildTerrAllianceInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqGuildTerrAllianceInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqGuildTerrAllianceInfoArg &roArg, ReqGuildTerrAllianceInfoRes &roRes);
	void OnDelayReplyRpc(const ReqGuildTerrAllianceInfoArg &roArg, ReqGuildTerrAllianceInfoRes &roRes, const CUserData &roUserData);

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
	ReqGuildTerrAllianceInfoArg m_oArg;
	ReqGuildTerrAllianceInfoRes m_oRes;
};

#endif
