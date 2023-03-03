﻿#ifndef __RPCG2M_ASKGUILDBOSSSCENEID_H__
#define __RPCG2M_ASKGUILDBOSSSCENEID_H__

// generate by ProtoGen at date: 2016/8/4 12:01:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_ASKGUILDBOSSSCENEID_TYPE 34697

class RpcG2M_AskGuildBossSceneID : public CRpc
{
private:

	RpcG2M_AskGuildBossSceneID():CRpc(RPCG2M_ASKGUILDBOSSSCENEID_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_AskGuildBossSceneID *CreateRpc()
	{
		return new RpcG2M_AskGuildBossSceneID;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AskGuildBossScene &roArg, const AskGuildBossSceneIDRec &roRes, const CUserData &roUserData);
	void OnTimeout(const AskGuildBossScene &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AskGuildBossScene &roArg, AskGuildBossSceneIDRec &roRes);
	void OnDelayReplyRpc(const AskGuildBossScene &roArg, AskGuildBossSceneIDRec &roRes, const CUserData &roUserData);

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
	AskGuildBossScene m_oArg;
	AskGuildBossSceneIDRec m_oRes;
};

#endif
