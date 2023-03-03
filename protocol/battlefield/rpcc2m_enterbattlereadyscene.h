#ifndef __RPCC2M_ENTERBATTLEREADYSCENE_H__
#define __RPCC2M_ENTERBATTLEREADYSCENE_H__

// generate by ProtoGen at date: 2017/8/31 12:29:08

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ENTERBATTLEREADYSCENE_TYPE 55233

class RpcC2M_EnterBattleReadyScene : public CRpc
{
private:

	RpcC2M_EnterBattleReadyScene():CRpc(RPCC2M_ENTERBATTLEREADYSCENE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_EnterBattleReadyScene *CreateRpc()
	{
		return new RpcC2M_EnterBattleReadyScene;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EnterBattleReadySceneArg &roArg, const EnterBattleReadySceneRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EnterBattleReadySceneArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EnterBattleReadySceneArg &roArg, EnterBattleReadySceneRes &roRes);
	void OnDelayReplyRpc(const EnterBattleReadySceneArg &roArg, EnterBattleReadySceneRes &roRes, const CUserData &roUserData);

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
	EnterBattleReadySceneArg m_oArg;
	EnterBattleReadySceneRes m_oRes;
};

#endif
