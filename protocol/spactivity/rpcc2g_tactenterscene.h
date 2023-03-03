#ifndef __RPCC2G_TACTENTERSCENE_H__
#define __RPCC2G_TACTENTERSCENE_H__

// generate by ProtoGen at date: 2017/7/24 13:44:49

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_TACTENTERSCENE_TYPE 27599

class RpcC2G_TactEnterScene : public CRpc
{
private:

	RpcC2G_TactEnterScene():CRpc(RPCC2G_TACTENTERSCENE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_TactEnterScene *CreateRpc()
	{
		return new RpcC2G_TactEnterScene;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TactEnterSceneArg &roArg, const TactEnterSceneRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TactEnterSceneArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TactEnterSceneArg &roArg, TactEnterSceneRes &roRes);
	void OnDelayReplyRpc(const TactEnterSceneArg &roArg, TactEnterSceneRes &roRes, const CUserData &roUserData);

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
	TactEnterSceneArg m_oArg;
	TactEnterSceneRes m_oRes;
};

#endif
