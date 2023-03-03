#ifndef __RPCC2M_ENTERWEDDINGSCENE_H__
#define __RPCC2M_ENTERWEDDINGSCENE_H__

// generate by ProtoGen at date: 2017/7/17 17:21:11

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ENTERWEDDINGSCENE_TYPE 30316

class RpcC2M_EnterWeddingScene : public CRpc
{
private:

	RpcC2M_EnterWeddingScene():CRpc(RPCC2M_ENTERWEDDINGSCENE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_EnterWeddingScene *CreateRpc()
	{
		return new RpcC2M_EnterWeddingScene;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EnterWeddingSceneArg &roArg, const EnterWeddingSceneRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EnterWeddingSceneArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EnterWeddingSceneArg &roArg, EnterWeddingSceneRes &roRes);
	void OnDelayReplyRpc(const EnterWeddingSceneArg &roArg, EnterWeddingSceneRes &roRes, const CUserData &roUserData);

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
	EnterWeddingSceneArg m_oArg;
	EnterWeddingSceneRes m_oRes;
};

#endif
