#ifndef __RPCC2M_ENTERBMREADYSCENE_H__
#define __RPCC2M_ENTERBMREADYSCENE_H__

// generate by ProtoGen at date: 2017/6/22 8:19:11

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_ENTERBMREADYSCENE_TYPE 24371

class RpcC2M_EnterBMReadyScene : public CRpc
{
private:

	RpcC2M_EnterBMReadyScene():CRpc(RPCC2M_ENTERBMREADYSCENE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_EnterBMReadyScene *CreateRpc()
	{
		return new RpcC2M_EnterBMReadyScene;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EnterBMReadySceneArg &roArg, const EnterBMReadySceneRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EnterBMReadySceneArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EnterBMReadySceneArg &roArg, EnterBMReadySceneRes &roRes);
	void OnDelayReplyRpc(const EnterBMReadySceneArg &roArg, EnterBMReadySceneRes &roRes, const CUserData &roUserData);

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
	EnterBMReadySceneArg m_oArg;
	EnterBMReadySceneRes m_oRes;
};

#endif
