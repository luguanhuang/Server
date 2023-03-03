#ifndef __RPCN2G_CREATEBATTLE_H__
#define __RPCN2G_CREATEBATTLE_H__

// generate by ProtoGen at date: 2016/11/4 10:52:51

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCN2G_CREATEBATTLE_TYPE 2620

class RpcN2G_CreateBattle : public CRpc
{
private:

	RpcN2G_CreateBattle():CRpc(RPCN2G_CREATEBATTLE_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcN2G_CreateBattle *CreateRpc()
	{
		return new RpcN2G_CreateBattle;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CreateBattleArg &roArg, const CreateBattleRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CreateBattleArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CreateBattleArg &roArg, CreateBattleRes &roRes);
	void OnDelayReplyRpc(const CreateBattleArg &roArg, CreateBattleRes &roRes, const CUserData &roUserData);

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
	CreateBattleArg m_oArg;
	CreateBattleRes m_oRes;
};

#endif
