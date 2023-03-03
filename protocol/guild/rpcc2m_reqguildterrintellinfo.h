#ifndef __RPCC2M_REQGUILDTERRINTELLINFO_H__
#define __RPCC2M_REQGUILDTERRINTELLINFO_H__

// generate by ProtoGen at date: 2017/2/8 20:47:23

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_REQGUILDTERRINTELLINFO_TYPE 43276

class RpcC2M_ReqGuildTerrIntellInfo : public CRpc
{
private:

	RpcC2M_ReqGuildTerrIntellInfo():CRpc(RPCC2M_REQGUILDTERRINTELLINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ReqGuildTerrIntellInfo *CreateRpc()
	{
		return new RpcC2M_ReqGuildTerrIntellInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqGuildTerrIntellInfoArg &roArg, const ReqGuildTerrIntellInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqGuildTerrIntellInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqGuildTerrIntellInfoArg &roArg, ReqGuildTerrIntellInfoRes &roRes);
	void OnDelayReplyRpc(const ReqGuildTerrIntellInfoArg &roArg, ReqGuildTerrIntellInfoRes &roRes, const CUserData &roUserData);

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
	ReqGuildTerrIntellInfoArg m_oArg;
	ReqGuildTerrIntellInfoRes m_oRes;
};

#endif
