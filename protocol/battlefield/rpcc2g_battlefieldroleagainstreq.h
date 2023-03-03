﻿#ifndef __RPCC2G_BATTLEFIELDROLEAGAINSTREQ_H__
#define __RPCC2G_BATTLEFIELDROLEAGAINSTREQ_H__

// generate by ProtoGen at date: 2017/9/5 15:12:33

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BATTLEFIELDROLEAGAINSTREQ_TYPE 12475

class RpcC2G_BattleFieldRoleAgainstReq : public CRpc
{
private:

	RpcC2G_BattleFieldRoleAgainstReq():CRpc(RPCC2G_BATTLEFIELDROLEAGAINSTREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_BattleFieldRoleAgainstReq *CreateRpc()
	{
		return new RpcC2G_BattleFieldRoleAgainstReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const BattleFieldRoleAgainstArg &roArg, const BattleFieldRoleAgainst &roRes, const CUserData &roUserData);
	void OnTimeout(const BattleFieldRoleAgainstArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const BattleFieldRoleAgainstArg &roArg, BattleFieldRoleAgainst &roRes);
	void OnDelayReplyRpc(const BattleFieldRoleAgainstArg &roArg, BattleFieldRoleAgainst &roRes, const CUserData &roUserData);

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
	BattleFieldRoleAgainstArg m_oArg;
	BattleFieldRoleAgainst m_oRes;
};

#endif
