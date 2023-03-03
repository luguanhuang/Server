#ifndef __RPCM2N_CREATEBATTLESCENE_H__
#define __RPCM2N_CREATEBATTLESCENE_H__

// generate by ProtoGen at date: 2016/11/4 10:52:58

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2N_CREATEBATTLESCENE_TYPE 50269

class RpcM2N_CreateBattleScene : public CRpc
{
private:

	RpcM2N_CreateBattleScene():CRpc(RPCM2N_CREATEBATTLESCENE_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2N_CreateBattleScene *CreateRpc()
	{
		return new RpcM2N_CreateBattleScene;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CreateBattleSceneArg &roArg, const CreateBattleSceneRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CreateBattleSceneArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CreateBattleSceneArg &roArg, CreateBattleSceneRes &roRes);
	void OnDelayReplyRpc(const CreateBattleSceneArg &roArg, CreateBattleSceneRes &roRes, const CUserData &roUserData);

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
	CreateBattleSceneArg m_oArg;
	CreateBattleSceneRes m_oRes;
};

#endif
