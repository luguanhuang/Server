#ifndef __RPCC2G_JADEOPERATION_H__
#define __RPCC2G_JADEOPERATION_H__

// generate by ProtoGen at date: 2015/6/17 16:33:38

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_JADEOPERATION_TYPE 55991

class RpcC2G_JadeOperation : public CRpc
{
private:

	RpcC2G_JadeOperation():CRpc(RPCC2G_JADEOPERATION_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_JadeOperation *CreateRpc()
	{
		return new RpcC2G_JadeOperation;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const JadeOperationArg &roArg, const JadeOperationRes &roRes, const CUserData &roUserData);
	void OnTimeout(const JadeOperationArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const JadeOperationArg &roArg, JadeOperationRes &roRes);
	void OnDelayReplyRpc(const JadeOperationArg &roArg, JadeOperationRes &roRes, const CUserData &roUserData);

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
	JadeOperationArg m_oArg;
	JadeOperationRes m_oRes;
};

#endif
