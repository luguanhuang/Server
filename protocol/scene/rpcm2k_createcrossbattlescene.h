#ifndef __RPCM2K_CREATECROSSBATTLESCENE_H__
#define __RPCM2K_CREATECROSSBATTLESCENE_H__

// generate by ProtoGen at date: 2017/4/13 16:18:50

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2K_CREATECROSSBATTLESCENE_TYPE 37652

class RpcM2K_CreateCrossBattleScene : public CRpc
{
private:

	RpcM2K_CreateCrossBattleScene():CRpc(RPCM2K_CREATECROSSBATTLESCENE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2K_CreateCrossBattleScene *CreateRpc()
	{
		return new RpcM2K_CreateCrossBattleScene;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CreateCrossBattleSceneArg &roArg, const CreateCrossBattleSceneRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CreateCrossBattleSceneArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CreateCrossBattleSceneArg &roArg, CreateCrossBattleSceneRes &roRes);
	void OnDelayReplyRpc(const CreateCrossBattleSceneArg &roArg, CreateCrossBattleSceneRes &roRes, const CUserData &roUserData);

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
	CreateCrossBattleSceneArg m_oArg;
	CreateCrossBattleSceneRes m_oRes;
};

#endif
