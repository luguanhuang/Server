﻿#ifndef __RPCC2M_NPCFLREQC2M_H__
#define __RPCC2M_NPCFLREQC2M_H__

// generate by ProtoGen at date: 2017/9/11 10:34:55

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_NPCFLREQC2M_TYPE 11607

class RpcC2M_NpcFlReqC2M : public CRpc
{
private:

	RpcC2M_NpcFlReqC2M():CRpc(RPCC2M_NPCFLREQC2M_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_NpcFlReqC2M *CreateRpc()
	{
		return new RpcC2M_NpcFlReqC2M;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const NpcFlArg &roArg, const NpcFlRes &roRes, const CUserData &roUserData);
	void OnTimeout(const NpcFlArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const NpcFlArg &roArg, NpcFlRes &roRes);
	void OnDelayReplyRpc(const NpcFlArg &roArg, NpcFlRes &roRes, const CUserData &roUserData);

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
	NpcFlArg m_oArg;
	NpcFlRes m_oRes;
};

#endif
