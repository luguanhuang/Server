#ifndef __RPCN2G_ENTERSCENE_H__
#define __RPCN2G_ENTERSCENE_H__

// generate by ProtoGen at date: 2016/11/28 12:53:07

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCN2G_ENTERSCENE_TYPE 38481

class RpcN2G_EnterScene : public CRpc
{
private:

	RpcN2G_EnterScene():CRpc(RPCN2G_ENTERSCENE_TYPE)
	{
        m_dwTimeout = 60000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcN2G_EnterScene *CreateRpc()
	{
		return new RpcN2G_EnterScene;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EnterSceneArg &roArg, const EnterSceneRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EnterSceneArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EnterSceneArg &roArg, EnterSceneRes &roRes);
	void OnDelayReplyRpc(const EnterSceneArg &roArg, EnterSceneRes &roRes, const CUserData &roUserData);

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
	EnterSceneArg m_oArg;
	EnterSceneRes m_oRes;
};

#endif
