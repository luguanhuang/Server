#ifndef __RPCC2G_SCENEMOBAOP_H__
#define __RPCC2G_SCENEMOBAOP_H__

// generate by ProtoGen at date: 2017/6/7 17:02:30

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SCENEMOBAOP_TYPE 32594

class RpcC2G_SceneMobaOp : public CRpc
{
private:

	RpcC2G_SceneMobaOp():CRpc(RPCC2G_SCENEMOBAOP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_SceneMobaOp *CreateRpc()
	{
		return new RpcC2G_SceneMobaOp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SceneMobaOpArg &roArg, const SceneMobaOpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SceneMobaOpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SceneMobaOpArg &roArg, SceneMobaOpRes &roRes);
	void OnDelayReplyRpc(const SceneMobaOpArg &roArg, SceneMobaOpRes &roRes, const CUserData &roUserData);

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
	SceneMobaOpArg m_oArg;
	SceneMobaOpRes m_oRes;
};

#endif
