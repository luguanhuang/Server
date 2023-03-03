#ifndef __RPCC2M_ADJUSTGUILDARENAROLEPOSNEW_H__
#define __RPCC2M_ADJUSTGUILDARENAROLEPOSNEW_H__

// generate by ProtoGen at date: 2016/9/22 15:44:37

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ADJUSTGUILDARENAROLEPOSNEW_TYPE 57124

class RpcC2M_AdjustGuildArenaRolePosNew : public CRpc
{
private:

	RpcC2M_AdjustGuildArenaRolePosNew():CRpc(RPCC2M_ADJUSTGUILDARENAROLEPOSNEW_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_AdjustGuildArenaRolePosNew *CreateRpc()
	{
		return new RpcC2M_AdjustGuildArenaRolePosNew;
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
