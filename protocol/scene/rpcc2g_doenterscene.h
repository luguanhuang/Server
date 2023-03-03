#ifndef __RPCC2G_DOENTERSCENE_H__
#define __RPCC2G_DOENTERSCENE_H__

// generate by ProtoGen at date: 2016/7/13 19:57:49

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_DOENTERSCENE_TYPE 45201

class RpcC2G_DoEnterScene : public CRpc
{
private:

	RpcC2G_DoEnterScene():CRpc(RPCC2G_DOENTERSCENE_TYPE)
	{
        m_dwTimeout = 600000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_DoEnterScene *CreateRpc()
	{
		return new RpcC2G_DoEnterScene;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DoEnterSceneArg &roArg, const DoEnterSceneRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DoEnterSceneArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DoEnterSceneArg &roArg, DoEnterSceneRes &roRes);
	void OnDelayReplyRpc(const DoEnterSceneArg &roArg, DoEnterSceneRes &roRes, const CUserData &roUserData);

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
	DoEnterSceneArg m_oArg;
	DoEnterSceneRes m_oRes;
};

#endif
