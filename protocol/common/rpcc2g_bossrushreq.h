#ifndef __RPCC2G_BOSSRUSHREQ_H__
#define __RPCC2G_BOSSRUSHREQ_H__

// generate by ProtoGen at date: 2016/7/11 21:48:22

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BOSSRUSHREQ_TYPE 44074

class RpcC2G_BossRushReq : public CRpc
{
private:

	RpcC2G_BossRushReq():CRpc(RPCC2G_BOSSRUSHREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_BossRushReq *CreateRpc()
	{
		return new RpcC2G_BossRushReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BossRushArg &roArg, const BossRushRes &roRes, const CUserData &roUserData);
	void OnTimeout(const BossRushArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BossRushArg &roArg, BossRushRes &roRes);
	void OnDelayReplyRpc(const BossRushArg &roArg, BossRushRes &roRes, const CUserData &roUserData);

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
	BossRushArg m_oArg;
	BossRushRes m_oRes;
};

#endif
