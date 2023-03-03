#ifndef __RPCC2G_CHOOSEROLLREQ_H__
#define __RPCC2G_CHOOSEROLLREQ_H__

// generate by ProtoGen at date: 2016/8/22 17:03:42

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_CHOOSEROLLREQ_TYPE 50047

class RpcC2G_ChooseRollReq : public CRpc
{
private:

	RpcC2G_ChooseRollReq():CRpc(RPCC2G_CHOOSEROLLREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ChooseRollReq *CreateRpc()
	{
		return new RpcC2G_ChooseRollReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ChooseRollReqArg &roArg, const ChooseRollReqRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ChooseRollReqArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ChooseRollReqArg &roArg, ChooseRollReqRes &roRes);
	void OnDelayReplyRpc(const ChooseRollReqArg &roArg, ChooseRollReqRes &roRes, const CUserData &roUserData);

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
	ChooseRollReqArg m_oArg;
	ChooseRollReqRes m_oRes;
};

#endif
