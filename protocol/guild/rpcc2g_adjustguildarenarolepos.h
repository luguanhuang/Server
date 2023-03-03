#ifndef __RPCC2G_ADJUSTGUILDARENAROLEPOS_H__
#define __RPCC2G_ADJUSTGUILDARENAROLEPOS_H__

// generate by ProtoGen at date: 2016/7/19 18:30:49

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ADJUSTGUILDARENAROLEPOS_TYPE 65149

class RpcC2G_AdjustGuildArenaRolePos : public CRpc
{
private:

	RpcC2G_AdjustGuildArenaRolePos():CRpc(RPCC2G_ADJUSTGUILDARENAROLEPOS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_AdjustGuildArenaRolePos *CreateRpc()
	{
		return new RpcC2G_AdjustGuildArenaRolePos;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AdjustGuildArenaRolePosArg &roArg, const AdjustGuildArenaRolePosRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AdjustGuildArenaRolePosArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AdjustGuildArenaRolePosArg &roArg, AdjustGuildArenaRolePosRes &roRes);
	void OnDelayReplyRpc(const AdjustGuildArenaRolePosArg &roArg, AdjustGuildArenaRolePosRes &roRes, const CUserData &roUserData);

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
	AdjustGuildArenaRolePosArg m_oArg;
	AdjustGuildArenaRolePosRes m_oRes;
};

#endif
