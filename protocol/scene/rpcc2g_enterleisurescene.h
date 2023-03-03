#ifndef __RPCC2G_ENTERLEISURESCENE_H__
#define __RPCC2G_ENTERLEISURESCENE_H__

// generate by ProtoGen at date: 2017/10/17 11:08:28

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ENTERLEISURESCENE_TYPE 62361

class RpcC2G_EnterLeisureScene : public CRpc
{
private:

	RpcC2G_EnterLeisureScene():CRpc(RPCC2G_ENTERLEISURESCENE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_EnterLeisureScene *CreateRpc()
	{
		return new RpcC2G_EnterLeisureScene;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EnterLeisureSceneArg &roArg, const EnterLeisureSceneRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EnterLeisureSceneArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EnterLeisureSceneArg &roArg, EnterLeisureSceneRes &roRes);
	void OnDelayReplyRpc(const EnterLeisureSceneArg &roArg, EnterLeisureSceneRes &roRes, const CUserData &roUserData);

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
	EnterLeisureSceneArg m_oArg;
	EnterLeisureSceneRes m_oRes;
};

#endif
